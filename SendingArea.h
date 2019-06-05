#ifndef SENDINGAREA_H
#define SENDINGAREA_H

#include <QFrame>
#include <QTextCodec>
#include <QMouseEvent>
#include <QTimer>
#include <QTextStream>
#include <QProgressBar>

#define SEND_FILE_ONCE_LEN_MAX  255

namespace Ui {
class SendingArea;
}

class SendingArea : public QFrame
{
    Q_OBJECT

public:
    explicit SendingArea(QWidget *parent = nullptr);
    ~SendingArea();

public:
    void ObtainSendingDatas(void);

    void SendingCntInit(void);
    void SendingCntSettings(int64_t cnt);
    int64_t SendingCntGet(void);
    /*设置鼠标按下可移动窗口的区域，在子窗口中必须设置该区域*/
    //void SetAreaMovable(const QRect rt);
    void HexSendingSettings(bool status);
    bool HexSendingGet(void);
    /*发送文件*/
    bool GetFileData(bool status);

    bool SendingFileStatusGet(void);
    void SendingFileTimeSet(bool status);

    bool AutoSendingStatusGet(void);
    void AutoSendingTimerSet(bool status);

signals:
    void SendingDatas(const QByteArray &data);


private slots:
    void ClearButtonClickedSlot();
    void SendButtonClickedSlot();
    void SetTime(void);
    void SendFile(void);
    void AutoSendingClickSlot(bool state);
    void AutoSendingSlot(void);

protected:
    /*鼠标点击*/
    //void mousePressEvent(QMouseEvent *e);
    /*鼠标移动*/
    //void mouseMoveEvent(QMouseEvent *e);
    /*鼠标释放*/
    //void mouseReleaseEvent(QMouseEvent *);
    /*鼠标双击*/
    //void mouseDoubleClickEvent(QMouseEvent *);

private:
    Ui::SendingArea *m_sendingUi;
    QTextCodec      *m_codeConverter;       //用于解决接收和发送中文乱码问题
    QTimer          *m_clock;                 //时间
    QTimer          *m_sendingFileTimer;        //发送文件定时器
    QTimer          *m_autoSendingTimer;        //自动发送定时器

    int64_t         m_sendingCnt;           //发送计数器
    bool            m_hexSending;           //是否16进制发送
    bool            m_autoSending;          //是否自动发送
    int32_t         m_SendingIntervalTime;  //发送间隔

//    QRect           m_areaMovable;          //可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
//    bool            m_mousePressed;         //鼠标按下标志（不分左右键）
//    QPoint          m_pressPos;             //鼠标按下的初始位置
    bool            m_sendFileStatus;           //发送文件标志
    QString         m_sendDir;                 //发送文件的路径
    QFile           *m_sendFile;
//    QTextStream     *m_sendText;
//    QByteArray      m_sendingData;
    qint64          m_sendFileSize;             //待发送文件总长度
    qint64          m_sentFileSize;             //已发送长度
    qint32          m_sendFileFrame;        //待发送文件总的数据帧数
    qint32          m_sentFileFrame;        //已发送的帧数
    qint16          m_offsetFrame;          //如果最后一帧不为SEND_FILE_ONCE_LEN_MAX，则其为最后一帧
    QProgressBar    *m_sendingprogress;     //发送文件进度条
};

#endif // SENDINGAREA_H
