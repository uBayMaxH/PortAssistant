#include "MainInterface.h"
#include "ui_MainInterface.h"
#include "DisplayArea.h"
#include "CommonTools.h"

#include <QMessageBox>
#include <QColorDialog>
#include <QFontDialog>


MainInterface::MainInterface(QWidget *parent) :
    QWidget(parent),
    m_mainUi(new Ui::MainInterface),
    m_uiLayout(new QVBoxLayout),
    m_displayAndSendLayout(new QVBoxLayout),
    m_displayArea(new DisplayArea),
    m_sendingArea(new SendingArea),
    m_serialSettings(new SerialSettings),
    m_serial(new QSerialPort(this)),
    m_netPortSettings(new NetPortSettings),
    m_udpPort(new UdpSocket),
    m_tcpClientPort(new TcpClientSocket),
    m_tcpServerPort(new TcpServerSocket),
    m_menuBar(new QMenuBar),
    m_toolBar(new QToolBar),
    m_statusBar(new QStatusBar)
{
    m_mainUi->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, true); //设置窗口参数 Qt::WA_QuitOnClose attribute,为true 的所有窗口关闭后程序退出

    MenuInit();
    ToolBarInit();
    DisplayAndSendAreaInit();
    StatusBarInit();

    m_uiLayout->setMargin(0);   //设置边距为0，即没有边距
    m_uiLayout->setSpacing(0);
    setLayout(m_uiLayout);
    m_uiLayout->addWidget(m_menuBar);
    m_uiLayout->addWidget(m_toolBar);
    m_uiLayout->addLayout(m_displayAndSendLayout);
    m_uiLayout->addWidget(m_statusBar);
    /*串口*/
    connect(m_serial, &QSerialPort::readyRead, this, &MainInterface::ReadPortData);
    connect(m_sendingArea, &SendingArea::SendingDatas, this, &MainInterface::WriteData);
    connect(m_serialSettings, &SerialSettings::ApplyClicked,this, &MainInterface::SettingsApplySlot);
    /*网口*/
    connect(m_netPortSettings, SIGNAL(NetPortTypeChange(bool)), this, SLOT(NetPortTypeChanged(bool)));
    connect(m_udpPort, &UdpSocket::ReadDatas, this, &MainInterface::ReadPortData);
    connect(m_udpPort, &UdpSocket::OppositeAddrChange, this, &MainInterface::UDPClientChanged);
    connect(m_tcpClientPort, &TcpClientSocket::ReadDatas, this, &MainInterface::ReadPortData);
    connect(m_tcpServerPort, &TcpServerSocket::ReadDatas, this, &MainInterface::ReadPortData);
    connect(m_tcpServerPort, &TcpServerSocket::OppositeAddrChange, this, &MainInterface::TCPServerOppositeAddrChanged);

    /*检测串口断开，可检测热插拔情况*/
    connect(m_serial,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(SerialMonitor(QSerialPort::SerialPortError)));
    connect(m_tcpClientPort,SIGNAL(Disconnect()),this,SLOT(NetPortMonitor()));
}

MainInterface::~MainInterface()
{
    delete m_mainUi;
}

