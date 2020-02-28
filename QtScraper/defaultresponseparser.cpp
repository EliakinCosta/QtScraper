#include "defaultresponseparser.h"

#include <QJsonArray>
#include <QXmlQuery>
#include <QXmlResultItems>

DefaultResponseParser::DefaultResponseParser(QObject *parent)
{

}

DefaultResponseParser::DefaultResponseParser(QJsonObject jsonObject, QObject *parent)
{
    this->m_name = jsonObject.value("name").toString();
    this->m_query = jsonObject.value("query").toString();    
}

DefaultResponseParser::~DefaultResponseParser()
{

}

ParserPrototype *DefaultResponseParser::clone(QJsonObject jsonObject)
{
    return new DefaultResponseParser(jsonObject);
}

QJsonArray DefaultResponseParser::parse(QString response)
{
    QXmlQuery xmlQuery;
    QStringList list;

    xmlQuery.setFocus(response);
    xmlQuery.setQuery(m_query);

    if (!xmlQuery.isValid()) {
        return QJsonArray();
    }

    xmlQuery.evaluateTo(&list);

    return QJsonArray::fromStringList(list);
}

QString DefaultResponseParser::name()
{
    return m_name;
}
