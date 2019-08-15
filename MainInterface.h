#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "DisplayArea.h"
#include "SendingArea.h"
#include "SerialSettings.h"
#include "QlabelEx.h"
#include "QStatusButton.h"
#include "MultipleStringSending.h"
#include "ConversionTools.h"
#include "CheckTools.h"
#include "NetPortSettings.h"
#include "UdpSocket.h"
#include "TcpClientSocket.h"
#include "TcpServerSocket.h"
#include "JsonOperate.h"

typedef enum
{
    PORT_TYPE_SERIAL = 0,
    PORT_TYPE_UDP = 1,
    PORT_TYPE_TCPCLIENT = 2,
    PORT_TYPE_TCPSERVER = 3,
    PORT_TYPE_TOTAL
}ePortType_e;

typedef enum
{
    MENU_PORT_SELECT = 0,       //端口选择
    MENU_PORT_SETTINGS,       //端口设置
    MENU_DISPLAY_SETTINGS,      //显示设置
    MENU_RECEIVE_SETTINGS,      //接收设置
    MENU_SENDING_SETTINGS,      //发送设置
    MENU_EXTEND,                //扩展功能
    MENU_HELP,                  //帮助
    MENU_TOTAL
}eMenuType_e;

namespace Ui {
class MainInterface;
}

class MainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();

private:
    /*菜单栏初始化*/
    void MenuInit(void);
    void ToolBarInit(void);
    /*显示发送区初始化*/
    void DisplayAndSendAreaInit(void);
    /*状态栏初始化*/
    void StatusBarInit(void);
    /*状态栏显示*/
    void ShowStatusMessage(const QString &message);
    void SendCntLabelSet(int64_t cnt);
    void RecvCntLabelSet(int64_t cnt);

    void SerialListGet(void);
    void PortSelect(QString string);

private slots:
    void TrigerMenuSlot(QAction *action);
    void StatusBarButtonClickedSlot(void);
    void SettingsApplySlot(void);
    //void act_new(void);

    void OpenPort(void);
    void ClosePort(void);
    void ReadPortData(void);
    void WriteData(const QByteArray &data);

    void SendCntLabelClear(void);
    void RecvCntLabelClear(void);

    void ToolBarSettingsSolt(void);
    void ToolBarOpenFileSolt(void);
    void ToolBarSaveFileSolt(void);

    void PauseDisplaySlot(bool state);

    void NetPortTypeChanged(bool state);

    void SerialMonitor(QSerialPort::SerialPortError error);
    void NetPortMonitor();

    void UDPClientChanged(void);
    void TCPServerOppositeAddrChanged(void);

private:
    Ui::MainInterface *m_mainUi = nullptr;
    ePortType_e     m_portType = PORT_TYPE_SERIAL;
    /*整个界面布局，采用水平布局*/
    QVBoxLayout *m_uiLayout = nullptr;
    QVBoxLayout *m_displayAndSendLayout = nullptr;
    /*显示区*/
    DisplayArea *m_displayArea = nullptr;
    /*发送区*/
    SendingArea *m_sendingArea = nullptr;
    /*串口设置区*/
    SerialSettings *m_serialSettings = nullptr;
    QSerialPort *m_serial = nullptr;
    /*网口*/
    NetPortSettings *m_netPortSettings = nullptr;
    UdpSocket       *m_udpPort;
    TcpClientSocket *m_tcpClientPort;
    TcpServerSocket *m_tcpServerPort;

    /*状态栏*/
    QPushButton *m_statusBarButton = nullptr;
    QLabelEx    *m_sendCntLabel = nullptr;
    QLabelEx    *m_recvCntLabel = nullptr;
    QLabel      *m_statusBarLabel = nullptr;
    QStatusButton *m_connectStatus = nullptr;
    /*菜单栏*/
    QMenuBar *m_menuBar = nullptr;
    /*工具栏*/
    QToolBar *m_toolBar = nullptr;
    QAction     *m_connect = nullptr;
    QAction     *m_disConnect = nullptr;
    QAction     *m_settings = nullptr;
    QAction     *m_clear = nullptr;
    QAction     *m_fileOpen = nullptr;
    QAction     *m_fileSave = nullptr;
    /*状态栏*/
    QStatusBar *m_statusBar = nullptr;
    /*菜单选项*/
    QMenu *m_menu[MENU_TOTAL] = {nullptr};
    /*菜单栏点击事件*/
    QAction *m_action[MENU_TOTAL][10] = {{nullptr}};
    /*多字符串发送类*/
    MultipleStringSending   *m_mulStrSending = nullptr;
    /*数据转换工具类*/
    ConversionTools         *m_conversionTool = nullptr;
    /*校验工具*/
    CheckTools              *m_checkTool = nullptr;

    /*JSON*/
    JsonOperate             *m_json = nullptr;
    QJsonObject             m_jsonObject;
};

#endif // MAININTERFACE_H
