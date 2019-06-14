#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "SocketBase.h"
#include <QUdpSocket>

class UdpSocket : public SocketBase
{
    Q_OBJECT

public:
    UdpSocket();
    virtual ~UdpSocket();

public:
    virtual bool SocketConnect();
    virtual void SocketDisConnect();
    virtual qint64 WriteDatas(const QByteArray &data);

private slots:
    void UdpSocketReadDtatSlot();

signals:
    void OppositeAddrChange();

private:
    QUdpSocket      *m_socket;

    QHostAddress    m_oppositeIpLast;      //上一次连接的远程IP
    quint16         m_oppositePortLast;    //上一次连接远程端口

};

#endif // UDPSOCKET_H