void MainInterface::MenuInit(void)
{
    //端口选择
    m_menu[MENU_PORT_SELECT] = new QMenu("端口选择(&O)");   //&O表示有Alt + O的快捷键
    m_action[MENU_PORT_SELECT][0] = new QAction("UDP", this);
    m_action[MENU_PORT_SELECT][0]->setCheckable(true);      //设置checkbox
    m_menu[MENU_PORT_SELECT]->addAction(m_action[MENU_PORT_SELECT][0]);
    //m_action[MENU_PORT_SELECT][0]->setEnabled(false);

    m_action[MENU_PORT_SELECT][1] = new QAction("TCP Client", this);
    m_action[MENU_PORT_SELECT][1]->setCheckable(true);
    m_menu[MENU_PORT_SELECT]->addAction(m_action[MENU_PORT_SELECT][1]);
    //m_action[MENU_PORT_SELECT][1]->setEnabled(false);

    m_action[MENU_PORT_SELECT][2] = new QAction("TCP Server", this);
    m_action[MENU_PORT_SELECT][2]->setCheckable(true);
    m_menu[MENU_PORT_SELECT]->addAction(m_action[MENU_PORT_SELECT][2]);
    //m_action[MENU_PORT_SELECT][2]->setEnabled(false);

    m_menu[MENU_PORT_SELECT]->addSeparator();                   //添加分割线

    m_action[MENU_PORT_SELECT][3] = new QAction("串口刷新", this);
    m_menu[MENU_PORT_SELECT]->addAction(m_action[MENU_PORT_SELECT][3]);

    //获取串口列表并显示
    SerialListGet();

    //端口设置
    m_menu[MENU_PORT_SETTINGS] = new QMenu("端口设置(&P)");
    m_action[MENU_PORT_SETTINGS][0] = new QAction("串口设置", this);
    m_menu[MENU_PORT_SETTINGS]->addAction(m_action[MENU_PORT_SETTINGS][0]);

    m_action[MENU_PORT_SETTINGS][1] = new QAction("连接", this);
    m_menu[MENU_PORT_SETTINGS]->addAction(m_action[MENU_PORT_SETTINGS][1]);

    m_action[MENU_PORT_SETTINGS][2] = new QAction("断开", this);
    m_menu[MENU_PORT_SETTINGS]->addAction(m_action[MENU_PORT_SETTINGS][2]);

    //显示设置
    m_menu[MENU_DISPLAY_SETTINGS] = new QMenu("显示设置(&D)");
    m_action[MENU_DISPLAY_SETTINGS][0] = new QAction("背景颜色", this);
    m_menu[MENU_DISPLAY_SETTINGS]->addAction(m_action[MENU_DISPLAY_SETTINGS][0]);

    m_action[MENU_DISPLAY_SETTINGS][1] = new QAction("字体颜色", this);
    m_menu[MENU_DISPLAY_SETTINGS]->addAction(m_action[MENU_DISPLAY_SETTINGS][1]);

    m_action[MENU_DISPLAY_SETTINGS][2] = new QAction("字体大小", this);
    m_menu[MENU_DISPLAY_SETTINGS]->addAction(m_action[MENU_DISPLAY_SETTINGS][2]);

    //接收设置
    m_menu[MENU_RECEIVE_SETTINGS] = new QMenu("接收设置(&R)");
    m_action[MENU_RECEIVE_SETTINGS][0] = new QAction("暂停接收", this);
    m_action[MENU_RECEIVE_SETTINGS][0]->setCheckable(true);      //设置checkbox
    m_menu[MENU_RECEIVE_SETTINGS]->addAction(m_action[MENU_RECEIVE_SETTINGS][0]);

    m_action[MENU_RECEIVE_SETTINGS][1] = new QAction("十六进制接收", this);
    m_action[MENU_RECEIVE_SETTINGS][1]->setCheckable(true);      //设置checkbox
    m_menu[MENU_RECEIVE_SETTINGS]->addAction(m_action[MENU_RECEIVE_SETTINGS][1]);

    m_action[MENU_RECEIVE_SETTINGS][2] = new QAction("显示时间戳", this);
    m_action[MENU_RECEIVE_SETTINGS][2]->setCheckable(true);      //设置checkbox
    m_menu[MENU_RECEIVE_SETTINGS]->addAction(m_action[MENU_RECEIVE_SETTINGS][2]);

    m_action[MENU_RECEIVE_SETTINGS][3] = new QAction("接收数据到文件", this);
    m_action[MENU_RECEIVE_SETTINGS][3]->setCheckable(true);      //设置checkbox
    m_menu[MENU_RECEIVE_SETTINGS]->addAction(m_action[MENU_RECEIVE_SETTINGS][3]);

    //发送设置
    m_menu[MENU_SENDING_SETTINGS] = new QMenu("发送设置(&T)");
    m_action[MENU_SENDING_SETTINGS][0] = new QAction("十六进制发送", this);
    m_action[MENU_SENDING_SETTINGS][0]->setCheckable(true);      //设置checkbox
    m_menu[MENU_SENDING_SETTINGS]->addAction(m_action[MENU_SENDING_SETTINGS][0]);

    m_action[MENU_SENDING_SETTINGS][1] = new QAction("发送文件", this);
    m_action[MENU_SENDING_SETTINGS][1]->setCheckable(true);      //设置checkbox
    m_menu[MENU_SENDING_SETTINGS]->addAction(m_action[MENU_SENDING_SETTINGS][1]);

    //扩展功能
    m_menu[MENU_EXTEND] = new QMenu("扩展(&E)");
    m_action[MENU_EXTEND][0] = new QAction("多字符串发送", this);
    m_menu[MENU_EXTEND]->addAction(m_action[MENU_EXTEND][0]);

    m_action[MENU_EXTEND][1] = new QAction("数据转换工具", this);
    m_menu[MENU_EXTEND]->addAction(m_action[MENU_EXTEND][1]);

    m_action[MENU_EXTEND][2] = new QAction("校验工具", this);
    m_menu[MENU_EXTEND]->addAction(m_action[MENU_EXTEND][2]);

    //帮助
    m_menu[MENU_HELP] = new QMenu("帮助(&H)");
    m_action[MENU_HELP][0] = new QAction("帮助", this);
    m_menu[MENU_HELP]->addAction(m_action[MENU_HELP][0]);

    //m_uiLayout->addWidget(m_menuBar);
    for (int i = 0; i < MENU_TOTAL; i++)
    {
        m_menuBar->addMenu(m_menu[i]);
    }
#ifdef Q_OS_LINUX
    m_menuBar->setFont(QFont("Timer", 8, QFont::Normal));
#endif
    m_menuBar->setMaximumHeight(23);
    connect(m_menuBar,SIGNAL(triggered(QAction*)),this,SLOT(TrigerMenuSlot(QAction*)));
    connect(m_displayArea, SIGNAL(PauseDisplaySet(bool)), this, SLOT(PauseDisplaySlot(bool)));
}

