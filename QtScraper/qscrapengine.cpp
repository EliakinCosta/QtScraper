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

void QScrapEngine::parseRequests(QJsonArray &actions)
{
    ParserPrototype::initialize();
    foreach(QJsonValue jsonValue, actions)
    {
        QJsonObject jsonObject = jsonValue.toObject();
        QString endpoint = jsonObject.value("endpoint").toString();
        if (jsonObject.value("method").isString())
        {
            if(jsonObject.value("method").toString() == "GET")
            {
                if (jsonObject.value("scraps").isArray())
                {
                    QJsonArray scraps = jsonObject.value("scraps").toArray();
                    foreach(QJsonValue scrap, scraps)
                    {
                        QJsonObject scrapObject = scrap.toObject();
                        QWebScraperResponseParser::Type type = QWebScraperResponseParser::Type(scrapObject.value("responseParser").toInt());
                        IQWebScraperReponseParser *parser = loadParser(type, scrapObject);
                        HttpRequestModel requestObj(
                            this,
                            this->evaluateStringToContext(endpoint),
                            "GET",
                            jsonObject.value("headers").toObject()
                        );

                        this->addRequest(requestObj);
                        m_parsers.append(parser);
                    }
                }
            } else if(jsonObject.value("method").toString() == "POST"){
                if (jsonObject.value("data").isArray())
                {
                    QJsonArray postData = jsonObject.value("data").toArray();
                    HttpRequestModel requestObj(
                        this,
                        this->evaluateStringToContext(endpoint),
                        jsonObject.value("method").toString(),
                        jsonObject.value("headers").toObject(),
                        postData
                    );
                    this->addRequest(requestObj);
                }
            }
        }
    }
}


void QScrapEngine::scrap()
{
    if (m_requestsSchedule.size() == 0 || m_scheduleIndex >= m_requestsSchedule.size())
    {
        qDebug() << QScrapEngine::CONTEXT;
        setStatus(QWebScraperStatus::Ready);
        return;
    }

    auto requestObj = m_requestsSchedule.at(m_scheduleIndex);

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
        m_request.setRawHeader(key.toUtf8(), requestObj.headers().value(key).toUtf8());

    m_request.setUrl(endpoint);
    if(httpMethod ==  "GET") {
       return m_manager.get(m_request);
    } else if (httpMethod ==  "POST"){
        qDebug() << requestObj.body();
        return m_manager.post(m_request, requestObj.body());
    }

    return nullptr;
}

QString QScrapEngine::fromByteArrayToString(QByteArray html)
{
    return QTextCodec::codecForName("iso-8859-1")->toUnicode(html);
}

void QScrapEngine::saveToContext(QString key, QStringList value)
{
    QScrapEngine::CONTEXT.insert(key, QJsonArray::fromStringList(value));
    Q_EMIT ctxChanged(QScrapEngine::CONTEXT);
}

void QScrapEngine::saveToContext(QString key, QJsonArray jsonArray)
{
    QScrapEngine::CONTEXT.insert(key, jsonArray);
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

    auto requestObj = m_requestsSchedule.at(m_scheduleIndex);

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
        QHash<QString, QString> hashObj;

        auto httpRequestModel = HttpRequestModel(this, newUrl.toString(), "GET", m_requestsSchedule.at(m_scheduleIndex).headersAsJsonObject());

        auto replyRedirect = doHttpRequest(httpRequestModel);

        connect (replyRedirect, &QNetworkReply::finished, this, [=]() {
            replyRedirect->deleteLater();            

            QString payload {replyRedirect->readAll()}; // clazy:exclude=qt4-qstring-from-array
            tidyPayload(payload);

        });
        return;
    }
    QString payload {reply->readAll()}; // clazy:exclude=qt4-qstring-from-array
    tidyPayload(payload);

    auto parser = m_parsers[m_scheduleIndex];
    auto model = parser->parse(payload);

    saveToContext(parser->name(), model);

    m_scheduleIndex++;
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
