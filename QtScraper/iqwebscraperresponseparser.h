#ifndef IQWEBSCRAPERPARSER_H
#define IQWEBSCRAPERPARSER_H

#include <QtCore/QObject>

class QJsonArray;

class IQWebScraperReponseParser : public QObject
{
    Q_OBJECT
public:
    explicit IQWebScraperReponseParser(QObject *parent = nullptr);
    virtual ~IQWebScraperReponseParser(){}
    virtual QJsonArray parse(QString response) = 0;
    virtual QString name() = 0;
};

#endif // IQWEBSCRAPERPARSER_H