void MainInterface::ToolBarInit(void)
{
    m_toolBar->setIconSize(QSize(18, 18));
    m_toolBar->setMaximumHeight(25);
    m_connect = new QAction(QIcon(":/images/connect.png"), "connect");
    m_disConnect = new QAction(QIcon(":/images/disconnect.png"), "disconnect");
    m_settings = new QAction(QIcon(":/images/settings.png"), "setting");
    m_clear = new QAction(QIcon(":/images/clear.png"), "clear");
    m_fileOpen = new QAction(QIcon(":/images/fileopen.png"), "openfile");
    m_fileSave = new QAction(QIcon(":/images/filesave.png"), "savefile");
    m_toolBar->addAction(m_connect);
    m_toolBar->addAction(m_disConnect);
    m_toolBar->addAction(m_settings);
    m_toolBar->addAction(m_clear);
    m_toolBar->addAction(m_fileOpen);
    m_toolBar->addAction(m_fileSave);

    connect(m_connect, SIGNAL(triggered()), this, SLOT(OpenPort()));
    connect(m_disConnect, SIGNAL(triggered()), this, SLOT(ClosePort()));
    connect(m_clear, SIGNAL(triggered()), m_displayArea, SLOT(Clear()));
    connect(m_settings,SIGNAL(triggered()), this, SLOT(ToolBarSettingsSolt()));
    connect(m_fileOpen,SIGNAL(triggered()), this, SLOT(ToolBarOpenFileSolt()));
    connect(m_fileSave,SIGNAL(triggered()), this, SLOT(ToolBarSaveFileSolt()));
}

void MainInterface::DisplayAndSendAreaInit(void)
{
    m_displayAndSendLayout->addWidget(m_displayArea);
    m_displayAndSendLayout->addWidget(m_sendingArea);
    m_sendingArea->hide();
    m_displayAndSendLayout->setMargin(0);   //设置边距为0，即没有边距
    m_displayAndSendLayout->setSpacing(0);

    //设置窗口最小值
    m_displayArea->setMinimumSize(800,400);
    m_sendingArea->setMinimumSize(800, 100);
    m_sendingArea->setMaximumHeight(100);
}

