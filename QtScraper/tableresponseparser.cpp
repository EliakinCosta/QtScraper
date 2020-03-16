#include "tableresponseparser.h"

#include <QJsonArray>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QListIterator>

TableResponseParser::TableResponseParser(QObject *parent):IQWebScraperReponseParser(parent)
{

}

TableResponseParser::TableResponseParser(QJsonObject jsonObject, QObject *parent):IQWebScraperReponseParser(parent)
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
            if (jsonStr.indexOf(",") != -1)
            {
                auto strList = jsonStr.split(",");
                jsonStr = strList.first();
                m_customQueries.insert(jsonStr, strList.last());
            }

            this->m_headers.append(jsonStr);
        }
    }
}

TableResponseParser::~TableResponseParser()
{

}

ParserPrototype *TableResponseParser::clone(QJsonObject jsonObject)
{
    return new TableResponseParser(jsonObject);
}

QJsonArray TableResponseParser::parse(QString response)
{
    QXmlQuery xmlQuery;
    QJsonArray resultArray;
    QMap<QString, QList<QString>> rawTable;
    int listSize = 0;

    xmlQuery.setFocus(response);

    for (int index = 0; index < m_indexes.size(); index++)
    {
        int tableIndex = m_indexes[index];

        QString index_query = QString(m_query).replace("$index", QString::number(tableIndex));
        if (m_customQueries.contains(m_headers[index]))
                index_query = m_customQueries.value(m_headers[index]).toString();
        xmlQuery.setQuery(index_query);

        QStringList list;

        if (xmlQuery.isValid()) {
            xmlQuery.evaluateTo(&list);
            if (list.size() > 0)
            {
                rawTable.insert(m_headers[index], list);
                listSize = list.size();
            }
        }        
    }

    for (int index = 0; index < listSize; index++)
    {
        QMap<QString, QVariant> tableObject;
        foreach(QString key, rawTable.keys())
        {
            tableObject.insert(key, rawTable.value(key)[index]);
        }
        resultArray.append(QJsonObject::fromVariantMap(tableObject));
    }

    return resultArray;
}

QString TableResponseParser::name()
{
    return m_name;
}
