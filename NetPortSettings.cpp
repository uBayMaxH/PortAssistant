#include "NetPortSettings.h"
#include "ui_NetPortSettings.h"
#include <QHostAddress>
#include <QNetworkInterface>

NetPortSettings::NetPortSettings(QWidget *parent) :
    QWidget(parent),
    m_netSetUi(new Ui::NetPortSettings)
{
    m_netSetUi->setupUi(this);
    //setAttribute(Qt::WA_QuitOnClose, false);
     setWindowModality(Qt::ApplicationModal);

    NetPortTypeSet(NET_PORT_TYPE_UDP);
    LocalIpAddrQuery();

    connect(m_netSetUi->UDPButton, SIGNAL(clicked()), this, SLOT(NetPortTypeChanged()));
    connect(m_netSetUi->TCPCButton, SIGNAL(clicked()), this, SLOT(NetPortTypeChanged()));
    connect(m_netSetUi->TCPSButton, SIGNAL(clicked()), this, SLOT(NetPortTypeChanged()));

    connect(m_netSetUi->ApplyButton, SIGNAL(clicked()), this, SLOT(ApplyBtnClicked()));
}

NetPortSettings::~NetPortSettings()
{
    delete m_netSetUi;
}

void NetPortSettings::NetPortTypeSet(eNetPortTypt portType)
{
    m_netType = portType;
    switch (m_netType)
    {
    case NET_PORT_TYPE_TCPSERVER:
        m_netSetUi->TCPSButton->setChecked(true);
        m_netSetUi->OppositeIp->setDisabled(true);
        m_netSetUi->OppositePort->setDisabled(true);
        break;
    case NET_PORT_TYPE_TCPCLICENT:
        m_netSetUi->TCPCButton->setChecked(true);
        m_netSetUi->OppositeIp->setEnabled(true);
        m_netSetUi->OppositePort->setEnabled(true);
        break;
    case NET_PORT_TYPE_UDP:
        m_netSetUi->UDPButton->setChecked(true);
        m_netSetUi->OppositeIp->setEnabled(true);
        m_netSetUi->OppositePort->setEnabled(true);
        break;
    default:
        m_netSetUi->UDPButton->setChecked(true);
        m_netSetUi->OppositeIp->setEnabled(true);
        m_netSetUi->OppositePort->setEnabled(true);
        break;
    }
}

eNetPortTypt NetPortSettings::NetPortTypeGet()
{
    return m_netType;
}

NetAddr_t NetPortSettings::LocalAddrGet()
{
    return m_localAddr;
}

NetAddr_t NetPortSettings::OppositeAddrGet()
{
    return m_oppositeAddr;
}

void NetPortSettings::LocalIpAddrQuery()
{
    //获取并配置本地IP地址
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int i = 0; i < list.count(); i++)
    {
        QHostAddress addr=list.at(i);
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
        {
            m_netSetUi->LocalIp->addItem(addr.toString());
        }
    }
    m_netSetUi->LocalIp->addItem("255.255.255.255");
    m_netSetUi->LocalIp->addItem("0.0.0.0");
    m_netSetUi->LocalIp->setEditable(true);
    m_netSetUi->LocalPort->setText("8080");

    m_netSetUi->OppositeIp->setEditable(true);
    m_netSetUi->OppositePort->setText("80");
}

void NetPortSettings::NetPortTypeChanged()
{
    eNetPortTypt netPortType = NET_PORT_TYPE_NULL;

    if (m_netSetUi->UDPButton->isChecked())
    {
        netPortType = NET_PORT_TYPE_UDP;
    }
    else if (m_netSetUi->TCPCButton->isChecked())
    {
        netPortType = NET_PORT_TYPE_TCPCLICENT;
    }
    else if (m_netSetUi->TCPSButton->isChecked())
    {
        netPortType = NET_PORT_TYPE_TCPSERVER;
    }

    if (m_netType == netPortType)
    {
        return;
    }
    else
    {
        m_netType = netPortType;
        NetPortTypeSet(m_netType);
    }
}

void NetPortSettings::ApplyBtnClicked()
{
    m_localAddr.ip = m_netSetUi->LocalIp->currentText();
    m_localAddr.port = static_cast<quint16>(m_netSetUi->LocalPort->text().toInt()) ;
    if ((!m_netSetUi->OppositeIp->currentText().isEmpty()) && (m_netSetUi->OppositeIp->currentText() != m_oppositeAddr.ip))
    {
        m_oppositeAddr.ip = m_netSetUi->OppositeIp->currentText();
        m_netSetUi->OppositeIp->addItem(m_oppositeAddr.ip);
    }
    m_oppositeAddr.port = static_cast<quint16>(m_netSetUi->OppositePort->text().toInt());
    NetPortTypeChange();
    hide();
}