void MainInterface::StatusBarInit(void)
{
    m_statusBarButton = new QPushButton(">>");
    m_statusBarButton->setFont(QFont("Timer", 7, QFont::Normal));
    m_statusBarButton->setMaximumSize(19,17);
    m_statusBarButton->setMinimumSize(19,17);
    m_statusBar->addWidget(m_statusBarButton);
    connect(m_statusBarButton, SIGNAL(clicked()), this, SLOT(StatusBarButtonClickedSlot()));

    //增加一个占位的label
    QLabel *emptyLabel = new QLabel;
    emptyLabel->setMinimumWidth(8);
    m_statusBar->addWidget(emptyLabel);

    m_connectStatus = new QStatusButton;
    m_statusBar->addWidget(m_connectStatus);

    m_statusBarLabel = new QLabel;
    m_statusBar->addWidget(m_statusBarLabel);
    ShowStatusMessage("Disconnected");

    m_sendCntLabel = new QLabelEx;
    m_sendCntLabel->setText("T: 0");
    m_sendCntLabel->setMinimumWidth(120);
    m_statusBar->addPermanentWidget(m_sendCntLabel);
    m_sendCntLabel->setToolTip("双击清零");     //鼠标悬停提示框
    connect(m_sendCntLabel, &QLabelEx::doubled, this, &MainInterface::SendCntLabelClear);
    m_recvCntLabel = new QLabelEx;
    m_recvCntLabel->setText("R: 0");
    m_recvCntLabel->setMinimumWidth(120);
    m_statusBar->addPermanentWidget(m_recvCntLabel);
    m_recvCntLabel->setToolTip("双击清零");     //鼠标悬停提示框
    connect(m_recvCntLabel, &QLabelEx::doubled, this, &MainInterface::RecvCntLabelClear);

    m_statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));   //不显示边框
    m_statusBar->setSizeGripEnabled(false);     //不显示右边的大小控制点
}

void MainInterface::ShowStatusMessage(const QString &message)
{
    m_statusBarLabel->setText(message);
    switch (m_portType)
    {
    case PORT_TYPE_SERIAL:
        m_serial->isOpen() ? m_connectStatus->setChecked(true) : m_connectStatus->setChecked(false);
        break;
    case PORT_TYPE_UDP:
        m_udpPort->SocketIsValid() ? m_connectStatus->setChecked(true) : m_connectStatus->setChecked(false);
        break;
    case PORT_TYPE_TCPCLIENT:
        m_tcpClientPort->SocketIsValid() ? m_connectStatus->setChecked(true) : m_connectStatus->setChecked(false);
        break;
    case PORT_TYPE_TCPSERVER:
        m_tcpServerPort->SocketIsValid() ? m_connectStatus->setChecked(true) : m_connectStatus->setChecked(false);
        break;
    default:
        break;
    }
}

void MainInterface::SendCntLabelSet(int64_t cnt)
{
    m_sendCntLabel->setText(tr("T: %1").arg(cnt));
}

void MainInterface::RecvCntLabelSet(int64_t cnt)
{
    m_recvCntLabel->setText(tr("R: %1").arg(cnt));
}

void MainInterface::SerialListGet(void)
{
    //先删除现有的串口列表
    for (int i = 4; i < 10; i++)
    {
        if (m_action[MENU_PORT_SELECT][i] != nullptr)
        {
            m_menu[MENU_PORT_SELECT]->removeAction(m_action[MENU_PORT_SELECT][i]);
        }
    }
    //刷新列表
    QStringList portList;
    m_serialSettings->SerialPortInfoGet(&portList);
    if (portList.size() <= 0)
    {
        /*当没有串口的时候，初始化为UDP*/
        m_action[MENU_PORT_SELECT][0]->setChecked(true);
        m_portType = PORT_TYPE_UDP;
    }
    else
    {
        m_portType = PORT_TYPE_SERIAL;
        //先关闭其他端口
       // ClosePort();
        //取消其他选项前面的勾
        for (int i = 0; i < 10; i++)
        {
            if (m_action[MENU_PORT_SELECT][i] != nullptr)
            {
                m_action[MENU_PORT_SELECT][i]->setChecked(false);
            }
        }

        for (int i = 0; i < portList.size(); i++)
        {
            if (4 + i < 10)
            {
                m_action[MENU_PORT_SELECT][4 + i] = new QAction(portList[i], this);
                m_action[MENU_PORT_SELECT][4 + i]->setCheckable(true);
                if (portList[i].indexOf(m_serialSettings->CurrentSerialNameGet()) >= 0)
                {
                    m_action[MENU_PORT_SELECT][4 + i]->setChecked(true);
                }
                m_menu[MENU_PORT_SELECT]->addAction(m_action[MENU_PORT_SELECT][4 + i]);
            }
        }
    }
}

