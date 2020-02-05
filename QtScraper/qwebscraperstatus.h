#ifndef QWEBSCRAPERSTATUS_H
#define QWEBSCRAPERSTATUS_H

#include <QObject>

class QWebScraperStatus : public QObject
{
    Q_OBJECT
public:
    explicit QWebScraperStatus(QObject *parent = nullptr);
    enum Status {
        Null,
        Ready,
        Loading,
        Error
    };
    Q_ENUM(Status)
};

#endif // QWEBSCRAPERSTATUS_H
