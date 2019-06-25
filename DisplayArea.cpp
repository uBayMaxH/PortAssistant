#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include "DisplayArea.h"
#include "CommonTools.h"


DisplayArea::DisplayArea(QWidget *parent) : QTextEdit (parent)
{
    m_codeConverter = QTextCodec::codecForName("GBK");//在windows下用GBK
    m_displayTime = false;          //不显示时间
    m_pauseDisplay = false;         //不暂停显示
    m_hexDisplay = false;            //不以16进制显示
    m_storeStatus = false;          //不保存
    m_firstFrame = true;            //初始化为是收到的第一帧数据
    ReceivingCntInit();
    //setReadOnly(false);
}

DisplayArea::~DisplayArea()
{
    if (m_storeStatus)
    {
        m_storeFile->close();
        m_storeStatus = false;
    }
}

void DisplayArea::DisplayDatas(const QByteArray &data)
{
    QString outString = "";

    if (m_hexDisplay)
    {
        //以16进制方式显示，需要转换
        outString = CommonTools::ByteArrayToHexString(data);
    }
    else
    {
        outString = m_codeConverter->toUnicode(data);
    }
    //设置收到数据计数器
    ReceivingCntSettings(data.size());

    if (!m_pauseDisplay)
    {
        Display(outString);
    }

    //如果需要保存数据
    if (m_storeStatus)
    {
        StoreToFile(data);
    }
}

void DisplayArea::Display(QString string)
{
    //显示类容追加在后面并且不换行，此句作用主要是滚动条一直在末尾
    moveCursor(QTextCursor::End);
    if (m_displayTime)
    {
        //显示时间
        static qint64 seconds = QDateTime::currentMSecsSinceEpoch();
        qint64 tmpSec = QDateTime::currentMSecsSinceEpoch();
        if (abs(seconds - tmpSec) > 25)
        {
            m_firstFrame = true;
        }
        if (m_firstFrame)
        {
            append(QString("【%1】<-[] %2").arg(QTime::currentTime().toString("AP:HH:mm:ss:zzz")).arg(string));
            m_firstFrame = false;
        }
        else
        {
            insertPlainText(string);
        }
        seconds = tmpSec;
    }
    else
    {
        //不显示时间
        insertPlainText(string);
    }
}

bool DisplayArea::StoreFileManage(bool manageState)
{
    if(manageState)
    {
        m_storeFileDir = QFileDialog::getSaveFileName(this,tr("数据保存"),".",tr("Text Files(*.txt);;Images Files(*.jpg *.png);;All File(*)"));
        if(m_storeFileDir.isEmpty())
        {
            m_storeStatus = false;
        }
        m_storeFile = new QFile(m_storeFileDir);
        if(m_storeFile != nullptr)
        {
            //text_save = new QTextStream(m_storeFile);
            /*这里text_save必须在后面需要写入的地方new，并且在写完后必须要delete，否则可能出现写入失败的情况*/
            m_storeStatus = true;
        }
        else
        {
            //文件打开失败
            m_storeStatus = false;
        }
    }
    else
    {
        m_storeFile->close();
        m_storeStatus = false;
    }
    return m_storeStatus;
}

void DisplayArea::StoreToFile(const QByteArray &data)
{
//    m_storeText = new QTextStream(m_storeFile);
//    m_storeText->seek(m_storeFile->size());//指针移到文件尾，保证以追加的方式写入
//    *m_storeText<<QString("%1").arg(string);
//    delete m_storeText;

    if(m_storeFile->open(QFile::ReadWrite))
    {
        /*将文件指针移动到末尾*/
        m_storeFile->seek(m_storeFile->size());
        /*将数据写入文件中*/
        m_storeFile->write(data.data(),data.size());
        m_storeFile->close();
    }
}

void DisplayArea::BackgroundColorSettings(QColor color)
{
    QPalette colorPalette = palette();
    colorPalette.setColor(QPalette::Base, color);    //设置背景颜色
    setPalette(colorPalette);
}

void DisplayArea::FontColorSettings(QColor color)
{
    QPalette colorPalette = palette();
    colorPalette.setColor(QPalette::Text, color);    //设置字体颜色
    setPalette(colorPalette);
}

void DisplayArea::FontSettings(QFont font)
{
    setFont(font);
}

void DisplayArea::DisplayTimeSettings(bool displayFlag)
{
    m_displayTime = displayFlag;
}

bool DisplayArea::DisplayTimeGet(void)
{
    return m_displayTime;
}

void DisplayArea::ReceivingCntInit(void)
{
    m_receivingCnt = 0;
}
void DisplayArea::ReceivingCntSettings(int64_t cnt)
{
    m_receivingCnt += cnt;
}

int64_t DisplayArea::ReceivingCntGet(void)
{
    return m_receivingCnt;
}

void DisplayArea::HexDisplaySettings(bool status)
{
    m_hexDisplay = status;
}

bool DisplayArea::HexDisplayGet(void)
{
    return m_hexDisplay;
}

void DisplayArea::PauseDisplaySettings(bool status)
{
    m_pauseDisplay = status;
}

bool DisplayArea::PauseDisplayGet(void)
{
    return m_pauseDisplay;
}

void DisplayArea::HelpInfo(void)
{
    //清空显示区
    Clear();
    append(QString("%1").arg("1、本工具支持串口调试、网口调试、数据转换、编码转换、数据校验、多字符串发送等常用功能"));
    append(QString("%1").arg(""));
    append(QString("%1").arg("2、支持16进制和Ascii两种方式发送和接收数据，支持发送文件和保存接收数据到指定文件，支持显示接收时间"));
    append(QString("%1").arg(""));
    append(QString("%1").arg("3、支持自定义接收区的背景颜色、字体颜色、字体大小"));
    append(QString("%1").arg(""));
    append(QString("%1").arg("4、双击接收或发送计数器可清零，双击接收区可暂停接收"));
    append(QString("%1").arg(""));
    append(QString("%1").arg("5、发送区可通过\">>\"按钮显示或隐藏"));
}

void DisplayArea::Clear(void)
{
    clear();
}

void DisplayArea::PauseDisplay(void)
{
    if (!m_pauseDisplay)
    {
        m_pauseDisplay = true;
    }
}

//void DisplayArea::mousePressEvent(QMouseEvent *e)
//{
//    if (e->button() == Qt::LeftButton)
//    {
//        m_pauseTimer->start(500);
//    }
//}

//void DisplayArea::mouseReleaseEvent(QMouseEvent *e)
//{
//    (void)e;
//    m_pauseTimer->stop();
//    PauseDisplaySettings(false);
//}

void DisplayArea::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //如果是左键双击则可暂停（恢复）显示
        m_pauseDisplay = !m_pauseDisplay;
        PauseDisplaySet(m_pauseDisplay);
    }
}
