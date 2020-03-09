#ifndef QWEBSCRAPER_H
#define QWEBSCRAPER_H

#include <QQuickItem>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QList>
#include <QVector>
#include <QQmlListProperty>

#include "qscrapengine.h"
#include "qwebscraperstatus.h"
#include "iqwebscraperresponseparser.h"
#include "qwebscraperresponseparser.h"
#include "qwebscraperaction.h"

class QWebScraper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<QWebScraperAction> actions READ actions NOTIFY actionsChanged)
    Q_PROPERTY(QJsonObject ctx READ ctx NOTIFY ctxChanged)
    Q_PROPERTY(QWebScraperStatus::Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool keepAlive READ keepAlive WRITE setKeepAlive)
public:
    explicit QWebScraper(QObject *parent = nullptr);
    virtual ~QWebScraper();

    QQmlListProperty<QWebScraperAction> actions();
    void appendAction(QWebScraperAction*);
    int actionCount() const;
    QWebScraperAction *action(int) const;
    void clearActions();

    QJsonObject ctx() const;

    QWebScraperStatus::Status status() const;

    bool keepAlive() const;
    void setKeepAlive(const bool keepAlive);

    Q_INVOKABLE void scrap();
    Q_INVOKABLE void clearCookies();

Q_SIGNALS:
    void statusChanged(QWebScraperStatus::Status);
    void ctxChanged(QJsonObject);
    void actionsChanged();
private:
    static void appendAction(QQmlListProperty<QWebScraperAction>*, QWebScraperAction*);
    static int actionCount(QQmlListProperty<QWebScraperAction>*);
    static QWebScraperAction* action(QQmlListProperty<QWebScraperAction>*, int);
    static void clearActions(QQmlListProperty<QWebScraperAction>*);    

    QVector<QWebScraperAction *> m_actions;
    QScrapEngine m_scrapEngine;
    bool m_keepAlive;
};

#endif // QWEBSCRAPER_H
