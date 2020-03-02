#include "httprequestmodel.h"

#include <QJsonObject>
#include <QUrlQuery>

#include "qscrapengine.h"

HttpRequestModel::HttpRequestModel(
        QScrapEngine *scrapEngine, QString url, QString method, QJsonObject headers, QJsonArray body
):m_url(url), m_method(method), m_headersAsJsonObject(headers)
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
                query.addQueryItem(it.key(), scrapEngine->evaluateStringToContext(it.value().toString()));
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

QJsonObject HttpRequestModel::headersAsJsonObject() const
{
    return m_headersAsJsonObject;
}

QByteArray HttpRequestModel::body() const
{
    return m_body;
}
