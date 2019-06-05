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

private:
    QUdpSocket      *m_socket;

};

#endif // UDPSOCKET_H