void MainInterface::PortSelect(QString string)
{
    //点击的是自己的情况
    for (int i = 0; i < 10; i++)
    {
        if ((m_action[MENU_PORT_SELECT][i] != nullptr) && (m_action[MENU_PORT_SELECT][i]->text() == string) && (!m_action[MENU_PORT_SELECT][i]->isChecked()))
        {
            m_action[MENU_PORT_SELECT][i]->setChecked(true);
            if (m_portType != PORT_TYPE_SERIAL)
            {
                m_netPortSettings->show();
            }
            return;
        }
    }

    //取消其他选项前面的勾
    for (int i = 0; i < 10; i++)
    {
        if ((m_action[MENU_PORT_SELECT][i] != nullptr) && (m_action[MENU_PORT_SELECT][i]->text() != string))
        {
            m_action[MENU_PORT_SELECT][i]->setChecked(false);
        }
    }

    if (string == "UDP")
    {
        m_portType = PORT_TYPE_UDP;
        m_netPortSettings->NetPortTypeSet(NET_PORT_TYPE_UDP);
        m_netPortSettings->show();
    }
    else if (string == "TCP Client")
    {
        m_portType = PORT_TYPE_TCPCLIENT;
        m_netPortSettings->NetPortTypeSet(NET_PORT_TYPE_TCPCLICENT);
        m_netPortSettings->show();
    }
    else if (string == "TCP Server")
    {
        m_portType = PORT_TYPE_TCPSERVER;
        m_netPortSettings->NetPortTypeSet(NET_PORT_TYPE_TCPSERVER);
        m_netPortSettings->show();
    }
#ifdef Q_OS_WIN
    else if (string.indexOf("COM") >= 0)
    {
        //如果是选的串口则先关闭其他端口
        ClosePort();

        m_portType = PORT_TYPE_SERIAL;
        m_serialSettings->CurrentSerialNameSet(string.mid(string.indexOf("COM"), 4));
    }
#else
    else if (string.indexOf("tty") >= 0)
    {
        //如果是选的串口则先关闭其他端口
        ClosePort();
        m_portType = PORT_TYPE_SERIAL;
        m_serialSettings->CurrentSerialNameSet(string.mid(string.indexOf("tty"), 7));
    }
#endif
}

void MainInterface::TrigerMenuSlot(QAction *action)
{
    if (action->text() == "串口刷新")
    {
        SerialListGet();
    }
#ifdef Q_OS_WIN
    else if ((action->text().indexOf("COM") >= 0) || (action->text().indexOf("UDP") >= 0) || (action->text().indexOf("TCP") >= 0))
    {
        PortSelect(action->text());
    }
#else
    else if ((action->text().indexOf("tty") >= 0) || (action->text().indexOf("UDP") >= 0) || (action->text().indexOf("TCP") >= 0))
    {
        PortSelect(action->text());
    }
#endif
    else if (action->text() == "串口设置")
    {
        m_serialSettings->show();
    }
    else if (action->text() == "连接")
    {
        OpenPort();
    }
    else if (action->text() == "断开")
    {
        ClosePort();
    }
    else if (action->text() == "背景颜色")
    {
        QColor color = QColorDialog::getColor(Qt::black);
        if (color.isValid())
        {
            m_displayArea->BackgroundColorSettings(color);
        }
    }
    else if (action->text() == "字体颜色")
    {
        QColor color = QColorDialog::getColor(Qt::black);
        if (color.isValid())
        {
            m_displayArea->FontColorSettings(color);
        }
    }
    else if (action->text() == "字体大小")
    {
        bool enable = false;
        QFont font = QFontDialog::getFont(&enable,QFont("宋体",20,QFont::Bold),this,"自定义文字属性");
        if (enable)
        {
            //font被设置为选择的字体
            m_displayArea->FontSettings(font);
        }
        else
        {
            //取消了这个对话框，font被设置为了初值，既 "宋体",20,QFont::Bold
            //m_displayArea->FontSettings(font);
        }
    }
    else if (action->text() == "暂停接收")
    {
        m_displayArea->PauseDisplaySettings(action->isChecked());
    }
    else if (action->text() == "十六进制接收")
    {
        m_displayArea->HexDisplaySettings(action->isChecked());
    }
    else if (action->text() == "显示时间戳")
    {
        m_displayArea->DisplayTimeSettings(action->isChecked());
    }
    else if (action->text() == "接收数据到文件")
    {
        if (!m_displayArea->StoreFileManage(action->isChecked()))
        {
            action->setChecked(false);
        }
    }
    else if (action->text() == "十六进制发送")
    {
        m_sendingArea->HexSendingSettings(action->isChecked());
    }
    else if (action->text() == "发送文件")
    {
        if (!m_sendingArea->GetFileData(action->isChecked()))
        {
            action->setChecked(false);
        }
    }
    else if (action->text() == "多字符串发送")
    {
        if (m_mulStrSending == nullptr)
        {
            m_mulStrSending = new MultipleStringSending(nullptr, m_sendingArea);
        }
        m_mulStrSending->show();
    }
    else if (action->text() == "数据转换工具")
    {
        if (m_conversionTool == nullptr)
        {
            m_conversionTool = new ConversionTools;
        }
        m_conversionTool->Init();
        m_conversionTool->show();
    }
    else if (action->text() == "校验工具")
    {
        if (m_checkTool == nullptr)
        {
            m_checkTool = new CheckTools;
        }
        m_checkTool->Init();
        m_checkTool->show();
    }
    else if (action->text() == "帮助")
    {
        m_displayArea->HelpInfo();
    }
}

