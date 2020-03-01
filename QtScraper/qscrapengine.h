#ifndef QSCRAPENGINE_H
#define QSCRAPENGINE_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "qwebscraperstatus.h"
#include "iqwebscraperresponseparser.h"
#include "qwebscraperresponseparser.h"

class QByteArray;
class QNetworkReply;
class QJsonArray;

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
    void parseRequests(QJsonArray &actions);
    void setBaseUrl(QString baseUrl);
    void addRequest(QString httpMethod, QString endpoint);
    void addRequest(QString httpMethod, QString endpoint, QJsonArray data);
    QString evaluateStringToContext(QString value);
    static void tidyPayload(QString &payload);
    static QJsonObject CONTEXT;

    QWebScraperStatus::Status status() const;
    void setStatus(QWebScraperStatus::Status status);
public slots:
    void replyFinished(QNetworkReply *reply);
Q_SIGNALS:
     void statusChanged(QWebScraperStatus::Status status);
     void ctxChanged(QJsonObject jsonObject);
private:
    QNetworkReply *doHttpRequest(QHash<QString, QString> requestObj);
    QString fromByteArrayToString(QByteArray html);
    void saveToContext(QString key, QStringList value);
    void saveToContext(QString key, QJsonArray jsonArray);
    QJsonObject objectFromString(const QString& in);
    IQWebScraperReponseParser *loadParser(QWebScraperResponseParser::Type, QJsonObject jsonObj);
private:
    QNetworkAccessManager m_manager;
    QNetworkRequest m_request;
    QList<QHash<QString, QString>> m_requestsSchedule;
    QList<IQWebScraperReponseParser*> m_parsers;
    QString m_baseUrl;
    int m_scheduleIndex = 0;
    QWebScraperStatus::Status m_status;

};

#endif // QSCRAPENGINE_H
