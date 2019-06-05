/**
 * Copyright (c) 2019
 * All rights reserved.
 *
 * 文件名称：CommonTools.h
 * 文件标识：
 * 摘     要：共用的辅助工具类
 *
 * 现在版本：1.0
 * 作   者 ：uBayMaxH
 * 完成日期：2019年04月12日
 */

#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

/*****************************/
#include <QDesktopWidget>
#include <QApplication>
/****************************/
#include <QTime>
#include <QDateTime>
/****************************/


class CommonTools : public QObject
{
public:
    //窗体居中显示
    static void windowCentered(QWidget *win)
    {
        int screenX=qApp->desktop()->width();
        int screenY=qApp->desktop()->height()-60;
        int wndX=win->width();
        int wndY=win->height();
        QPoint movePoint((screenX-wndX)/2,(screenY-wndY)/2);
        win->move(movePoint);
    }

    //延时ms
    static void sleep(int msec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);//处理未处理的事件
    }

    //字节数组转16进制字符串，用于16进制显示
    static QString ByteArrayToHexString(QByteArray data)
    {
        QString outStr="";
        QString hexStr=data.toHex();
        //加空格
        for (int i=0;i<hexStr.length();i=i+2)
        {
            outStr +=hexStr.mid(i,2)+" ";
        }
        //trimmed():删除字符串开头和末尾的空格
        //toUpper():将字符串转换成大写
        //return outStr.trimmed().toUpper();
        return outStr.toUpper();
    }

    //16进制字符串转字节数组
    static QByteArray HexStringToByteArray(QString str)
    {
        QByteArray senddata = str.toLatin1();
        QByteArray  tempData;
        int         count = 0;
        int         len = senddata.size();
        char hstr = 0,lstr = 0;
        bool hFlag = false;
        bool lFlag = false;
        for(int i = 0;i < len;)
        {
            //高位
            if(!hFlag)
            {
                if(senddata[i] != ' ')//高位不是空格
                {
                    hstr = convertHexChar(senddata[i]);
                    if(i == (len - 1))//此位是最后一位了
                    {
                        hFlag = true;
                        lFlag = true;
                        lstr = hstr;
                        hstr = 0;
                        i ++;
                    }
                    else
                    {
                        i ++;
                        hFlag = true;
                    }
                }
                else//高位是空格
                {
                    i ++;//这里一直往后面找，直到找到不是空格或者结束为止，这里也起到消除中间或者头部多余的空格的作用
                }
            }
            else//低位
            {
                if(!lFlag)
                {
                    if(senddata[i] == ' ')//如果低位是空格,则高位为0，原来的高位变成低位
                    {
                        lstr = hstr;
                        hstr = 0;
                        i ++;
                    }
                    else//如果低位不为空格
                    {
                        lstr = convertHexChar(senddata[i]);
                        i ++;
                    }
                    lFlag = true;
                }
            }
            if((hFlag)&&(lFlag))
            {
                tempData[count ++] = ((hstr << 4) & 0xf0) + lstr;
                hFlag = false;
                lFlag = false;
            }
        }
        return tempData;
    }

    static char convertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (0);
    }

    /*转义字符处理函数*/
    static void CharacterEscapeProcess(QByteArray *sourceByteArray)
    {
        for (int i = 0; i < sourceByteArray->size(); i++)
        {
            if ((sourceByteArray->at(i) == 0x5c) && (i + 1 <  sourceByteArray->size()))    // '\'
            {
                switch (sourceByteArray->at(i + 1))
                {
                case 'a':
                    (*sourceByteArray)[i] = 0x07;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 'b':
                    (*sourceByteArray)[i] = 0x08;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 'f':
                    (*sourceByteArray)[i] = 0x0c;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 'n':
                    (*sourceByteArray)[i] = 0x0a;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 'r':
                    (*sourceByteArray)[i] = 0x0d;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 't':
                    (*sourceByteArray)[i] = 0x09;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 'v':
                    (*sourceByteArray)[i] = 0x0b;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 0x5c:
                    (*sourceByteArray)[i] = 0x5c;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case '`':
                    (*sourceByteArray)[i] = 0x27;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case '"':
                    (*sourceByteArray)[i] = 0x22;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case '?':
                    (*sourceByteArray)[i] = 0x3f;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case '0':
                    (*sourceByteArray)[i] = 0x00;
                    sourceByteArray->remove(i + 1, 1);
                    break;
                case 'd':   //1到3位八进制数所代表的任意字符
                    break;
                case 'x':   //1到2位十六进制所代表的任意字符
                    break;
                default:
                    break;
                }
            }
        }
    }

};

#endif // COMMONTOOLS_H
