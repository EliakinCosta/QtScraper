#ifndef QWEBSCRAPERPARSER_H
#define QWEBSCRAPERPARSER_H

#include <QObject>

class QWebScraperParser : public QObject
{
    Q_OBJECT
public:
    explicit QWebScraperParser(QObject *parent = nullptr);
    enum Type {
        DefaultParser,
        TableParser
    };
    Q_ENUM(Type)

signals:

};

#endif // QWEBSCRAPERPARSER_H
