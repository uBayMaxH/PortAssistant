#include "SocketBase.h"

SocketBase::SocketBase()
{
    m_connectState = false;

    m_localIp = QHostAddress::Broadcast;
    m_localPort = 8080;
    m_oppositeIp = QHostAddress::Broadcast;
    m_oppositePort = 80;
}

SocketBase::~SocketBase()
{

}


void SocketBase::LocalAddrInfoSet(const QString ip, quint16 port)
{
    m_localIp.setAddress(ip);
    m_localPort = port;
}

void SocketBase::OppositeAddrInfoSet(const QString ip, quint16 port)
{
    m_oppositeIp.setAddress(ip);
    m_oppositePort = port;
}

bool SocketBase::SocketIsValid()
{
    return m_connectState;
}

QString SocketBase::LocalIpGet()
{
    return m_localIp.toString();
}

quint16 SocketBase::LocalPortGet()
{
    return m_localPort;
}

QString SocketBase::OppositeIpGet()
{
    return m_oppositeIp.toString();
}

quint16 SocketBase::OppositePortGet()
{
    return m_oppositePort;
}

QByteArray SocketBase::ReadDatasGet()
{
    return m_readData;
}

