#include "httprequestmodel.h"

#include <QJsonObject>
#include <QUrlQuery>

#include "qscrapengine.h"

HttpRequestModel::HttpRequestModel(
    QString url, QString method, QJsonObject headers, QJsonArray body, QJsonObject validator
):m_url(url), m_method(method), m_headersAsJsonObject(headers), m_body(body), m_validator(validator)
{
    QStringList keys = headers.keys();
    foreach (QString key, keys) {
        m_headers.insert(key, headers.value(key).toString());
    }
}

QString HttpRequestModel::url() const
{
    return m_url;
}

QString HttpRequestModel::method() const
{
    return m_method;
}

QMap<QString, QString> HttpRequestModel::headers() const
{
    return m_headers;
}

QJsonObject HttpRequestModel::headersAsJsonObject() const
{
    return m_headersAsJsonObject;
}

QJsonArray HttpRequestModel::body() const
{
    return m_body;
}

QJsonObject HttpRequestModel::validator() const
{
    return m_validator;
}
