#include "qwebscraper.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>

#include "qscrapengine.h"
#include "qwebscraperstatus.h"

QWebScraper::QWebScraper(QObject *parent) :
    QObject{parent}, m_actions({}), m_keepAlive(false)
{
    connect(&m_scrapEngine, &QScrapEngine::statusChanged, this, &QWebScraper::statusChanged);
    connect(&m_scrapEngine, &QScrapEngine::ctxChanged, this, &QWebScraper::ctxChanged);    
}

QWebScraper::~QWebScraper()
{

}

QQmlListProperty<QWebScraperAction> QWebScraper::actions()
{
    return QQmlListProperty<QWebScraperAction>(this, this,
             &QWebScraper::appendAction,
             &QWebScraper::actionCount,
             &QWebScraper::action,
             &QWebScraper::clearActions);
}

void QWebScraper::appendAction(QWebScraperAction *action)
{
    m_actions.append(action);
    connect(action, &QWebScraperAction::validChanged, this, &QWebScraper::actionsChanged);
    Q_EMIT actionsChanged();
}

int QWebScraper::actionCount() const
{
    return m_actions.count();
}

QWebScraperAction *QWebScraper::action(int index) const
{
    return m_actions.at(index);
}

void QWebScraper::clearActions() {
    m_actions.clear();
    Q_EMIT actionsChanged();
}


void QWebScraper::appendAction(QQmlListProperty<QWebScraperAction>* list, QWebScraperAction* p) {
    reinterpret_cast< QWebScraper* >(list->data)->appendAction(p);
}

void QWebScraper::clearActions(QQmlListProperty<QWebScraperAction>* list) {
    reinterpret_cast< QWebScraper* >(list->data)->clearActions();
}

QWebScraperAction* QWebScraper::action(QQmlListProperty<QWebScraperAction>* list, int i) {
    return reinterpret_cast< QWebScraper* >(list->data)->action(i);
}

int QWebScraper::actionCount(QQmlListProperty<QWebScraperAction>* list) {
    return reinterpret_cast< QWebScraper* >(list->data)->actionCount();
}

QJsonObject QWebScraper::ctx() const
{
    return m_scrapEngine.CONTEXT;
}

QWebScraperStatus::Status QWebScraper::status() const
{
    return m_scrapEngine.status();
}

bool QWebScraper::keepAlive() const
{
    return m_keepAlive;
}

void QWebScraper::setKeepAlive(const bool keepAlive)
{
    m_keepAlive = keepAlive;
}

void QWebScraper::scrap()
{
    m_scrapEngine.parseRequests(m_actions, m_keepAlive);
    m_scrapEngine.scrap();
}

void QWebScraper::clearCookies()
{
    m_scrapEngine.clearCookieSettings();
}
