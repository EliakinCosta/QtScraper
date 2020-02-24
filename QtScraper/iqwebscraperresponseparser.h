#ifndef IQWEBSCRAPERPARSER_H
#define IQWEBSCRAPERPARSER_H

#include <QObject>

class IQWebScraperReponseParser : public QObject
{
    Q_OBJECT
public:
    explicit IQWebScraperReponseParser(QObject *parent = nullptr);

signals:

};

#endif // IQWEBSCRAPERPARSER_H
