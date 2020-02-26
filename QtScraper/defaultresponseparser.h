#ifndef DEFAULTRESPONSEPARSER_H
#define DEFAULTRESPONSEPARSER_H

#include <QJsonObject>
#include <QObject>

#include "iqwebscraperresponseparser.h"
#include "parserprototype.h"

class DefaultResponseParser: public IQWebScraperReponseParser,
                             public ParserPrototype
{    
public:
    DefaultResponseParser(QObject *parent = nullptr);
    DefaultResponseParser(QJsonObject jsonObj, QObject *parent = nullptr);
    virtual ~DefaultResponseParser();
    virtual ParserPrototype *clone(QJsonObject jsonObject = {});
    virtual QJsonArray parse(QString response);
private:
    QString m_name;
    QString m_query;
    QList<int> m_indexes;
    QList<QString> m_headers;
};

#endif // DEFAULTRESPONSEPARSER_H
