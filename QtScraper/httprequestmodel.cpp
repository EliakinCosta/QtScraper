#include "httprequestmodel.h"

#include <QUrlQuery>

#include "qscrapengine.h"

HttpRequestModel::HttpRequestModel(
    QString url, QString method, QVariantMap headers, QJsonArray body, QVariantMap validator
):m_url(url), m_method(method), m_headers(headers), m_body(body), m_validator(validator)
{    
}

QString HttpRequestModel::url() const
{
    return m_url;
}

QString HttpRequestModel::method() const
{
    return m_method;
}

QVariantMap HttpRequestModel::headers() const
{
    return m_headers;
}

QJsonArray HttpRequestModel::body() const
{
    return m_body;
}

QVariantMap HttpRequestModel::validator() const
{
    return m_validator;
}
