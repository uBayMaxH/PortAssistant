#include "TcpClientSocket.h"

TcpClientSocket::TcpClientSocket()
{
    m_socket = new QTcpSocket(this);
    m_initiativeDisconnect = false;

    connect(m_socket,SIGNAL(connected()),this,SLOT(TcpClientConnectedSlot()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(TcpClientDisconnectedSlot()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(TcpClientSocketReadDtatSlot()));
}

TcpClientSocket::~TcpClientSocket()
{
    delete  m_socket;
}

bool TcpClientSocket::SocketConnect()
{
    //m_socket->bind(m_localPort);  //客户端一般不绑定端口
    m_socket->connectToHost(m_oppositeIp, m_oppositePort);
    return true;
}

void TcpClientSocket::SocketDisConnect()
{
    m_initiativeDisconnect = true;
    if (m_connectState)
    {
        //m_socket->disconnectFromHost();     //不会立即关闭
        //m_socket->abort();                //会立即关闭
        m_socket->close();
    }
}

qint64 TcpClientSocket::WriteDatas(const QByteArray &data)
{
    return m_socket->write(data);
}

void TcpClientSocket::TcpClientConnectedSlot()
{
    m_connectState = true;
    m_localIp = m_socket->localAddress();
    m_localPort = m_socket->localPort();
}

void TcpClientSocket::TcpClientDisconnectedSlot()
{
    m_connectState = false;
    if (!m_initiativeDisconnect)
    {
        Disconnect();
    }
    else
    {
        m_initiativeDisconnect = false;
    }
}

void TcpClientSocket::TcpClientSocketReadDtatSlot()
{
    m_readData = m_socket->readAll();

    ReadDatas();
}
