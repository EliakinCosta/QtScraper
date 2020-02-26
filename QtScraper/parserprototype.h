#ifndef PARSERPROTOTYPE_H
#define PARSERPROTOTYPE_H

#include <QMap>
#include <QJsonObject>

#include "qwebscraperresponseparser.h"

class ParserPrototype
{
public:
    ParserPrototype();
    virtual ~ParserPrototype();
    virtual ParserPrototype *clone(QJsonObject jsonObj = {}) = 0;
    static void initialize();
    static ParserPrototype *create(QWebScraperResponseParser::Type, QJsonObject jsonObj);
    static QMap<QWebScraperResponseParser::Type, ParserPrototype*> PROTOTYPES;
};

#endif // PARSERPROTOTYPE_H
