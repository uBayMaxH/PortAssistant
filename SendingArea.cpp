#include "SendingArea.h"
#include "ui_SendingArea.h"
#include "CommonTools.h"
#include <QFileDialog>
#include <QMessageBox>

SendingArea::SendingArea(QJsonObject obj, QWidget *parent) :
    QFrame(parent),
    m_sendingUi(new Ui::SendingArea),
    m_clock(new QTimer),
    m_sendingFileTimer(new QTimer),
    m_autoSendingTimer(new QTimer)
{
    m_hexSending = false;
    m_autoSending = false;          //是否自动发送
    m_SendingIntervalTime = 0;          //发送间隔
    m_sendFileStatus = false;           //发送文件标志
    SendingCntInit();
    //m_areaMovable = geometry();
    //m_mousePressed = false;
    m_codeConverter = QTextCodec::codecForName("GBK");//在windows下用GBK
    this->setWindowFlags(Qt::CustomizeWindowHint);      //标题栏也没有 按钮也没有
    //this->setWindowFlags(Qt::WindowCloseButtonHint);        //有关闭按钮
    //this->setWindowFlags(Qt::FramelessWindowHint);      //设置窗体无边框，不可拖动拖拽拉伸
    //QPoint movePoint(100,300);
    //move(movePoint);

    m_jsonObject = obj;

    m_sendingUi->setupUi(this);
    setLayout(m_sendingUi->sendLayout);
    m_sendingUi->sendLayout->setMargin(0);   //设置边距为0，即没有边距
    m_sendingUi->sendLayout->setSpacing(0);

    m_sendingUi->showLayout->setMargin(0);   //设置边距为0，即没有边距
    m_sendingUi->showLayout->setSpacing(0);

    m_sendingUi->intervalTime->setText("1000");     //默认设置间隔时间为1000ms
    //显示日期
    QDate dateNow=QDate::currentDate();
    m_sendingUi->dayLabel->setText(QString("  日期:%1").arg(dateNow.toString("yy年MM月dd日 dddd")));

    m_sendingprogress = new QProgressBar(m_sendingUi->sendingTextEdit);
    m_sendingprogress->setGeometry(0,0,0,0);//这里起到一个影藏的效果
    m_sendingprogress->setValue(0);

    m_clock->start(1000);
    connect(m_clock,SIGNAL(timeout()),this,SLOT(SetTime()));
    connect(m_sendingFileTimer,SIGNAL(timeout()),this,SLOT(SendFile()));

    connect(m_sendingUi->sendButton, SIGNAL(clicked()), this, SLOT(SendButtonClickedSlot()));
    connect(m_sendingUi->clearButton, SIGNAL(clicked()), this, SLOT(ClearButtonClickedSlot()));
    connect(m_sendingUi->autoCheckBox, SIGNAL(clicked(bool)), this, SLOT(AutoSendingClickSlot(bool)));

    connect(m_autoSendingTimer,SIGNAL(timeout()),this,SLOT(AutoSendingSlot()));

    //恢复关闭前json保存的发送内容
    RecoverLastData();
}

SendingArea::~SendingArea()
{
    delete m_sendingUi;
}

