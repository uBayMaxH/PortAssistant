#include "MainInterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainInterface w;
    w.show();

    return a.exec();
}
