#include "qwebscraperaction.h"

#include <QDebug>
#include <QJsonObject>

#include "qwebscraperresponseparser.h"
#include "parserprototype.h"

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

IQWebScraperReponseParser *QWebScraperAction::loadParser(QWebScraperResponseParser::Type type, QJsonObject jsonObj)
{
    return dynamic_cast<IQWebScraperReponseParser*>(ParserPrototype::create(type, jsonObj));
}

void QWebScraperAction::loadScraps()
{        
    foreach(QJsonValue scrap, this->scraps())
    {
        QJsonObject scrapObject = scrap.toObject();
        QWebScraperResponseParser::Type type = QWebScraperResponseParser::Type(scrapObject.value("responseParser").toInt());
        IQWebScraperReponseParser *parser = loadParser(type, scrapObject);                ;
        m_parsers.append(parser);
    }
}

QJsonArray QWebScraperAction::parseScraps(QString payload)
{
    QJsonArray model;
    foreach (IQWebScraperReponseParser *parser, this->m_parsers)
    {
        QJsonObject obj;
        obj.insert("name", parser->name());
        obj.insert("value", parser->parse(payload));
        model.append(obj);
    }
    return model;
}
