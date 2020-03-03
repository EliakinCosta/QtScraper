#ifndef QWEBSCRAPER_H
#define QWEBSCRAPER_H

#include <QQuickItem>

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>

#include "qscrapengine.h"
#include "qwebscraperstatus.h"
#include "iqwebscraperresponseparser.h"
#include "qwebscraperresponseparser.h"

class QWebScraper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QJsonArray actions READ actions WRITE setActions)    
    Q_PROPERTY(QJsonObject ctx READ ctx NOTIFY ctxChanged)
    Q_PROPERTY(QWebScraperStatus::Status status READ status NOTIFY statusChanged)
public:
    explicit QWebScraper(QObject *parent = nullptr);
    virtual ~QWebScraper();
    QJsonArray actions() const;
    void setActions(const QJsonArray &actions);    
    QJsonObject ctx() const;

    QWebScraperStatus::Status status() const;

    Q_INVOKABLE void scrap();

Q_SIGNALS:
    void statusChanged(QWebScraperStatus::Status);
    void ctxChanged(QJsonObject);

private:
    void saveToContext();
private:
    QJsonArray m_actions;    
    QScrapEngine m_scrapEngine;
};

#endif // QWEBSCRAPER_H