void MainInterface::StatusBarButtonClickedSlot(void)
{
    if (m_statusBarButton->text() == ">>")
    {
        m_sendingArea->show();
        m_statusBarButton->setText("<<");
    }
    else
    {
        m_sendingArea->hide();
        m_statusBarButton->setText(">>");
    }
}

void MainInterface::SettingsApplySlot(void)
{
    SerialListGet();
}

void MainInterface::OpenPort(void)
{
    switch (m_portType)
    {
    case PORT_TYPE_SERIAL:
        {
            const SerialSettings::Settings p = m_serialSettings->settings();
            m_serial->setPortName(p.name);
            m_serial->setBaudRate(p.baudRate);
            m_serial->setDataBits(p.dataBits);
            m_serial->setParity(p.parity);
            m_serial->setStopBits(p.stopBits);
            m_serial->setFlowControl(p.flowControl);
            if (m_serial->open(QIODevice::ReadWrite))
            {
                ShowStatusMessage(tr("Connected to %1 : %2  %3  %4  %5  %6")
                                  .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                  .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
            }
            else
            {
                QMessageBox::critical(this, tr("Error"), m_serial->errorString());
            }
        }
        break;
    case PORT_TYPE_UDP:
        {
            //获取本机地址
            NetAddr_t addr = m_netPortSettings->LocalAddrGet();
            m_udpPort->LocalAddrInfoSet(addr.ip, addr.port);
            //获取远程地址
            addr = m_netPortSettings->OppositeAddrGet();
            m_udpPort->OppositeAddrInfoSet(addr.ip, addr.port);
            if (m_udpPort->SocketConnect())
            {
                ShowStatusMessage(tr("UDP begin:[local addr %1:%2]-[opposite addr %3:%4]").arg(m_udpPort->LocalIpGet()).arg(m_udpPort->LocalPortGet())
                                  .arg(m_udpPort->OppositeIpGet()).arg(m_udpPort->OppositePortGet()));
            }
            else
            {
                QMessageBox::critical(this, tr("Error"), tr("UDP Connection fail to %1:%2").arg(m_udpPort->OppositeIpGet()).arg(m_udpPort->OppositePortGet()));
            }
        }
        break;
    case PORT_TYPE_TCPCLIENT:
        {
            NetAddr_t addr = m_netPortSettings->OppositeAddrGet();
            m_tcpClientPort->OppositeAddrInfoSet(addr.ip, addr.port);
            m_tcpClientPort->SocketConnect();
            qint8 cnt = 50;        //5s
            while (cnt--)
            {
                CommonTools::sleep(100);
                if (m_tcpClientPort->SocketIsValid())
                {
                    ShowStatusMessage(tr("TcpClient Connect Success:[local addr %1:%2]-[server addr %3:%4]").arg(m_tcpClientPort->LocalIpGet()).arg(m_tcpClientPort->LocalPortGet())
                                      .arg(m_tcpClientPort->OppositeIpGet()).arg(m_tcpClientPort->OppositePortGet()));
                    break;
                }
            }
            if (cnt <= 0)
            {
                QMessageBox::critical(this, tr("Error"), tr("连接失败，请重新连接"));
                m_tcpClientPort->SocketDisConnect();
            }
        }
        break;
    case PORT_TYPE_TCPSERVER:
        {
            NetAddr_t addr = m_netPortSettings->LocalAddrGet();
            m_tcpServerPort->LocalAddrInfoSet(addr.ip, addr.port);
            m_tcpServerPort->SocketConnect();
            if (m_tcpServerPort->SocketIsValid())
            {
                ShowStatusMessage(tr("TcpServer Success:[local addr %1:%2]").arg(m_tcpServerPort->LocalIpGet()).arg(m_tcpServerPort->LocalPortGet()));
            }
            else
            {
                QMessageBox::critical(this, tr("Error"), tr("连接失败，请重新连接"));
                m_tcpServerPort->SocketDisConnect();
            }
        }
        break;
    default:
        break;
    }
}

void MainInterface::ClosePort(void)
{
    if (m_sendingArea->AutoSendingStatusGet())
    {
        //如果定时发送的定时器已经start，则stop
        m_sendingArea->AutoSendingTimerSet(false);
    }

    switch (m_portType)
    {
    case PORT_TYPE_SERIAL:
        if (m_serial->isOpen())
        {
            m_serial->close();
        }
        ShowStatusMessage(tr("Disconnected"));
        break;
    case PORT_TYPE_UDP:
        if (m_udpPort->SocketIsValid())
        {
            m_udpPort->SocketDisConnect();
        }
        ShowStatusMessage(tr("Disconnected"));
        break;
    case PORT_TYPE_TCPCLIENT:
        if (m_tcpClientPort->SocketIsValid())
        {
            m_tcpClientPort->SocketDisConnect();
        }
        ShowStatusMessage(tr("Disconnected"));
        break;
    case PORT_TYPE_TCPSERVER:
        m_tcpServerPort->SocketDisConnect();
        ShowStatusMessage(tr("Disconnected"));
        break;
    default:
        break;
    }
}

void MainInterface::ReadPortData(void)
{
    switch (m_portType)
    {
    case PORT_TYPE_SERIAL:
        {
            m_displayArea->DisplayDatas(m_serial->readAll());
        }
        break;
    case PORT_TYPE_UDP:
        {
            m_displayArea->DisplayDatas(m_udpPort->ReadDatasGet());
        }
        break;
    case PORT_TYPE_TCPCLIENT:
        {
            m_displayArea->DisplayDatas(m_tcpClientPort->ReadDatasGet());
        }
        break;
    case PORT_TYPE_TCPSERVER:
        {
            m_displayArea->DisplayDatas(m_tcpServerPort->ReadDatasGet());
        }
        break;
    default:
        break;
    }
    RecvCntLabelSet(m_displayArea->ReceivingCntGet());
}

void MainInterface::WriteData(const QByteArray &data)
{
    switch (m_portType)
    {
    case PORT_TYPE_SERIAL:
        if (!m_serial->isOpen())
        {
            QMessageBox::critical(this, tr("Error"), "串口还未打开，请先打开串口");
            //把已经加了的发送个数减出去
            m_sendingArea->SendingCntSettings(-data.size());
            if (m_sendingArea->SendingFileStatusGet())
            {
                m_sendingArea->SendingFileTimeSet(false);
            }
            if (m_mulStrSending->CycleSendStateGet())
            {
                m_mulStrSending->CycleSendStateReset();
            }
            return;
        }
        m_serial->write(data);
        break;
    case PORT_TYPE_UDP:
        if (m_udpPort->WriteDatas(data) <= 0)
        {
            QMessageBox::critical(this, tr("Error"), "发送失败");
        }
        break;
    case PORT_TYPE_TCPCLIENT:
        if (m_tcpClientPort->SocketIsValid())
        {
            m_tcpClientPort->WriteDatas(data);
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), "Tcp Client未连接");
        }
        break;
    case PORT_TYPE_TCPSERVER:
        if (m_tcpServerPort->WriteDatas(data) == -1)
        {
            QMessageBox::warning(this,"发送错误","没有连接的客户端!",QMessageBox::Ok);
        }
        break;
    default:
        break;
    }
    SendCntLabelSet(m_sendingArea->SendingCntGet());
}

