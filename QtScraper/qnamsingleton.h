#ifndef QNAMSINGLETON_H
#define QNAMSINGLETON_H

#include <QNetworkAccessManager>

class QNAMSingleton
{    
public:
    QNAMSingleton();
    virtual ~QNAMSingleton();
    static QNetworkAccessManager* getQNAM();
protected:
    static QNetworkAccessManager* m_qnam;
};

#endif // QNAMSINGLETON_H
