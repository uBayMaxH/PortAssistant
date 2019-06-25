#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T19:32:02
#
#-------------------------------------------------

QT       += core gui
QT       += widgets serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#exe文件名称
TARGET = PortAssistant
TEMPLATE = app

OBJECTS_DIR  = tmp
MOC_DIR      = tmp

#exe文件图标
RC_FILE = icon.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        main.cpp \
        MainInterface.cpp \
    DisplayArea.cpp \
    SendingArea.cpp \
    SerialSettings.cpp \
    QlabelEx.cpp \
    QStatusButton.cpp \
    MultipleStringSending.cpp \
    ConversionTools.cpp \
    CheckTools.cpp \
    QTextEditEx.cpp \
    NetPortSettings.cpp \
    UdpSocket.cpp \
    TcpClientSocket.cpp \
    TcpServerSocket.cpp \
    SocketBase.cpp

HEADERS += \
        MainInterface.h \
    CommonTools.h \
    DisplayArea.h \
    SendingArea.h \
    SerialSettings.h \
    QlabelEx.h \
    QStatusButton.h \
    MultipleStringSending.h \
    ConversionTools.h \
    CheckTools.h \
    QTextEditEx.h \
    NetPortSettings.h \
    UdpSocket.h \
    TcpClientSocket.h \
    TcpServerSocket.h \
    SocketBase.h

FORMS += \
        MainInterface.ui \
    SendingArea.ui \
    SerialSettings.ui \
    MultipleStringSending.ui \
    ConversionTools.ui \
    CheckTools.ui \
    NetPortSettings.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc


