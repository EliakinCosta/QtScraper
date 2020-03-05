#include "qscrapengine.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QTextCodec>
#include <QRegularExpression>
#include <QStringLiteral>
#include <QUrlQuery>

#include <tidy.h>
#include <tidybuffio.h>

#include "qwebscraperstatus.h"
#include "parserprototype.h"
#include "httprequestmodel.h"


QJsonObject QScrapEngine::CONTEXT;

ScrapReply::ScrapReply(QObject *parent)
    : QObject {parent}
{

}

QScrapEngine::QScrapEngine(QObject *parent) : QObject(parent)
{
    m_request.setHeader(
                QNetworkRequest::ContentTypeHeader,
                QStringLiteral("application/x-www-form-urlencoded")
                );

    m_request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    QSslConfiguration conf = m_request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);

    m_request.setSslConfiguration(conf);

    QObject::connect(&m_manager, &QNetworkAccessManager::finished,
                     this, &QScrapEngine::replyFinished);

}

QScrapEngine::~QScrapEngine()
{

}

void QScrapEngine::tidyPayload(QString &payload)
{
    TidyDoc tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyXmlOut, yes);
    tidyOptSetBool(tdoc, TidyQuiet, yes);
    tidyOptSetBool(tdoc, TidyNumEntities, yes);
    tidyOptSetBool(tdoc, TidyShowWarnings, no);


    tidyParseString(tdoc, payload.toUtf8());
    tidyCleanAndRepair(tdoc);
    TidyBuffer output = {nullptr, nullptr, 0, 0, 0};
    tidySaveBuffer(tdoc, &output);

    payload = QString::fromUtf8(reinterpret_cast<char*>(output.bp)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

IQWebScraperReponseParser *QScrapEngine::loadParser(QWebScraperResponseParser::Type type, QJsonObject jsonObj)
{
    return dynamic_cast<IQWebScraperReponseParser*>(ParserPrototype::create(type, jsonObj));
}

QString QScrapEngine::parseBaseUrl(QString endpoint)
{
    QRegularExpression re("^.+?[^/:](?=[?/]|$)");
    QRegularExpressionMatchIterator i = re.globalMatch(endpoint);
    QRegularExpressionMatch match = i.next();
    QString baseUrl = match.captured();
    return baseUrl;
}

void QScrapEngine::parseRequests(QVector<QWebScraperAction*> actions)
{
    m_actions = actions;
    ParserPrototype::initialize();

    foreach(QWebScraperAction *action, m_actions)
    {
        action->parseScraps();
        QString endpoint = action->endpoint();
        if (m_baseUrl.isEmpty())
            m_baseUrl = parseBaseUrl(endpoint);
        QString method = action->method();

        if(method == "GET")
        {
            HttpRequestModel requestObj(
                this->evaluateStringToContext(endpoint),
                method,
                action->headers()
            );
            this->addRequest(requestObj);
        } else if(method == "POST"){
            HttpRequestModel requestObj(
                this->evaluateStringToContext(endpoint),
                method,
                action->headers(),
                action->data(),
                action->validator()
            );
            this->addRequest(requestObj);
        }
    }
}


void QScrapEngine::scrap()
{
    if (m_requestsSchedule.size() == 0 || m_requestScheduleIndex >= m_requestsSchedule.size())
    {
        qDebug() << QScrapEngine::CONTEXT;
        setStatus(QWebScraperStatus::Ready);
        return;
    }

    auto requestObj = m_requestsSchedule.at(m_requestScheduleIndex);

    doHttpRequest(requestObj);
    setStatus(QWebScraperStatus::Loading);
}

void QScrapEngine::addRequest(HttpRequestModel requestObj)
{
    m_requestsSchedule.append(requestObj);
}

QNetworkReply *QScrapEngine::doHttpRequest(HttpRequestModel requestObj)
{
    QString httpMethod = requestObj.method();
    QString endpoint = requestObj.url();

    if (!endpoint.toLower().startsWith("http"))
        endpoint = m_baseUrl + endpoint;

    foreach(QString key, requestObj.headers().keys())
        m_request.setRawHeader(key.toUtf8(), requestObj.headers().value(key).toString().toUtf8());

    m_request.setUrl(endpoint);
    if(httpMethod ==  "GET") {
        return m_manager.get(m_request);
    } else if (httpMethod ==  "POST"){
        QByteArray body = parseRequestBody(requestObj.body());
        qDebug() << body;
        return m_manager.post(m_request, body);
    }

    return nullptr;
}

QString QScrapEngine::fromByteArrayToString(QByteArray html)
{
    return QTextCodec::codecForName("iso-8859-1")->toUnicode(html);
}

void QScrapEngine::saveToContext(QJsonObject jsonObject)
{
    QScrapEngine::CONTEXT.insert(jsonObject.value("objectName").toString(), jsonObject.value("jsonArray").toArray());
    Q_EMIT ctxChanged(QScrapEngine::CONTEXT);
}

void QScrapEngine::saveToContext(QString key, QJsonArray jsonArray)
{
    QScrapEngine::CONTEXT.insert(key, jsonArray);
    qDebug() << QScrapEngine::CONTEXT;
    Q_EMIT ctxChanged(QScrapEngine::CONTEXT);
}

QString QScrapEngine::evaluateStringToContext(QString value)
{
    QString new_value = value;
    QRegularExpression re("%%(.*?)%%");
    QRegularExpressionMatchIterator i = re.globalMatch(value);
    while (i.hasNext()) {
        // For instance, we can capture only one variable for string.
        QRegularExpressionMatch match = i.next();
        QString templateKey = match.captured(1);
        QString templateValue = QScrapEngine::CONTEXT.value(templateKey).toArray().first().toString();

        new_value = value.replace(QString("%%%1%%").arg(templateKey), templateValue);
    }
    qDebug() << new_value;
    return new_value;
}

QJsonObject QScrapEngine::objectFromString(const QString& in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << in << endl;
    }

    return obj;
}

