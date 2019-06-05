#ifndef QLABELEX_H
#define QLABELEX_H


#include <QLabel>

class QLabelEx : public QLabel
{
    Q_OBJECT
public:
    explicit QLabelEx(QWidget *parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);  //抬起
    void mousePressEvent(QMouseEvent *ev);    //按下
    void mouseDoubleClickEvent(QMouseEvent *ev);  //双击
    void mouseMoveEvent(QMouseEvent *ev);     //拖动

signals:
    void clicked(QMouseEvent *ev);  //抬起
    void pressed(QMouseEvent *ev);  //按下
    void doubled(QMouseEvent *ev);  //双击
    void moved(QMouseEvent *ev);    //拖动
};

#endif // QLABELEX_H
