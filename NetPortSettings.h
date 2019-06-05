#ifndef NETPORTSETTINGS_H
#define NETPORTSETTINGS_H

#include <QWidget>

typedef enum
{
    NET_PORT_TYPE_NULL = 0,
    NET_PORT_TYPE_UDP = 1,
    NET_PORT_TYPE_TCPCLICENT = 2,
    NET_PORT_TYPE_TCPSERVER = 3,
    NET_PORT_TYPE_TOTAL
}eNetPortTypt;

typedef struct
{
    QString ip;
    quint16 port;
}NetAddr_t;

namespace Ui {
class NetPortSettings;
}

class NetPortSettings : public QWidget
{
    Q_OBJECT

public:
    explicit NetPortSettings(QWidget *parent = nullptr);
    ~NetPortSettings();

public:
    void NetPortTypeSet(eNetPortTypt portType);
    eNetPortTypt NetPortTypeGet();
    NetAddr_t LocalAddrGet();
    NetAddr_t OppositeAddrGet();

private:
    void LocalIpAddrQuery();

private slots:
    void NetPortTypeChanged();
    void ApplyBtnClicked();

signals:
    void NetPortTypeChange();

private:
    Ui::NetPortSettings *m_netSetUi;

    eNetPortTypt m_netType;           //当前选择的网络类型

    NetAddr_t   m_localAddr;
    NetAddr_t   m_oppositeAddr;
};

#endif // NETPORTSETTINGS_H
