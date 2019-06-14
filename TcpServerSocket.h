#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include "SocketBase.h"
#include <QTcpSocket>
#include <QTcpServer>

class TcpServerSocket : public SocketBase
{
    Q_OBJECT

public:
    TcpServerSocket();
    virtual ~TcpServerSocket();

public:
    virtual bool SocketConnect();
    virtual void SocketDisConnect();
    virtual qint64 WriteDatas(const QByteArray &data);

private slots:
    void NewClientConnection();
    void ReadTcpClientData();
    void TcpClientDisconnected();

signals:
    void OppositeAddrChange();

private:
    QTcpServer      *m_socket;

    QList<QTcpSocket *> m_clientSocket;

    QHostAddress    m_oppositeIpLast;      //上一次连接的远程IP
    quint16         m_oppositePortLast;    //上一次连接远程端口
};

#endif // TCPSERVERSOCKET_H