void MainInterface::SendCntLabelClear(void)
{
    m_sendingArea->SendingCntInit();
    SendCntLabelSet(0);
}

void MainInterface::RecvCntLabelClear(void)
{
    m_displayArea->ReceivingCntInit();
    RecvCntLabelSet(0);
}

void MainInterface::ToolBarSettingsSolt(void)
{
    m_serialSettings->show();
}

void MainInterface::ToolBarOpenFileSolt(void)
{
    if (m_sendingArea->GetFileData(true))
    {
        m_action[MENU_SENDING_SETTINGS][1]->setChecked(true);
    }
}

void MainInterface::ToolBarSaveFileSolt(void)
{
    if (m_displayArea->StoreFileManage(true))
    {
        m_action[MENU_RECEIVE_SETTINGS][3]->setChecked(true);
    }
}

void MainInterface::PauseDisplaySlot(bool state)
{
    m_action[MENU_RECEIVE_SETTINGS][0]->setChecked(state);
}

void MainInterface::NetPortTypeChanged(bool state)
{
    if (state)
    {
        //先关闭端口
        ClosePort();
    }
    m_action[MENU_PORT_SELECT][0]->setChecked(false);
    m_action[MENU_PORT_SELECT][1]->setChecked(false);
    m_action[MENU_PORT_SELECT][2]->setChecked(false);
    switch (m_netPortSettings->NetPortTypeGet())
    {
    case NET_PORT_TYPE_UDP:
        m_portType = PORT_TYPE_UDP;
        m_action[MENU_PORT_SELECT][0]->setChecked(true);
        break;
    case NET_PORT_TYPE_TCPCLICENT:
        m_portType = PORT_TYPE_TCPCLIENT;
        m_action[MENU_PORT_SELECT][1]->setChecked(true);
        break;
    case NET_PORT_TYPE_TCPSERVER:
        m_portType = PORT_TYPE_TCPSERVER;
        m_action[MENU_PORT_SELECT][2]->setChecked(true);
        break;
    default:
        break;
    }
}

