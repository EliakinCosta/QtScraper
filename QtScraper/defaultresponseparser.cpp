#include "defaultresponseparser.h"

#include <QJsonArray>


DefaultResponseParser::DefaultResponseParser(QObject *parent)
{

}

DefaultResponseParser::DefaultResponseParser(QJsonObject jsonObject, QObject *parent)
{
    this->m_name = jsonObject.value("name").toString();
    this->m_query = jsonObject.value("query").toString();

    auto objIndexes = jsonObject.value("indexes").toArray();
    for (QJsonArray::const_iterator iter = objIndexes.begin(); iter != objIndexes.end(); ++iter) {
        int jsonInt = iter->toInt();
        this->m_indexes.append(jsonInt);
     }

    auto objHeaders = jsonObject.value("headers").toArray();
    for (QJsonArray::const_iterator iter = objHeaders.begin(); iter != objHeaders.end(); ++iter) {
        if (iter->isString())
        {
            QString jsonStr = iter->toString();
            this->m_headers.append(jsonStr);
        }
    }
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
    return QJsonArray();
}
