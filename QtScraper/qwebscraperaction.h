#ifndef QWEBSCRAPERACTION_H
#define QWEBSCRAPERACTION_H

#include <QQuickItem>
#include <QObject>
#include <QJsonArray>
#include <QVariantMap>

#include "iqwebscraperresponseparser.h"
#include "qwebscraperresponseparser.h"

class QWebScraperAction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString endpoint READ endpoint WRITE setEndpoint)
    Q_PROPERTY(QString method READ method WRITE setMethod)
    Q_PROPERTY(QVariantMap headers READ headers WRITE setHeaders)
    Q_PROPERTY(QJsonArray scraps READ scraps WRITE setScraps)
    Q_PROPERTY(QJsonArray data READ data WRITE setData)
    Q_PROPERTY(QVariantMap validator READ validator WRITE setValidator)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
public:
    explicit QWebScraperAction(QObject *parent = nullptr);
    virtual ~QWebScraperAction();
    QString endpoint() const;
    void setEndpoint(const QString endpoint);
    QString method() const;
    void setMethod(const QString method);
    QVariantMap headers() const;
    void setHeaders(const QVariantMap headers);
    QJsonArray scraps() const;
    void setScraps(const QJsonArray scraps);
    QJsonArray data() const;
    void setData(const QJsonArray data);
    QVariantMap validator() const;
    void setValidator(const QVariantMap validator);
    void appendParser(IQWebScraperReponseParser *parser);
    void loadScraps();
    QJsonArray parseScraps(QString payload);
    bool valid() const;
    void checkValidator(QString payload);
Q_SIGNALS:
    void validChanged(bool);
private:
    void setValid(bool valid);
private:
    IQWebScraperReponseParser *loadParser(QWebScraperResponseParser::Type, QJsonObject jsonObj);
    QString m_endpoint;
    QString m_method;
    QVariantMap m_headers;
    QJsonArray m_scraps;
    QJsonArray m_data;
    QVariantMap m_validator;
    QList<IQWebScraperReponseParser*> m_parsers;
    bool m_valid;
};

#endif // QWEBSCRAPERACTION_H
