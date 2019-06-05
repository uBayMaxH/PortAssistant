#include "QlabelEx.h"

QLabelEx::QLabelEx(QWidget *parent) : QLabel(parent)
{
}

void QLabelEx::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked(ev);
}

void QLabelEx::mousePressEvent(QMouseEvent *ev)
{
    emit pressed(ev);
}

void QLabelEx::mouseDoubleClickEvent(QMouseEvent *ev)
{
    emit doubled(ev);
}

void QLabelEx::mouseMoveEvent(QMouseEvent *ev)
{
    emit moved(ev);
}

