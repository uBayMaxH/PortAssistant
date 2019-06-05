#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include <QTextEdit>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>

class DisplayArea : public QTextEdit
{
    Q_OBJECT

public:
    explicit DisplayArea(QWidget *parent = nullptr);
    ~DisplayArea();

public:
    /*显示*/
    void DisplayDatas(const QByteArray &data);
    void Display(QString string);
    bool StoreFileManage(bool manageState);
    void StoreToFile(const QByteArray &data);

public :
    /*背景颜色设置*/
    void BackgroundColorSettings(QColor color);
    /*字体颜色设置*/
    void FontColorSettings(QColor color);
    /*字体设置*/
    void FontSettings(QFont font);
    /*显示时间的set、get方法*/
    void DisplayTimeSettings(bool displyaFlag);
    bool DisplayTimeGet(void);
    /*接收计数器set、get方法*/
    void ReceivingCntInit(void);
    void ReceivingCntSettings(int64_t cnt);
    int64_t ReceivingCntGet(void);
    void HexDisplaySettings(bool status);
    bool HexDisplayGet(void);
    void PauseDisplaySettings(bool status);
    bool PauseDisplayGet(void);

public slots:
    void Clear(void);
    void PauseDisplay(void);

protected:
    //void mouseReleaseEvent(QMouseEvent *e);  //抬起
    //void mousePressEvent(QMouseEvent *e);    //按下
    void mouseDoubleClickEvent(QMouseEvent *e);//双击

private:
    QTextCodec  *m_codeConverter;       //用于解决接收和发送中文乱码问题
    bool        m_displayTime;          //是否显示时间
    bool        m_pauseDisplay;         //是否暂停显示
    bool        m_hexDisplay;           //是否以16进制方式显示
    bool        m_storeStatus;          //是否保存显示区的内容
    int64_t     m_receivingCnt;         //接收计数器
    bool        m_firstFrame;           //是否是收到的第一帧数据，主要用于显示接收时间

    QString     m_storeFileDir;         //保存文件的路径
    QFile       *m_storeFile;
    QTextStream *m_storeText;
};

#endif // DISPLAYAREA_H
