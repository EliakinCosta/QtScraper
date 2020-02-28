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
//    qDeleteAll(m_parsers);
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
                        m_parsers.append(parser);
                        this->addRequest("GET", this->evaluateStringToContext(endpoint));
                    }
                }
            } else {
                if (jsonObject.value("data").isArray())
                {
                    QJsonArray postData = jsonObject.value("data").toArray();
                    this->addRequest(
                        "POST",
                        this->evaluateStringToContext(endpoint),
                        postData
                    );
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

    QHash<QString, QString> requestObj;
    requestObj = m_requestsSchedule.at(m_scheduleIndex);

    doHttpRequest(requestObj);
    setStatus(QWebScraperStatus::Loading);
}

void QScrapEngine::addRequest(QString httpMethod, QString endpoint)
{
    QHash<QString, QString> hashObj;

    hashObj.insert("httpMethod", httpMethod);
    hashObj.insert("endpoint", endpoint);    

    m_requestsSchedule.append(hashObj);
}

void QScrapEngine::addRequest(QString httpMethod, QString endpoint, QJsonArray data)
{
    QHash<QString, QString> hashObj;

    hashObj.insert("httpMethod", httpMethod);
    hashObj.insert("endpoint", endpoint);

    // Convert QJsonObject to QString
    QJsonDocument doc(data);
    QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson = QLatin1String(docByteArray);
    hashObj.insert("data", strJson);

    m_requestsSchedule.append(hashObj);
}

QNetworkReply *QScrapEngine::doHttpRequest(QHash<QString, QString> requestObj)
{
    QString httpMethod = requestObj.value("httpMethod");
    QString endpoint = requestObj.value("endpoint");

    if (!endpoint.toLower().startsWith("http"))
        endpoint = m_baseUrl + endpoint;

    m_request.setUrl(endpoint);
    if(httpMethod ==  "GET") {
       return m_manager.get(m_request);
    } else if (httpMethod ==  "POST"){
        QString data = requestObj.value("data");
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonArray obj = doc.array();
        QUrlQuery query;

        for (QJsonArray::const_iterator iter = obj.begin(); iter != obj.end(); ++iter) {
            if (iter->isObject())
            {
                QJsonObject jsonObj = iter->toObject();
                for (QJsonObject::const_iterator it = jsonObj.begin(); it != jsonObj.end(); it++) {
                    query.addQueryItem(it.key(), evaluateStringToContext(it.value().toString()));
                }
            }
        }
        QByteArray postData = query.toString(QUrl::FullyEncoded).toUtf8();

        qDebug() << postData;

        return m_manager.post(m_request, postData);
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
}

void QScrapEngine::saveToContext(QString key, QJsonArray jsonArray)
{
    QScrapEngine::CONTEXT.insert(key, jsonArray);
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

    QHash<QString, QString> requestObj;
    requestObj = m_requestsSchedule.at(m_scheduleIndex);

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

        hashObj.insert("httpMethod", "GET");
        hashObj.insert("endpoint", newUrl.toString());

        auto replyRedirect = doHttpRequest(hashObj);

        connect (replyRedirect, &QNetworkReply::finished, this, [=]() {
            replyRedirect->deleteLater();

            auto statusCode = replyRedirect->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

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
