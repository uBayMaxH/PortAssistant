#include "TcpClientSocket.h"

TcpClientSocket::TcpClientSocket()
{
    m_socket = new QTcpSocket(this);

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
    m_socket->connectToHost(m_oppositeIp, m_oppositePort);
    return true;
}

void TcpClientSocket::SocketDisConnect()
{
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
}

void TcpClientSocket::TcpClientDisconnectedSlot()
{
    m_connectState = false;
    Disconnect();
}

void TcpClientSocket::TcpClientSocketReadDtatSlot()
{
    m_readData = m_socket->readAll();

    ReadDatas();
}
