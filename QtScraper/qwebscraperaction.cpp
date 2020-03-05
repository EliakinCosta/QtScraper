#include "qwebscraperaction.h"

#include <QDebug>
#include <QJsonObject>

#include "qwebscraperresponseparser.h"

QWebScraperAction::QWebScraperAction(QObject *parent) : QObject(parent)
{
    this->setMethod("GET");
}

QWebScraperAction::~QWebScraperAction()
{

}

QString QWebScraperAction::endpoint() const
{
    return m_endpoint;
}

void QWebScraperAction::setEndpoint(const QString endpoint)
{
    m_endpoint = endpoint;
}

QString QWebScraperAction::method() const
{
    return m_method;
}

void QWebScraperAction::setMethod(const QString method)
{
    m_method = "GET";
    if (!method.isEmpty())
        m_method = method;
}

QVariantMap QWebScraperAction::headers() const
{
    return m_headers;
}

void QWebScraperAction::setHeaders(const QVariantMap headers)
{
    m_headers = headers;
}

QJsonArray QWebScraperAction::scraps() const
{
    return m_scraps;
}

void QWebScraperAction::setScraps(const QJsonArray scraps)
{
    m_scraps = scraps;
}

QJsonArray QWebScraperAction::data() const
{
    return m_data;
}

void QWebScraperAction::setData(const QJsonArray data)
{
    m_data = data;
}

QVariantMap QWebScraperAction::validator() const
{
    return m_validator;
}

void QWebScraperAction::setValidator(const QVariantMap validator)
{
    m_validator = validator;
}

void QWebScraperAction::appendParser(IQWebScraperReponseParser *parser)
{
    m_parsers.append(parser);
}

void QWebScraperAction::loadScraps()
{
    if(this->method() == "GET")
    {
        foreach(QJsonValue scrap, this->scraps())
        {
            QJsonObject scrapObject = scrap.toObject();
            QWebScraperResponseParser::Type type = QWebScraperResponseParser::Type(scrapObject.value("responseParser").toInt());
            IQWebScraperReponseParser *parser = loadParser(type, scrapObject);                ;
            m_parsers.append(parser);
        }
    } else if(this->method() == "POST"){
            QJsonObject scrapObject;
            IQWebScraperReponseParser *parser = loadParser(QWebScraperResponseParser::DefaultParser, scrapObject);
            QJsonArray postData = this->data();            ;
            m_parsers.append(parser);
    }
}

void parseScraps(QString payload)
{

}
