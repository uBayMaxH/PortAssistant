#include "CheckTools.h"
#include "ui_CheckTools.h"

#include "CommonTools.h"

CheckTools::CheckTools(QWidget *parent) :
    QWidget(parent),
    m_checkUi(new Ui::CheckTools)
{
    m_checkUi->setupUi(this);

    m_crcCustom = false;
    m_checkUi->FormulaLineEdit->setDisabled(true);
    m_checkUi->InitValueLineEdit->setDisabled(true);

    m_crc8FormulaL = 0x8C;
    m_crc16FormulaL = 0x1021;
    m_crc32FormulaL = 0xEDB88320;

    m_crc8InitVal = 0x00;
    m_crc16InitVal = 0x0000;
    m_crc32InitVal = 0x00000000;

    connect(m_checkUi->CuSum8Btn, SIGNAL(clicked()), this, SLOT(CuSum8BtnClickedSlot()));
    connect(m_checkUi->CuSum16Btn, SIGNAL(clicked()), this, SLOT(CuSum16BtnClickedSlot()));
    connect(m_checkUi->Xor8Btn, SIGNAL(clicked()), this, SLOT(Xor8BtnClickedSlot()));
    connect(m_checkUi->Crc8Btn, SIGNAL(clicked()), this, SLOT(Crc8BtnClickedSlot()));
    connect(m_checkUi->Crc16Btn, SIGNAL(clicked()), this, SLOT(Crc16BtnClickedSlot()));
    connect(m_checkUi->Crc32Btn, SIGNAL(clicked()), this, SLOT(Crc32BtnClickedSlot()));

    connect(m_checkUi->CustomCheckBox, SIGNAL(clicked(bool)), this, SLOT(CustomCheckBoxSlot(bool)));
}

CheckTools::~CheckTools()
{
    delete m_checkUi;
}

void CheckTools::Init()
{
    m_checkUi->InputTextEdit->clear();
    m_checkUi->OutputTextEdit->clear();
    m_checkUi->CustomCheckBox->setChecked(false);
    //CustomCheckBoxSlot(false);
    m_crcCustom = false;
    m_checkUi->FormulaLineEdit->setEnabled(false);
    m_checkUi->InitValueLineEdit->setEnabled(false);
    m_crc8FormulaL = 0x8C;
    m_crc16FormulaL = 0x1021;
    m_crc32FormulaL = 0xEDB88320;
    m_crc8InitVal = 0x00;
    m_crc16InitVal = 0x0000;
    m_crc32InitVal = 0x00000000;
}

QByteArray CheckTools::InputDataGet()
{
    QString inputStr = m_checkUi->InputTextEdit->toPlainText();

    return CommonTools::HexStringToByteArray(inputStr);
}

void CheckTools::crc8TableGet(quint8 value)
{
    quint8 crc8 = 0;

    for (auto i = 0; i < 256; i++)
    {
        crc8 = i & 0xff;
        for (auto bit = 0; bit < 8; bit++)
        {
            crc8 = (crc8 & 0x01) ? ((crc8 >> 1) ^ value) : (crc8 >> 1);
//            if (crc8 & 0x01)
//            {
//                crc8 = (crc8 >> 1) ^ value;
//            }
//            else
//            {
//                crc8 = crc8 >> 1;
//            }
        }
        m_crc8Table[i] = crc8;
    }
}

#define CRC16_WIDTH 16  //crc16 位宽始终为16
void CheckTools::crc16TableGet(quint16 value)
{
    quint16 crc16 = 0;
    int bit;

    for (auto i = 0; i < 256; i++)
    {
        crc16 = (i << (CRC16_WIDTH - 8)) & 0xffff;
        for (bit = 0; bit < 8; bit++)
        {
            crc16 = (crc16 & (1 << (CRC16_WIDTH - 1))) ? (((crc16 << 1) ^ value) & 0xffff) : ((crc16 << 1) & 0xffff);
//            if (crc16 & (1 << (CRC16_WIDTH - 1)))
//            {
//                crc16 = ((crc16 << 1) ^ value) & 0xffff;
//            }
//            else
//            {
//                crc16 = (crc16 << 1) & 0xffff;
//            }
        }
        m_crc16Table[i] = crc16;
    }
}

void CheckTools::crc32TableGet(quint32 value)
{
    quint32 crc32 = 0;

    for (quint32 i = 0; i < 256; i++)
    {
        crc32 = i;
        for (auto bit = 0; bit < 8; bit++)
        {
            crc32 = (crc32 & 0x00000001) ? ((crc32 >> 1) ^ value) : (crc32 >> 1);
//            if (crc32 & 0x00000001)
//            {
//                crc32 = (crc32 >> 1) ^ value;
//            }
//            else
//            {
//                crc32 = crc32 >> 1;
//            }
        }
        m_crc32Table[i] = crc32;
    }
}

