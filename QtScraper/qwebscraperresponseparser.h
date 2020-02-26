#ifndef QWEBSCRAPERPARSER_H
#define QWEBSCRAPERPARSER_H

#include <QObject>

class QWebScraperResponseParser : public QObject
{
    Q_OBJECT
public:
    explicit QWebScraperResponseParser(QObject *parent = nullptr);
    enum Type {
        DefaultParser,
        TableParser
    };
    Q_ENUM(Type)
};

#endif // QWEBSCRAPERPARSER_H
