#include "httprequestmodel.h"

#include <QJsonObject>
#include <QUrlQuery>

HttpRequestModel::HttpRequestModel(
        QString url, QString method, QJsonObject headers, QJsonArray body
):m_url(url), m_method(method)
{
    QStringList keys = headers.keys();
    foreach (QString key, keys) {
        m_headers.insert(key, headers.value(key).toString());
    }

    QUrlQuery query;
    for (QJsonArray::const_iterator iter = body.begin(); iter != body.end(); ++iter) {
        if (iter->isObject())
        {
            QJsonObject jsonObj = iter->toObject();
            for (QJsonObject::const_iterator it = jsonObj.begin(); it != jsonObj.end(); it++) {
                query.addQueryItem(it.key(), it.value().toString());
            }
        }
    }
    m_body = query.toString(QUrl::FullyEncoded).toUtf8();
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

QByteArray HttpRequestModel::body() const
{
    return m_body;
}
