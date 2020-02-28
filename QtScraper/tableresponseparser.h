#ifndef TABLERESPONSEPARSER_H
#define TABLERESPONSEPARSER_H

#include <QJsonObject>
#include <QObject>

#include "iqwebscraperresponseparser.h"
#include "parserprototype.h"

class TableResponseParser : public IQWebScraperReponseParser,
                            public ParserPrototype
{
public:    
    TableResponseParser(QObject *parent = nullptr);
    TableResponseParser(QJsonObject jsonObj, QObject *parent = nullptr);
    virtual ~TableResponseParser();
    virtual ParserPrototype *clone(QJsonObject jsonObject = {});
    virtual QJsonArray parse(QString response);
    virtual QString name();
private:
    QString m_name;
    QString m_query;
    QList<int> m_indexes;
    QList<QString> m_headers;
};

#endif // TABLERESPONSEPARSER_H