quint8  CheckTools::Crc8(QByteArray &data)
{
    quint8 crc8 = m_crc8InitVal;
    if (m_crcCustom)
    {
        QByteArray arr = CommonTools::HexStringToByteArray(m_checkUi->FormulaLineEdit->text());
        if (arr.size() > 0)
        {
            m_crc8FormulaL = static_cast<quint8>(arr.at(arr.size() - 1));
        }
        QByteArray arrInitVal = CommonTools::HexStringToByteArray(m_checkUi->InitValueLineEdit->text());
        if (arrInitVal.size() > 0)
        {
            m_crc8InitVal = static_cast<quint8>(arrInitVal.at(arrInitVal.size() - 1));
            crc8 = m_crc8InitVal;
        }
    }
    crc8TableGet(m_crc8FormulaL);
    for(auto i = 0; i < data.size(); i++)
    {
        crc8 = m_crc8Table[(crc8 ^ data.at(i)) & 0xff];
    }
    return crc8;
}

quint16  CheckTools::Crc16(QByteArray &data)
{
    quint8 tmp;
    quint16 crc16 = m_crc16InitVal;

    if (m_crcCustom)
    {
        QByteArray arr = CommonTools::HexStringToByteArray(m_checkUi->FormulaLineEdit->text());
        if (arr.size() > 0)
        {
            if (arr.size() == 1)
            {
                m_crc16FormulaL = arr.at(0) & 0x00ff;
            }
            else
            {
                m_crc16FormulaL = ((arr.at(arr.size() - 2) << 8) & 0xff00) | ((arr.at(arr.size() - 1)) & 0x00ff);
            }
        }
        QByteArray arrInitVal = CommonTools::HexStringToByteArray(m_checkUi->InitValueLineEdit->text());
        if (arrInitVal.size() > 0)
        {
            if (arrInitVal.size() == 1)
            {
                m_crc16InitVal = arrInitVal.at(0) & 0x00ff;
            }
            else
            {
                m_crc16InitVal = ((arrInitVal.at(arrInitVal.size() - 2) << 8) & 0xff00) | ((arrInitVal.at(arrInitVal.size() - 1)) & 0x00ff);
            }
            crc16 = m_crc16InitVal;
        }
    }
    crc16TableGet(m_crc16FormulaL);
    for ( auto i = 0; i < data.size(); i++)
    {
        tmp = (((crc16 >> (CRC16_WIDTH - 8)) & 0xff) ^ data.at(i)) & 0xff;
        crc16 = (m_crc16Table[tmp] ^ (crc16 << 8)) & 0xffff;
    }

    return crc16;
}

quint32  CheckTools::Crc32(QByteArray &data)
{
    quint32 crc32 = ~m_crc32InitVal;

    if (m_crcCustom)
    {
        QByteArray arr = CommonTools::HexStringToByteArray(m_checkUi->FormulaLineEdit->text());
        if (arr.size() > 0)
        {
            if (arr.size() == 1)
            {
                m_crc32FormulaL = arr.at(0) & 0x000000ff;
            }
            else if (arr.size() == 2)
            {
                m_crc32FormulaL = ((arr.at(0) << 8) & 0x0000ff00) | ((arr.at(1)) & 0x000000ff);
            }
            else if (arr.size() == 3)
            {
                m_crc32FormulaL = ((arr.at(0) << 16) & 0x00ff0000) | ((arr.at(1) << 8) & 0x0000ff00) | (arr.at(2) & 0x000000ff);
            }
            else
            {
                m_crc32FormulaL = (static_cast<quint32>(arr.at(arr.size() - 4) << 24) & 0xff000000) | ((arr.at(arr.size() - 3) << 16) & 0x00ff0000)
                        | ((arr.at(arr.size() - 2) << 8) & 0x0000ff00) | ((arr.at(arr.size() - 1)) & 0x000000ff);
            }
        }
        QByteArray arrInitVal = CommonTools::HexStringToByteArray(m_checkUi->InitValueLineEdit->text());
        if (arrInitVal.size() > 0)
        {
            if (arrInitVal.size() == 1)
            {
                m_crc32InitVal = arrInitVal.at(0) & 0x000000ff;
            }
            else if (arrInitVal.size() == 2)
            {
                m_crc32InitVal = ((arrInitVal.at(0) << 8) & 0x0000ff00) | ((arrInitVal.at(1)) & 0x000000ff);
            }
            else if (arrInitVal.size() == 3)
            {
                m_crc32InitVal = ((arrInitVal.at(0) << 16) & 0x00ff0000) | ((arrInitVal.at(1) << 8) & 0x0000ff00) | (arrInitVal.at(2) & 0x000000ff);
            }
            else
            {
                m_crc32InitVal = (static_cast<quint32>(arrInitVal.at(arrInitVal.size() - 4) << 24) & 0xff000000) | ((arrInitVal.at(arrInitVal.size() - 3) << 16) & 0x00ff0000)
                        | ((arrInitVal.at(arrInitVal.size() - 2) << 8) & 0x0000ff00) | ((arrInitVal.at(arrInitVal.size() - 1)) & 0x000000ff);
            }
            crc32 = ~m_crc32InitVal;
        }
    }
    crc32TableGet(m_crc32FormulaL);
    for ( auto i = 0; i < data.size(); i++)
    {
        crc32 = m_crc32Table[(crc32 ^ static_cast<quint32>(data.at(i))) & 0xff] ^ (crc32 >> 8);
    }

    return ~crc32;
}

