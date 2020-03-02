#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QMap>
#include <QJsonArray>
#include <QJsonObject>

class QByteArray;
class QScrapEngine;

class HttpRequestModel
{
public:
    HttpRequestModel(QScrapEngine *scrapEngine, QString url, QString method, QJsonObject headers, QJsonArray body = {});
    virtual ~HttpRequestModel(){}
    QString url() const;
    QString method() const;
    QMap<QString, QString> headers() const;
    QJsonObject headersAsJsonObject() const;
    QByteArray body() const;
private:
    QString m_url;
    QString m_method;
    QMap<QString, QString> m_headers;
    QJsonObject m_headersAsJsonObject;
    QByteArray m_body;
};

#endif // HTTPREQUESTMODEL_H
