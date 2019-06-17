#include "UdpSocket.h"

UdpSocket::UdpSocket()
{
    m_socket=new QUdpSocket(this);

    m_oppositeIp = QHostAddress::Broadcast;
    m_oppositePort = 80;
}

UdpSocket::~UdpSocket()
{
    delete  m_socket;
}


bool UdpSocket::SocketConnect()
{
    /*1. 创建UDP套接字*/
    //m_udpSocket=new QUdpSocket(this);
    /*2. 绑定IP地址和端口号*/
    m_connectState = m_socket->bind(m_localIp,m_localPort);
    /*3. 关联读信号*/
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(UdpSocketReadDtatSlot()));

    return m_connectState;
}

void UdpSocket::SocketDisConnect()
{
    if (m_connectState)
    {
        m_socket->close();
        disconnect(m_socket,SIGNAL(readyRead()),this,SLOT(UdpSocketReadDtatSlot()));
        m_connectState = false;
    }
}

qint64 UdpSocket::WriteDatas(const QByteArray &data)
{
    return m_socket->writeDatagram(data, m_oppositeIp, m_oppositePort);
}

void UdpSocket::UdpSocketReadDtatSlot()
{
    /*判断有没有收到数据*/
    if(m_socket->hasPendingDatagrams())
    {
        qint64 rxLen = m_socket->pendingDatagramSize();
        m_readData.resize(static_cast<int>(rxLen));
        m_socket->readDatagram(m_readData.data(),rxLen,&m_oppositeIp,&m_oppositePort);
        ReadDatas();
        if ((m_oppositeIpLast != m_oppositeIp) || (m_oppositePortLast != m_oppositePort))
        {
            //用于实时显示udp连接状态
            m_oppositeIpLast = m_oppositeIp;
            m_oppositePortLast = m_oppositePort;
            OppositeAddrChange();
        }
    }
}
