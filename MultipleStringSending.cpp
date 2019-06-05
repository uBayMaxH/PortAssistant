#include "MultipleStringSending.h"
#include "ui_MultipleStringSending.h"
#include "CommonTools.h"

MultipleStringSending::MultipleStringSending(QWidget *parent, SendingArea *sendInstance) :
    QWidget(parent),
    m_mulStrUi(new Ui::MultipleStringSending),
    m_sendInstance(sendInstance)
{
    m_mulStrUi->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    m_codeConverter = QTextCodec::codecForName("GBK");//在windows下用GBK
    m_VBoxLayout = new QVBoxLayout;
    m_mulStrUi->intervalLineEdit->setText("1000");  //默认设置为1000ms
    m_mulStrUi->scrollAreaWidgetContents->setLayout(m_VBoxLayout);
    for (int i = 0; i < SEND_CLAUSES_MAX; i++)
    {
        m_sendEnable[i] = new QCheckBox;
        m_sendLineEdit[i] = new QLineEdit;
        m_HBoxLayout[i] = new QHBoxLayout;
        m_HBoxLayout[i]->addWidget(m_sendEnable[i]);
        m_HBoxLayout[i]->addWidget(m_sendLineEdit[i]);
        m_VBoxLayout->addLayout(m_HBoxLayout[i]);
    }
    m_VBoxLayout->setSpacing(0);    //设置控件间距为0

    m_cycleTimer = new QTimer;

    connect(m_mulStrUi->cycleCheckBox, SIGNAL(clicked(bool)), this, SLOT(CycleCheckBoxClicked(bool)));
    connect(m_mulStrUi->hexCheckBox, SIGNAL(clicked(bool)), this, SLOT(HexCheckBoxClicked(bool)));
    connect(m_mulStrUi->sendButton, SIGNAL(clicked()), this, SLOT(SendButtonClicked()));
    connect(m_cycleTimer,SIGNAL(timeout()),this,SLOT(AutoSendingSlot()));
}

MultipleStringSending::~MultipleStringSending()
{
    delete m_mulStrUi;
}

bool MultipleStringSending::CycleSendStateGet()
{
    return m_cycleSend;
}

void MultipleStringSending::CycleTimerSet(bool state)
{
    m_cycleSend = state;
    if (m_cycleSend)
    {
        m_cycleTimer->start(m_mulStrUi->intervalLineEdit->text().toInt());
    }
    else
    {
        m_cycleTimer->stop();
        m_mulStrUi->cycleCheckBox->setChecked(false);
    }
}

void MultipleStringSending::Send(void)
{
    CYCSTART:
    qint8 sendClauses = m_curSendClauses;
    for (sendClauses = m_curSendClauses; sendClauses < SEND_CLAUSES_MAX; sendClauses++)
    {
        if (m_sendEnable[sendClauses]->isChecked())
        {
            //设置当前发送条目为红色字体，其他为黑色字体
            QPalette palette;
            palette.setColor(QPalette::Text,Qt::black);
            m_sendLineEdit[m_lastSendClauses]->setPalette(palette);
            palette.setColor(QPalette::Text,Qt::red);
            m_sendLineEdit[sendClauses]->setPalette(palette);

            QString sendStr = m_sendLineEdit[sendClauses]->text();
            if (sendStr.length() <= 0)
            {
                //发送数据为空
                break;
            }

            QByteArray sendData;

            if (m_hexSending)//转化为16进制发送
            {
                sendData=CommonTools::HexStringToByteArray(sendStr);
            }
            else
            {
                sendData = m_codeConverter->fromUnicode(sendStr);
                CommonTools::CharacterEscapeProcess(&sendData); //转义处理
            }
            m_sendInstance->SendingCntSettings(sendData.size());
            //发送(发送数据)信号
            m_sendInstance->SendingDatas(sendData);
            m_lastSendClauses = sendClauses;
            break;
        }
    }
    if (sendClauses >= SEND_CLAUSES_MAX)
    {
        m_curSendClauses = 0;
        goto CYCSTART;
    }
    m_curSendClauses = ++sendClauses;
}

void MultipleStringSending::CycleCheckBoxClicked(bool state)
{
    m_cycleSend = state;
    if (!m_cycleSend)
    {
        m_cycleTimer->stop();
    }
}

void MultipleStringSending::HexCheckBoxClicked(bool state)
{
    m_hexSending = state;
}

void MultipleStringSending::SendButtonClicked(void)
{
    if (m_cycleSend)
    {
        m_cycleTimer->start(m_mulStrUi->intervalLineEdit->text().toInt());
    }
    else
    {
        Send();
    }
}

void MultipleStringSending::AutoSendingSlot(void)
{
    Send();
}
