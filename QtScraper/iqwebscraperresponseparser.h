#ifndef IQWEBSCRAPERPARSER_H
#define IQWEBSCRAPERPARSER_H

#include <QObject>

class QJsonArray;

class IQWebScraperReponseParser : public QObject
{
    Q_OBJECT
public:
    explicit IQWebScraperReponseParser(QObject *parent = nullptr);
    virtual QJsonArray parse(QString response) = 0;
};

#endif // IQWEBSCRAPERPARSER_H