void CheckTools::CuSum8BtnClickedSlot()
{
    QByteArray inputArr = InputDataGet();
    quint8 sum = 0x00;
    for (int i = 0; i < inputArr.size(); i++)
    {
        sum += inputArr.at(i);
    }

    m_checkUi->OutputTextEdit->clear();
    m_checkUi->OutputTextEdit->setText(QString("8位累加和为：%1").arg(sum, 2, 16, QLatin1Char('0')).toUpper());
}

void CheckTools::CuSum16BtnClickedSlot()
{
    QByteArray inputArr = InputDataGet();
    quint16 sum = 0x0000;
    for (int i = 0; i < inputArr.size(); i++)
    {
        sum += (inputArr.at(i) & 0x00ff);
    }
    m_checkUi->OutputTextEdit->clear();
    m_checkUi->OutputTextEdit->setText(QString("16位累加和为：%1").arg(sum, 4, 16, QLatin1Char('0')).toUpper());
}

void CheckTools::Xor8BtnClickedSlot()
{
    QByteArray inputArr = InputDataGet();
    quint8 sum = 0x00;
    for (int i = 0; i < inputArr.size(); i++)
    {
        sum ^= inputArr.at(i);
    }

    m_checkUi->OutputTextEdit->clear();
    m_checkUi->OutputTextEdit->setText(QString("8异或和为：%1").arg(sum, 2, 16, QLatin1Char('0')).toUpper());
}

void CheckTools::Crc8BtnClickedSlot()
{
    QByteArray inputArr = InputDataGet();
    quint8 crc8 = Crc8(inputArr);

    m_checkUi->OutputTextEdit->clear();
    m_checkUi->OutputTextEdit->append(QString("CRC8多项式：%1，初值：%2").arg(m_crc8FormulaL, 2, 16, QLatin1Char('0')).arg(m_crc8InitVal, 2, 16, QLatin1Char('0')).toUpper());
    m_checkUi->OutputTextEdit->append(QString("CRC8的计算结果为：%1").arg(crc8, 2, 16, QLatin1Char('0')).toUpper());
}

void CheckTools::Crc16BtnClickedSlot()
{
    QByteArray inputArr = InputDataGet();
    quint16 crc16 = Crc16(inputArr);

    m_checkUi->OutputTextEdit->clear();
    m_checkUi->OutputTextEdit->append(QString("CRC16多项式：%1，初值：%2").arg(m_crc16FormulaL, 4, 16, QLatin1Char('0')).arg(m_crc16InitVal, 4, 16, QLatin1Char('0')).toUpper());
    m_checkUi->OutputTextEdit->append(QString("CRC16的计算结果为：%1").arg(crc16, 4, 16, QLatin1Char('0')).toUpper());
}

void CheckTools::Crc32BtnClickedSlot()
{
    QByteArray inputArr = InputDataGet();
    quint32 crc32 = Crc32(inputArr);

    m_checkUi->OutputTextEdit->clear();
    m_checkUi->OutputTextEdit->append(QString("CRC32多项式：%1，初值：%2").arg(m_crc32FormulaL, 8, 16, QLatin1Char('0')).arg(m_crc32InitVal, 8, 16, QLatin1Char('0')).toUpper());
    m_checkUi->OutputTextEdit->append(QString("CRC32的计算结果为：%1").arg(crc32, 8, 16, QLatin1Char('0')).toUpper());
}

void CheckTools::CustomCheckBoxSlot(bool status)
{
    m_crcCustom = status;
    if (m_crcCustom)
    {
        m_checkUi->FormulaLineEdit->setEnabled(true);
        m_checkUi->InitValueLineEdit->setEnabled(true);
    }
    else
    {
        m_checkUi->FormulaLineEdit->setDisabled(true);
        m_checkUi->InitValueLineEdit->setDisabled(true);

        m_crc8FormulaL = 0x8C;
        m_crc16FormulaL = 0x1021;
        m_crc32FormulaL = 0xEDB88320;

        m_crc8InitVal = 0x00;
        m_crc16InitVal = 0x0000;
        m_crc32InitVal = 0x00000000;
    }
}
