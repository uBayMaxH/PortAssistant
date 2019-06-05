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

private:
    QTcpServer      *m_socket;

    QList<QTcpSocket *> m_clientSocket;
};

#endif // TCPSERVERSOCKET_H
