#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QJsonArray>
#include <QVariantMap>

class HttpRequestModel
{
public:
    HttpRequestModel(QString url, QString method, QVariantMap headers, QJsonArray body = {}, QVariantMap validator = {});
    virtual ~HttpRequestModel(){}
    QString url() const;
    QString method() const;
    QVariantMap headers() const;
    QJsonArray body() const;
    QVariantMap validator() const;
private:
    QString m_url;
    QString m_method;
    QVariantMap m_headers;
    QJsonArray m_body;
    QVariantMap m_validator;
};

#endif // HTTPREQUESTMODEL_H