void QScrapEngine::setBaseUrl(QString baseUrl)
{
    m_baseUrl = baseUrl;
}

void QScrapEngine::replyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    auto requestObj = m_requestsSchedule.at(m_requestScheduleIndex);

    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        qDebug() << "ERRO:" << reply->errorString().toLower();
        setStatus(QWebScraperStatus::Error);
        return;
    }
    auto statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode != 200 && statusCode != 302) {
        setStatus(QWebScraperStatus::Error);
        return;
    }

    if(statusCode == 302)
    {
        QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        qDebug() << "redirected to " + newUrl.toString();

        auto httpRequestModel = HttpRequestModel(newUrl.toString(), "GET", {});
        this->addRequest(httpRequestModel);

        m_requestScheduleIndex++;
        scrap();
        return;
    }
    QString payload {reply->readAll()}; // clazy:exclude=qt4-qstring-from-array
    tidyPayload(payload);

    auto model = m_actions.at(m_currentActionIndex)->parseScraps(payload);

    saveToContext(model);

    m_requestScheduleIndex++;
    m_currentActionIndex++;
    scrap();
}

QWebScraperStatus::Status QScrapEngine::status() const
{
    return m_status;
}

void QScrapEngine::setStatus(QWebScraperStatus::Status status)
{
    if (m_status!=status)
    {
        m_status = status;
        Q_EMIT statusChanged(m_status);
    }
}

QByteArray QScrapEngine::parseRequestBody(QJsonArray body)
{
    QUrlQuery query;
    for (QJsonArray::const_iterator iter = body.begin(); iter != body.end(); ++iter) {
        if (iter->isObject())
        {
            QJsonObject jsonObj = iter->toObject();
            for (QJsonObject::const_iterator it = jsonObj.begin(); it != jsonObj.end(); it++) {
                query.addQueryItem(it.key(), this->evaluateStringToContext(it.value().toString()));
            }
        }
    }
    return query.toString(QUrl::FullyEncoded).toUtf8();
}
