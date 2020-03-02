#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QMap>
#include <QJsonArray>
#include <QJsonObject>

class HttpRequestModel
{
public:
    HttpRequestModel(QString url, QString method, QJsonObject headers, QJsonArray body = {}, QJsonObject validator = {});
    virtual ~HttpRequestModel(){}
    QString url() const;
    QString method() const;
    QMap<QString, QString> headers() const;
    QJsonObject headersAsJsonObject() const;
    QJsonArray body() const;
    QJsonObject validator() const;
private:
    QString m_url;
    QString m_method;
    QMap<QString, QString> m_headers;
    QJsonObject m_headersAsJsonObject;
    QJsonArray m_body;
    QJsonObject m_validator;
};

#endif // HTTPREQUESTMODEL_H
