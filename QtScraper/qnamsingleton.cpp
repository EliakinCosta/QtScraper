#include "qnamsingleton.h"

QNetworkAccessManager* QNAMSingleton::m_qnam = Q_NULLPTR;

QNAMSingleton::QNAMSingleton(){
    if(!m_qnam)
        m_qnam=new QNetworkAccessManager;
}

QNAMSingleton::~QNAMSingleton()
{
    delete m_qnam;
    m_qnam=Q_NULLPTR;
}

QNetworkAccessManager* QNAMSingleton::getQNAM()
{
    return m_qnam;
}