void SendingArea::ObtainSendingDatas(void)
{
    //获取要发送的内容
    QString sendStr=m_sendingUi->sendingTextEdit->toPlainText();
    if (sendStr.length() <= 0)
    {
        //发送数据为空
        m_sendingUi->sendingTextEdit->setFocus();
        return;
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

    m_sendingCnt += sendData.size();
    //发送(发送数据)信号
    SendingDatas(sendData);
}

void SendingArea::SendingCntInit(void)
{
    m_sendingCnt = 0;
}

void SendingArea::SendingCntSettings(int64_t cnt)
{
    m_sendingCnt += cnt;
}

int64_t SendingArea::SendingCntGet(void)
{
    return m_sendingCnt;
}

bool SendingArea::SendingFileStatusGet(void)
{
    return m_sendFileStatus;
}

void SendingArea::SendingFileTimeSet(bool status)
{
    if (status)
    {
        m_sendingFileTimer->start(20);
        m_sendingUi->sendButton->setText("停止");
    }
    else
    {
        m_sendingFileTimer->stop();
        m_sendingUi->sendButton->setText("发送");
    }
}
bool SendingArea::AutoSendingStatusGet(void)
{
    return m_autoSending;
}

void SendingArea::AutoSendingTimerSet(bool status)
{
    if (status)
    {
        m_autoSendingTimer->start(20);
    }
    else
    {
        m_autoSendingTimer->stop();
        m_sendingUi->autoCheckBox->setChecked(false);
    }
}

QTextEdit* SendingArea::SendTextEditGet(void)
{
    return m_sendingUi->sendingTextEdit;
}

void SendingArea::RecoverLastData(void)
{
    if (JsonOperate::JsonContains(m_jsonObject, "SEND_STR"))
    {
        QJsonValue value = m_jsonObject.value("SEND_STR");
        if (value.isString())
        {
            if (!value.toString().isNull())
            {
                m_sendDataLast = value.toString();
                m_sendingUi->sendingTextEdit->clear();
                m_sendingUi->sendingTextEdit->setText(m_sendDataLast);
            }
        }
    }
}

void SendingArea::SendButtonClickedSlot()
{
    if (!m_sendFileStatus)
    {
        if (m_autoSending)
        {
            //自动发送，点击一下发送，点击第二下停止
            if (m_sendingUi->sendButton->text() == "发送")
            {
                m_SendingIntervalTime = m_sendingUi->intervalTime->text().toInt();
                m_autoSendingTimer->start(m_SendingIntervalTime);
                m_sendingUi->sendButton->setText("停止");
            }
            else
            {
                m_autoSendingTimer->stop();
                m_sendingUi->sendButton->setText("发送");
            }
        }
        else
        {
            ObtainSendingDatas();
        }
    }
    else
    {
        if (m_sendingUi->sendButton->text() == "发送")
        {
            m_sendingUi->sendingTextEdit->append(QString("【%1】[]-> %2").arg(QTime::currentTime().toString("AP:HH:mm:ss:zzz")).arg("start sending......"));
            m_sendingprogress->setGeometry(0,50,m_sendingUi->sendingTextEdit->width(),20);
            m_sendingprogress->setRange(0, m_sendFileFrame);    //***当文件过大的时候可能有误
            m_sendingFileTimer->start(20);
            m_sendingUi->sendButton->setText("停止");
        }
        else
        {
            m_sendingFileTimer->stop();
            m_sendingUi->sendingTextEdit->append(QString("【%1】[]-> %2").arg(QTime::currentTime().toString("AP:HH:mm:ss:zzz")).arg("stop sending......"));
            m_sendingUi->sendButton->setText("发送");
        }
    }

}

void SendingArea::ClearButtonClickedSlot()
{
    m_sendingUi->sendingTextEdit->clear();
}

//设置鼠标按下的区域
//void SendingArea::SetAreaMovable(const QRect rt)
//{
//    if(m_areaMovable != rt)
//    {
//        m_areaMovable = rt;
//    }
//}

void SendingArea::HexSendingSettings(bool status)
{
    m_hexSending = status;

    if(m_hexSending)
    {
        QString str = m_sendingUi->sendingTextEdit->toPlainText();
        m_sendingUi->sendingTextEdit->clear();
        QByteArray data = m_codeConverter->fromUnicode(str);
        CommonTools::CharacterEscapeProcess(&data);
        QString tempstr = CommonTools::ByteArrayToHexString(data);
        m_sendingUi->sendingTextEdit->append(tempstr);
    }
    else
    {

        QByteArray tmpArray = CommonTools::HexStringToByteArray(m_sendingUi->sendingTextEdit->toPlainText());
        m_sendingUi->sendingTextEdit->clear();
        QString tempstr = m_codeConverter->toUnicode(CommonTools::HexEscapeProcess(&tmpArray));
        m_sendingUi->sendingTextEdit->append(tempstr);
    }
}

bool SendingArea::HexSendingGet(void)
{
    return m_hexSending;
}

bool SendingArea::GetFileData(bool status)
{
    m_sendFileStatus = status;
    if(m_sendFileStatus)
    {
        m_sendDir = QFileDialog::getOpenFileName(this,tr("打开文件"),".",tr("All File(*);;Text Files(*.txt);;Images Files(*.jpg *.png)"));
        if(m_sendDir.isEmpty())
        {
            m_sendFileStatus = false;
            return m_sendFileStatus;
        }

//        m_sendFile = new QFile(m_sendDir);
//        if(m_sendFile->open(QIODevice::ReadOnly))
//        {
//            m_sendText = new QTextStream(m_sendFile);
//            QString sendingStr = m_sendText->readAll();
//            m_sendingData = m_codeConverter->fromUnicode(sendingStr);
//            //sendingData = sendingStr.toLatin1().toHex();
//            delete m_sendText;

//            m_sendingUi->sendingTextEdit->clear();
//            m_sendingUi->sendingTextEdit->append(QString("%1").arg("Sending File:"));
//            m_sendingUi->sendingTextEdit->append(QString("%1").arg(m_sendDir + "......"));
//            m_sendingUi->sendingTextEdit->setEnabled(false);
//            m_sendingUi->clearButton->setEnabled(false);
//        }
        m_sendFile = new QFile(m_sendDir);
        m_sendFile->setFileName(m_sendDir);
        if (!m_sendFile->open(QFile::ReadOnly))
        {
            QMessageBox::warning(this, tr("提示"), tr("无法读取文件 %1:\n%2.")
                                 .arg(m_sendDir)
                                 .arg(m_sendFile->errorString()));
            m_sendFileStatus = false;
            return m_sendFileStatus;
        }
        m_sendingUi->sendingTextEdit->clear();
        m_sendingUi->sendingTextEdit->append(QString("%1").arg("Sending File:"));
        m_sendingUi->sendingTextEdit->append(QString("%1").arg(m_sendDir + "......"));
        m_sendingUi->sendingTextEdit->setEnabled(false);
        m_sendingUi->clearButton->setEnabled(false);

        m_sendFileSize = m_sendFile->size();
        m_offsetFrame = m_sendFileSize % SEND_FILE_ONCE_LEN_MAX;
        if (m_offsetFrame)
        {
            m_sendFileFrame = ((m_sendFileSize / SEND_FILE_ONCE_LEN_MAX) & 0xffffff) + 1;
        }
        else
        {
            m_sendFileFrame = (m_sendFileSize / SEND_FILE_ONCE_LEN_MAX) & 0xffffff;
        }
        m_sentFileFrame = 0;
        m_sentFileSize = 0;
    }
    else
    {
        m_sendingUi->sendingTextEdit->setEnabled(true);
        m_sendingUi->clearButton->setEnabled(true);
        m_sendingUi->sendingTextEdit->clear();
        if (m_sendFile->isOpen())
        {
            m_sendFile->close();
        }
    }
    return m_sendFileStatus;
}

//void SendingArea::mousePressEvent(QMouseEvent *e)
//{
//    //鼠标左键
//    if(e->button() == Qt::LeftButton)
//    {
//        m_pressPos = e->pos();
//        m_mousePressed = m_areaMovable.contains(m_pressPos);
//    }
//}

//void SendingArea::mouseMoveEvent(QMouseEvent *e)
//{
//    if(m_mousePressed)
//    {
//        move(pos() + e->pos() - m_pressPos);
//    }
//}

//void SendingArea::mouseReleaseEvent(QMouseEvent *)
//{
//    m_mousePressed = false;
//}

//void SendingArea::mouseDoubleClickEvent(QMouseEvent *)
//{
//    //hide();
//}
void SendingArea::SetTime()
{
    QTime timeNow=QTime::currentTime();
    m_sendingUi->timeLabel->setText(QString(" %1").arg(timeNow.toString()));
}

static QByteArray sendFrame;
void SendingArea::  SendFile(void)
{
//    static int  sendingPos = 0;
//    int strSize = m_sendingData.size();
//    QByteArray tempData;
//    if(strSize - sendingPos >= SEND_FILE_ONCE_LEN_MAX)
//    {
//        tempData = m_sendingData.mid(sendingPos,SEND_FILE_ONCE_LEN_MAX);
//        sendingPos += SEND_FILE_ONCE_LEN_MAX;
//    }
//    else
//    {
//        tempData = m_sendingData.mid(sendingPos,strSize - sendingPos);
//        sendingPos = strSize;
//    }
//    m_sendingprogress->setValue(sendingPos);
//    int size = tempData.size();
//    m_sendingCnt += size;

//    SendingDatas(tempData);
//    if(sendingPos == strSize)
//    {
//        sendingPos = 0;//sendingPos = 0，说明已经发送完一个文件了
//        QMessageBox::information(this,tr("恭喜"),tr("文件发送完毕！"));
//        m_sendingprogress->setGeometry(0,0,0,0);//这里起到一个影藏的效果
//        m_sendingprogress->setValue(0);
//        m_sendFile->close();
//        m_sendingFileTimer->stop();
//        m_sendingUi->sendingTextEdit->append(QString("【%1】[]-> %2").arg(QTime::currentTime().toString("AP:HH:mm:ss:zzz")).arg("File sending completed!"));

//        m_sendingUi->sendButton->setText("发送");
//    }
    qint16  curReadCount = 0;
    if (m_sentFileFrame < m_sendFileFrame)
    {
        m_sentFileFrame++;
        if ((m_offsetFrame != 0) && (m_sentFileFrame == m_sendFileFrame))
        {
            curReadCount = m_offsetFrame;
        }
        else
        {
            curReadCount = SEND_FILE_ONCE_LEN_MAX;
        }
        sendFrame.resize(0);
        sendFrame = m_sendFile->read(curReadCount);
        m_sentFileSize += curReadCount;
        m_sendingCnt += curReadCount;
        m_sendFile->seek(m_sentFileSize);

        SendingDatas(sendFrame);
        m_sendingprogress->setValue(m_sentFileFrame);
    }
    else
    {
        QMessageBox::information(this, tr("恭喜"), tr("文件发送完成"));
        m_sendingprogress->setGeometry(0,0,0,0);//这里起到一个影藏的效果
        m_sendingprogress->setValue(0);
        m_sentFileFrame = 0;
        m_sentFileSize = 0;
        //m_sendFile->close();
        m_sendingFileTimer->stop();
        m_sendingUi->sendingTextEdit->append(QString("【%1】[]-> %2").arg(QTime::currentTime().toString("AP:HH:mm:ss:zzz")).arg("File sending completed!"));

        m_sendingUi->sendButton->setText("发送");
    }
}

void SendingArea::AutoSendingClickSlot(bool state)
{
    if (state)
    {
        m_autoSending = true;
    }
    else
    {
        m_autoSending = false;
        m_autoSendingTimer->stop();
        if (m_sendingUi->sendButton->text() == "停止")
        {
            m_sendingUi->sendButton->setText("发送");
        }
    }
}

void SendingArea::AutoSendingSlot(void)
{
    ObtainSendingDatas();
}
