#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QMap>
#include <QJsonArray>

class QByteArray;
class QJsonObject;

class HttpRequestModel
{
public:
    HttpRequestModel(QString url, QString method, QJsonObject headers, QJsonArray body = {});
    virtual ~HttpRequestModel(){}
    QString url() const;
    QString method() const;
    QMap<QString, QString> headers() const;
    QByteArray body() const;
private:
    QString m_url;
    QString m_method;
    QMap<QString, QString> m_headers;
    QByteArray m_body;
};

#endif // HTTPREQUESTMODEL_H
