#include "qnamsingleton.h"

QNetworkAccessManager* QNAMSingleton::m_qnam = Q_NULLPTR;

QNAMSingleton::QNAMSingleton(){    
}

QNAMSingleton::~QNAMSingleton()
{
    delete m_qnam;
    m_qnam=Q_NULLPTR;
}

QNetworkAccessManager* QNAMSingleton::getQNAM()
{
    if(!m_qnam)
        m_qnam=new QNetworkAccessManager;
    return m_qnam;
}