void MainInterface::SerialMonitor(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        if (m_sendingArea->AutoSendingStatusGet())
        {
            //如果定时发送的定时器已经start，则stop
            m_sendingArea->AutoSendingTimerSet(false);
        }
        if ((m_mulStrSending != nullptr) && (m_mulStrSending->CycleSendStateGet()))
        {
            m_mulStrSending->CycleTimerSet(false);
        }
        QMessageBox::critical(this, tr("Error"), "串口连接中断，请检查连接情况！");
        ClosePort();
    }
}

void MainInterface::NetPortMonitor()
{
    if (m_sendingArea->AutoSendingStatusGet())
    {
        //如果定时发送的定时器已经start，则stop
        m_sendingArea->AutoSendingTimerSet(false);
    }
    QMessageBox::critical(this, tr("Error"), "连接已断开，请检查连接情况！");
    ClosePort();
}

void MainInterface::UDPClientChanged(void)
{
    ShowStatusMessage(tr("UDP begin:[local addr %1:%2]-[opposite addr %3:%4]").arg(m_udpPort->LocalIpGet()).arg(m_udpPort->LocalPortGet())
                      .arg(m_udpPort->OppositeIpGet()).arg(m_udpPort->OppositePortGet()));
}

void MainInterface::TCPServerOppositeAddrChanged(void)
{
    ShowStatusMessage(tr("TcpServer Success:[local addr %1:%2]-[client addr %3:%4]").arg(m_tcpServerPort->LocalIpGet()).arg(m_tcpServerPort->LocalPortGet())
                      .arg(m_tcpServerPort->OppositeIpGet()).arg(m_tcpServerPort->OppositePortGet()));
}
