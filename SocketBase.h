#ifndef SOCKETBASE_H
#define SOCKETBASE_H

#include <QHostInfo>
#include <QtNetwork>
#include <QHostInfo>
#include <QHostAddress>

class SocketBase : public QObject
{
    Q_OBJECT

public:
    SocketBase();
    virtual ~SocketBase();

public:
    void LocalAddrInfoSet(const QString ip, quint16 port);
    void OppositeAddrInfoSet(const QString ip, quint16 port);
    bool SocketIsValid();

    virtual bool SocketConnect() = 0;
    virtual void SocketDisConnect() = 0;
    virtual qint64 WriteDatas(const QByteArray &data) = 0;

    QString LocalIpGet();
    quint16 LocalPortGet();
    QString OppositeIpGet();
    quint16 OppositePortGet();

    QByteArray ReadDatasGet();

signals:
    void ReadDatas();

public:
    QHostAddress    m_localIp;         //本地IP
    quint16         m_localPort;       //本地端口

    QHostAddress    m_oppositeIp;      //远程IP
    quint16         m_oppositePort;    //远程端口

    bool            m_connectState;     //连接状态

    QByteArray      m_readData;
};

#endif // SOCKETBASE_H
