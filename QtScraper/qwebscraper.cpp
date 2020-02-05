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
    foreach(QJsonValue jsonValue, m_actions)
    {
        QJsonObject jsonObject = jsonValue.toObject();
        QString endpoint = jsonObject.value("endpoint").toString();
        if (jsonObject.value("method").isString())
        {
            if(jsonObject.value("method").toString() == "GET")
            {
                if (jsonObject.value("scraps").isArray())
                {
                    QJsonArray scraps = jsonObject.value("scraps").toArray();
                    foreach(QJsonValue scrap, scraps)
                    {
                        QJsonObject scrapObject = scrap.toObject();
                        m_scrapEngine.addRequest(
                                    "GET",
                                    m_scrapEngine.evaluateStringToContext(endpoint),
                                    m_scrapEngine.evaluateStringToContext(scrapObject.value("name").toString()),
                                    m_scrapEngine.evaluateStringToContext(scrapObject.value("query").toString())
                                    );
                    }
                }
            } else {
                if (jsonObject.value("data").isArray())
                {
                    QJsonArray postData = jsonObject.value("data").toArray();
                    m_scrapEngine.addRequest(
                        "POST",
                        m_scrapEngine.evaluateStringToContext(endpoint),
                        postData
                    );
                }
            }
        }

    }
    m_scrapEngine.scrap();
}

void QWebScraper::saveToContext()
{
    return;
}
