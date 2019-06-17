#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include "SocketBase.h"
#include <QTcpSocket>

class TcpClientSocket : public SocketBase
{
    Q_OBJECT

public:
    TcpClientSocket();
    virtual ~TcpClientSocket();

public:
    virtual bool SocketConnect();
    virtual void SocketDisConnect();
    virtual qint64 WriteDatas(const QByteArray &data);

private slots:
    void TcpClientConnectedSlot();
    void TcpClientDisconnectedSlot();
    void TcpClientSocketReadDtatSlot();

signals:
    void Disconnect();

private:
    QTcpSocket      *m_socket;
    bool            m_initiativeDisconnect;
};

#endif // TCPCLIENTSOCKET_H
