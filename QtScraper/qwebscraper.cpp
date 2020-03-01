#include "qwebscraper.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QObject>
#include <QQmlEngine>

#include "qscrapengine.h"
#include "qwebscraperstatus.h"

QWebScraper::QWebScraper(QObject *parent) :
    QObject{parent}
{
    connect(&m_scrapEngine, &QScrapEngine::statusChanged, this, &QWebScraper::statusChanged);
    connect(&m_scrapEngine, &QScrapEngine::ctxChanged, this, &QWebScraper::ctxChanged);
}

QWebScraper::~QWebScraper()
{

}

QJsonArray QWebScraper::actions() const
{
    return m_actions;
}

void QWebScraper::setActions(const QJsonArray &actions)
{
    m_actions = actions;
}

QJsonObject QWebScraper::ctx() const
{
    return m_scrapEngine.CONTEXT;
}

QString QWebScraper::url() const
{
    return m_url;
}

void QWebScraper::setUrl(const QString url)
{
    m_url = url;
}

QWebScraperStatus::Status QWebScraper::status() const
{
    return m_scrapEngine.status();
}

void QWebScraper::scrap()
{
    m_scrapEngine.setBaseUrl(m_url);    
    m_scrapEngine.parseRequests(m_actions);
    m_scrapEngine.scrap();
}

void QWebScraper::saveToContext()
{
    return;
}
