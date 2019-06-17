#include "TcpServerSocket.h"

TcpServerSocket::TcpServerSocket()
{
    m_socket = new QTcpServer(this);
    m_clientSocket.clear();

    m_oppositeIp = QHostAddress::Broadcast;
    m_oppositePort = 80;
}

TcpServerSocket::~TcpServerSocket()
{
    delete m_socket;
}

bool TcpServerSocket::SocketConnect()
{
    //监听
    m_socket->listen(m_localIp, m_localPort);
    //关联连接信号，检测是否有新的客户端连接
    connect(m_socket,SIGNAL(newConnection()),this,SLOT(NewClientConnection()));
    m_connectState = m_socket->isListening();
    return m_connectState;
}

void TcpServerSocket::SocketDisConnect()
{
    for (int i = 0; i < m_clientSocket.count(); i++)
    {
        m_clientSocket.at(i)->close();
        m_clientSocket.removeAt(i);
    }
    m_socket->close();
    disconnect(m_socket,SIGNAL(newConnection()),this,SLOT(NewClientConnection()));
    m_connectState = false;
}

qint64 TcpServerSocket::WriteDatas(const QByteArray &data)
{
    if (m_clientSocket.count() <= 0)
    {
        //QMessageBox::warning(this,"发送错误","没有连接的客户端!",QMessageBox::Ok);
        return -1;
    }
    qint64 sendLen = 0;
    for(int i=0; i < m_clientSocket.count(); i++)
    {
        //取出地址列表中的一个客户端地址
        QTcpSocket *item = m_clientSocket[i];
        sendLen = item->write(data);
    }
    return sendLen;
}

void TcpServerSocket::NewClientConnection()
{
    /*获取新客户端的套接字信息*/
    QTcpSocket *clientSocket=m_socket->nextPendingConnection();

    if (m_clientSocket.count() <= 10)
    {
        m_oppositeIp = clientSocket->peerAddress();
        m_oppositePort = clientSocket->peerPort();
        if ((m_oppositeIpLast != m_oppositeIp) || (m_oppositePortLast != m_oppositePort))
        {
            m_oppositeIpLast = m_oppositeIp;
            m_oppositePortLast = m_oppositePort;
            OppositeAddrChange();
        }
        m_clientSocket.append(clientSocket);
    }
    /*关联可读信号*/
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(ReadTcpClientData()));
    /*关联断开信号*/
    connect(clientSocket,SIGNAL(disconnected()),this,SLOT(TcpClientDisconnected()));
}

void TcpServerSocket::ReadTcpClientData()
{
    for (int i = 0; i < m_clientSocket.size(); i++)
    {
        m_readData = m_clientSocket.at(i)->readAll();
        if (m_readData.size() > 0)
        {
            ReadDatas();
        }
    }
}

void TcpServerSocket::TcpClientDisconnected()
{
    for (int i = 0; i < m_clientSocket.size(); i++)
    {
        //判断这个客户端是否断开
        if (m_clientSocket.at(i)->socketDescriptor() == -1)
        {
            disconnect(m_clientSocket.at(i),SIGNAL(readyRead()),this,SLOT(ReadTcpClientData()));
            disconnect(m_clientSocket.at(i),SIGNAL(disconnected()),this,SLOT(TcpClientDisconnected()));
            m_clientSocket.removeAt(i);
        }
    }
}
