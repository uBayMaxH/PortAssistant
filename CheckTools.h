#ifndef CHECKTOOLS_H
#define CHECKTOOLS_H

#include <QWidget>


namespace Ui {
class CheckTools;
}

class CheckTools : public QWidget
{
    Q_OBJECT

public:
    explicit CheckTools(QWidget *parent = nullptr);
    ~CheckTools();

public:
    void Init();

private:
    QByteArray InputDataGet();

    void crc8TableGet(quint8 value = 0x8C);
    void crc16TableGet(quint16 value = 0x1021);
    void crc32TableGet(quint32 value = 0xEDB88320);
    quint8  Crc8(QByteArray &data);
    quint16 Crc16(QByteArray &data);
    quint32 Crc32(QByteArray &data);

private slots:
    void CuSum8BtnClickedSlot();
    void CuSum16BtnClickedSlot();
    void Xor8BtnClickedSlot();
    void Crc8BtnClickedSlot();
    void Crc16BtnClickedSlot();
    void Crc32BtnClickedSlot();

    void CustomCheckBoxSlot(bool status);

private:
    Ui::CheckTools *m_checkUi;

    bool            m_crcCustom;

    quint8          m_crc8FormulaL;         //简式
    quint16         m_crc16FormulaL;
    quint32         m_crc32FormulaL;

    quint8          m_crc8InitVal;          //初值
    quint16         m_crc16InitVal;
    quint32         m_crc32InitVal;

    quint8          m_crc8Table[256];
    quint16         m_crc16Table[256];
    quint32         m_crc32Table[256];

};

#endif // CHECKTOOLS_H
