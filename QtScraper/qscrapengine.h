#ifndef QSCRAPENGINE_H
#define QSCRAPENGINE_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

#include "qwebscraperresponseparser.h"
#include "qwebscraperstatus.h"
#include "qwebscraperaction.h"
#include "qnamsingleton.h"

class QByteArray;
class QNetworkReply;
class QJsonArray;
class HttpRequestModel;

class MyCookieJar : public QNetworkCookieJar
{
    public:
        QList<QNetworkCookie> getAllCookies() { return allCookies(); }
        void setNewCookies(QList<QNetworkCookie> cookies) { setAllCookies(cookies); }
};

class ScrapReply : public QObject
{
    Q_OBJECT

public:
    explicit ScrapReply(QObject *parent = nullptr);

Q_SIGNALS:
    void finished(const QStringList &result);
    void finished(const QHash<QString, QStringList> &result);
    void finished(int statusCode, bool result);
};

class QScrapEngine : public QObject
{
    Q_OBJECT
public:
    explicit QScrapEngine(QObject *parent = nullptr);
    virtual ~QScrapEngine();
    void scrap();
    void clearCookieSettings();
    void parseRequests(QVector<QWebScraperAction*> actions, bool keepAlive);
    void setBaseUrl(QString baseUrl);
    void addRequest(HttpRequestModel requestObj);
    QString evaluateStringToContext(QString value);

    static void tidyPayload(QString &payload);
    static QJsonObject CONTEXT;

    QWebScraperStatus::Status status() const;
    void setStatus(QWebScraperStatus::Status status);
public slots:
    void replyFinished();
Q_SIGNALS:
     void statusChanged(QWebScraperStatus::Status status);
     void ctxChanged(QJsonObject jsonObject);
     void actionsChanged();
private:
    QNetworkReply *doHttpRequest(HttpRequestModel requestObj);
    QString fromByteArrayToString(QByteArray html);
    void saveToContext(QJsonObject jsonObject);
    void saveToContext(QString key, QJsonArray jsonArray);
    QJsonObject objectFromString(const QString& in);    
    QByteArray parseRequestBody(QJsonArray body);
    QString parseBaseUrl(QString endpoint);
    void resetSchedule();
    void saveCookiejar();
    void loadCookieJar();
private:    
    MyCookieJar *m_cookieJar;
    QNetworkRequest m_request;
    QList<HttpRequestModel> m_requestsSchedule;
    QVector<QWebScraperAction*> m_actions;
    QString m_baseUrl;
    int m_requestScheduleIndex = 0;
    int m_currentActionIndex = 0;
    QWebScraperStatus::Status m_status;
    bool m_keepAlive;
    QNAMSingleton m_manager;
};

#endif // QSCRAPENGINE_H
