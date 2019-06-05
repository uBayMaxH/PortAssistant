#ifndef QTEXTEDITEX_H
#define QTEXTEDITEX_H

#include <QTextEdit>

class QTextEditEx : public QTextEdit
{
    Q_OBJECT

public:
    explicit QTextEditEx();

protected:
    void mouseDoubleClickEvent(QMouseEvent *);  //双击


};

#endif // QTEXTEDITEX_H
