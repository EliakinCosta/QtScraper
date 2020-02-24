#ifndef DEFAULTRESPONSEPARSER_H
#define DEFAULTRESPONSEPARSER_H

#include "iqwebscraperresponseparser.h"


class DefaultResponseParser : public IQWebScraperReponseParser
{
public:
    explicit DefaultResponseParser(QObject *parent = nullptr);
    virtual ~DefaultResponseParser();
};

#endif // DEFAULTRESPONSEPARSER_H
