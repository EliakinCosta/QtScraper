#include "parserprototype.h"

#include <QJsonObject>

#include "iqwebscraperresponseparser.h"
#include "defaultresponseparser.h"
#include "tableresponseparser.h"

QMap<QWebScraperResponseParser::Type, ParserPrototype*> ParserPrototype::PROTOTYPES;

ParserPrototype::ParserPrototype()
{

}

ParserPrototype::~ParserPrototype()
{
    qDeleteAll(PROTOTYPES);
}

void ParserPrototype::initialize()
{
    PROTOTYPES.insert(QWebScraperResponseParser::DefaultParser, new DefaultResponseParser);
    PROTOTYPES.insert(QWebScraperResponseParser::TableParser, new TableResponseParser);
}

ParserPrototype *ParserPrototype::create(QWebScraperResponseParser::Type type, QJsonObject jsonObj)
{
    if(PROTOTYPES.contains(type))
    {
        return PROTOTYPES[type]->clone(jsonObj);
    }    
}
