//////为了显示连接状态，重写鼠标动作相关函数
#ifndef QSTATUSBUTTON_H
#define QSTATUSBUTTON_H

#include <QRadioButton>

class QStatusButton : public QRadioButton
{
public:
    QStatusButton();

protected:
    void mouseReleaseEvent(QMouseEvent *);  //抬起
    void mousePressEvent(QMouseEvent *);    //按下
    void mouseDoubleClickEvent(QMouseEvent *);  //双击
    void mouseMoveEvent(QMouseEvent *);     //拖动
};

#endif // QSTATUSBUTTON_H
