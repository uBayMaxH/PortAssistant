#include "ConversionTools.h"
#include "ui_ConversionTools.h"
#include "CommonTools.h"

#include <QTime>
#include <QMessageBox>

ConversionTools::ConversionTools(QWidget *parent) :
    QWidget(parent),
    m_conversionUi(new Ui::ConversionTools)
{
    m_conversionUi->setupUi(this);

    m_codeConverter = QTextCodec::codecForName("GBK");//在windows下用GBK
    m_utf8Converter = QTextCodec::codecForName("UTF-8");

    m_input1 = new QTextEditEx;
    m_input2 = new QTextEditEx;
    m_output = new QTextEditEx;

    m_conversionUi->input1Layout->addWidget(m_input1);
    m_conversionUi->input2Layout->addWidget(m_input2);
    m_conversionUi->outputLayout->addWidget(m_output);

    m_conversionUi->AscRadioButton->setChecked(true);

    connect(m_conversionUi->CharacterNumBtn, SIGNAL(clicked()), this, SLOT(CharacterNumBtnClickedSlot()));
    connect(m_conversionUi->RandomNumBtn, SIGNAL(clicked()), this, SLOT(RandomNumBtnClickedSlot()));
    connect(m_conversionUi->HexToDecBtn, SIGNAL(clicked()), this, SLOT(HexToDecBtnClickedSlot()));
    connect(m_conversionUi->DecToHexBtn, SIGNAL(clicked()), this, SLOT(DecToHexBtnClickedSlot()));
    connect(m_conversionUi->HexToAscBtn, SIGNAL(clicked()), this, SLOT(HexToAscBtnClickedSlot()));
    connect(m_conversionUi->AscToHexBtn, SIGNAL(clicked()), this, SLOT(AscToHexBtnClickedSlot()));
    connect(m_conversionUi->HexToFloatBtn, SIGNAL(clicked()), this, SLOT(HexToFloatBtnClickedSlot()));
    connect(m_conversionUi->FloatToHexBtn, SIGNAL(clicked()), this, SLOT(FloatToHexBtnClickedSlot()));
    connect(m_conversionUi->BitwiseAndBtn, SIGNAL(clicked()), this, SLOT(BitwiseAndBtnClickedSlot()));
    connect(m_conversionUi->BitwiseOrBtn, SIGNAL(clicked()), this, SLOT(BitwiseOrBtnClickedSlot()));
    connect(m_conversionUi->BitwiseXorBtn, SIGNAL(clicked()), this, SLOT(BitwiseXorBtnClickedSlot()));
    connect(m_conversionUi->BitwiseNotBtn, SIGNAL(clicked()), this, SLOT(BitwiseNotBtnClickedSlot()));
    connect(m_conversionUi->ToUpperBtn, SIGNAL(clicked()), this, SLOT(ToUpperBtnClickedSlot()));
    connect(m_conversionUi->ToLowerBtn, SIGNAL(clicked()), this, SLOT(ToLowerBtnClickedSlot()));
    connect(m_conversionUi->ToOppositeBtn, SIGNAL(clicked()), this, SLOT(ToOppositeBtnClickedSlot()));
    connect(m_conversionUi->InversionBtn, SIGNAL(clicked()), this, SLOT(InversionBtnClickedSlot()));
    connect(m_conversionUi->GbkToChBtn, SIGNAL(clicked()), this, SLOT(GbkToChBtnClickedSlot()));
    connect(m_conversionUi->ChToGbkBtn, SIGNAL(clicked()), this, SLOT(ChToGbkBtnClickedSlot()));
    connect(m_conversionUi->Utf8ToChBtn, SIGNAL(clicked()), this, SLOT(Utf8ToChBtnClickedSlot()));
    connect(m_conversionUi->ChToUtf8Btn, SIGNAL(clicked()), this, SLOT(ChToUtf8BtnClickedSlot()));
    connect(m_conversionUi->GbkToUtf8Btn, SIGNAL(clicked()), this, SLOT(GbkToUtf8BtnClickedSlot()));
    connect(m_conversionUi->Utf8ToGbkBtn, SIGNAL(clicked()), this, SLOT(Utf8ToGbkBtnClickedSlot()));

    connect(m_conversionUi->AscRadioButton, SIGNAL(clicked()), this, SLOT(RadioBtnClickedSlot()));
    connect(m_conversionUi->HexRadioButton, SIGNAL(clicked()), this, SLOT(RadioBtnClickedSlot()));
}

ConversionTools::~ConversionTools()
{
    delete m_conversionUi;
}

void ConversionTools::Init()
{
    m_input1->clear();
    m_input2->clear();
    m_output->clear();
    m_conversionUi->AscRadioButton->setChecked(true);
    m_hexStatus = false;
}

void ConversionTools::CharacterNumBtnClickedSlot()
{
    m_output->clear();
    if (m_hexStatus)
    {
        if (m_input1->toPlainText().size() > 0)
        {
            m_output->append(QString("输入区1的字节数为：%1").arg(CommonTools::HexStringToByteArray(m_input1->toPlainText()).size()));
        }

        if (m_input2->toPlainText().size() > 0)
        {
            m_output->append("");
            m_output->append(QString("输入区2的字节数为：%1").arg(CommonTools::HexStringToByteArray(m_input2->toPlainText()).size()));
        }
    }
    else
    {
        if (m_input1->toPlainText().size() > 0)
        {
            m_output->append(QString("输入区1的字符个数为：%1").arg(m_input1->toPlainText().size()));
        }

        if (m_input2->toPlainText().size() > 0)
        {
            m_output->append("");
            m_output->append(QString("输入区2的字符个数为：%1").arg(m_input2->toPlainText().size()));
        }
    }
}

void ConversionTools::RandomNumBtnClickedSlot()
{
    if(m_input1->toPlainText().size() <= 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("请在输入区1输入一个整数n(整数n表示产生的随机数为n以内的随机数)"));
        return;
    }

    QTime time;
    time= QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()+time.second()*1000));

    QByteArray strArray = m_input1->toPlainText().toLatin1();
    for (int i = 0; i < strArray.size(); i++)
    {
        if ((strArray.at(i) < '0') || (strArray.at(i) > '9'))
        {
            QMessageBox::warning(this, tr("提示"), tr("输入数据格式有误，请重新输入"));
            m_input1->clear();
            return;
        }
    }

    if (strArray.size() > 9)
    {
        QMessageBox::warning(this, tr("提示"), tr("输入数据过大，请重新输入"));
        m_input1->clear();
        return;
    }

    m_output->clear();
    m_output->setText(QString("生成的随机数为：%1").arg(qrand() % m_input1->toPlainText().toInt()));
}

void ConversionTools::HexToDecBtnClickedSlot()
{
    if (!m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Hex"));
        return;
    }

    char decBuf[BUF_MAX] = "";
    char tmpBuf[10] = "";
    if (m_input1->toPlainText().size() > 0)
    {
        QByteArray inputData1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());

        m_output->clear();
        m_output->append("输入区1的10进制格式为：");

        for (int i = 0; i < inputData1.size(); i++)
        {
            memset(tmpBuf, 0x00, 10);
            sprintf(tmpBuf, "%d ", inputData1.at(i));
            strcat(decBuf, tmpBuf);
        }
        m_output->append(QString(QLatin1String(decBuf)));
    }

    if (m_input2->toPlainText().size() > 0)
    {
        QByteArray inputData2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());

        m_output->append("");

        m_output->append("输入区2的10进制格式为：");
        memset(decBuf, 0x00, BUF_MAX);
        for (int i = 0; i < inputData2.size(); i++)
        {
            memset(tmpBuf, 0x00, 10);
            sprintf(tmpBuf, "%d ", inputData2.at(i));
            strcat(decBuf, tmpBuf);
        }
        m_output->append(QString(QLatin1String(decBuf)));
    }

}

void ConversionTools::DecToHexBtnClickedSlot()
{
    if (m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Ascii"));
        return;
    }
    m_output->clear();

    long num = 0;
    char tmpBuf[20] = "";
    if (m_input1->toPlainText().size() > 0)
    {
        num = m_input1->toPlainText().toLong();
        char tmpBuf[20] = "";
        sprintf(tmpBuf, "%lX", num);
        m_output->append(QString("输入区1转换后的结果为：%1").arg(tmpBuf));
    }

    if (m_input2->toPlainText().size() > 0)
    {
        m_output->append("");
        num = m_input2->toPlainText().toLong();
        memset(tmpBuf, 0x00 ,20);
        sprintf(tmpBuf, "%lX", num);
        m_output->append(QString("输入区2转换后的结果为：%1").arg(tmpBuf));
    }
}

void ConversionTools::HexToAscBtnClickedSlot()
{
    if (!m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Hex"));
        return;
    }
    if (m_input1->toPlainText().size() > 0)
    {
        m_output->clear();
        QByteArray tmpArray1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
        QString str1 = m_codeConverter->toUnicode(CommonTools::HexEscapeProcess(&tmpArray1));
        m_output->append("输入区1的ASCII格式输出为：");
        m_output->append(str1);
    }

    if (m_input2->toPlainText().size() > 0)
    {
        m_output->append("");
        QByteArray tmpArray2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());
        QString str2 = m_codeConverter->toUnicode(CommonTools::HexEscapeProcess(&tmpArray2));
        m_output->append("输入区2的ASCII格式输出为：");
        m_output->append(str2);
    }
}

void ConversionTools::AscToHexBtnClickedSlot()
{
    if (m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Ascii"));
        return;
    }

    m_output->clear();
    if (m_input1->toPlainText().size() > 0)
    {
        QByteArray data1 = m_codeConverter->fromUnicode(m_input1->toPlainText());
        CommonTools::CharacterEscapeProcess(&data1);
        QString str1 = CommonTools::ByteArrayToHexString(data1);
        m_output->append("输入区1的16进制格式输出为：");
        m_output->append(str1);
    }

    if (m_input2->toPlainText().size() > 0)
    {
        QByteArray data2 = m_codeConverter->fromUnicode(m_input2->toPlainText());
        CommonTools::CharacterEscapeProcess(&data2);
        QString str2 = CommonTools::ByteArrayToHexString(data2);

        m_output->append("");
        m_output->append("输入区2的16进制格式输出为：");
        m_output->append(str2);
    }
}

typedef  union
{
    char ch[4];
    float fData;
}chToF_t;
void ConversionTools::HexToFloatBtnClickedSlot()
{
    if (!m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Hex"));
        return;
    }
    QByteArray arr1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
    if (arr1.size() % 4 != 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("输入数据个数必须是4的整数倍"));
        return;
    }

    chToF_t chToF;
    chToF.ch[0] = arr1.at(3);
    chToF.ch[1] = arr1.at(2);
    chToF.ch[2] = arr1.at(1);
    chToF.ch[3] = arr1.at(0);
    m_output->clear();
    m_output->append(QString("转换后的结果为：%1").arg(static_cast<double>( chToF.fData)));
}

void ConversionTools::FloatToHexBtnClickedSlot()
{
    if (m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Ascii"));
        return;
    }
    chToF_t chToF;
    chToF.fData = m_input1->toPlainText().toFloat();
    QByteArray arr;
    arr.resize(4);
    arr[0] = chToF.ch[3];
    arr[1] = chToF.ch[2];
    arr[2] = chToF.ch[1];
    arr[3] = chToF.ch[0];
    QString str = CommonTools::ByteArrayToHexString(arr);
    m_output->clear();
    m_output->append(QString("单精度转换后的结果为：%1").arg(str));
    typedef  union
    {
        char ch[8];
        double fData;
    }chToD_t;
    chToD_t chToD;
    chToD.fData = m_input1->toPlainText().toDouble();
    arr.resize(8);
    arr[0] = chToD.ch[7];
    arr[1] = chToD.ch[6];
    arr[2] = chToD.ch[5];
    arr[3] = chToD.ch[4];
    arr[4] = chToD.ch[3];
    arr[5] = chToD.ch[2];
    arr[6] = chToD.ch[1];
    arr[7] = chToD.ch[0];
    str = CommonTools::ByteArrayToHexString(arr);
    m_output->append(QString("双精度转换后的结果为：%1").arg(str));
}

void ConversionTools::BitwiseAndBtnClickedSlot()
{
    QByteArray arr1;
    QByteArray arr2;
    QByteArray arr;
    if (m_hexStatus)
    {
        arr1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
        arr2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());
    }
    else
    {
        arr1 = m_input1->toPlainText().toLatin1();
        arr2 = m_input2->toPlainText().toLatin1();
    }
    int len = (arr1.size() > arr2.size()) ? arr2.size() : arr1.size();
    arr.resize(len);
    for (int i = 0; i < len; ++i)
    {
        arr[i] = arr1[i] & arr2[i];
    }

    m_output->clear();
    m_output->append("两输入区的按位与结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(arr));
}

void ConversionTools::BitwiseOrBtnClickedSlot()
{
    QByteArray arr1;
    QByteArray arr2;
    QByteArray arr;
    if (m_hexStatus)
    {
        arr1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
        arr2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());
    }
    else
    {
        arr1 = m_input1->toPlainText().toLatin1();
        arr2 = m_input2->toPlainText().toLatin1();
    }
    int len = (arr1.size() > arr2.size()) ? arr2.size() : arr1.size();
    arr.resize(len);
    for (int i = 0; i < len; ++i)
    {
        arr[i] = arr1[i] | arr2[i];
    }

    m_output->clear();
    m_output->append("两输入区的按位或结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(arr));
}

void ConversionTools::BitwiseXorBtnClickedSlot()
{
    QByteArray arr1;
    QByteArray arr2;
    QByteArray arr;
    if (m_hexStatus)
    {
        arr1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
        arr2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());
    }
    else
    {
        arr1 = m_input1->toPlainText().toLatin1();
        arr2 = m_input2->toPlainText().toLatin1();
    }
    int len = (arr1.size() > arr2.size()) ? arr2.size() : arr1.size();
    arr.resize(len);
    for (int i = 0; i < len; ++i)
    {
        arr[i] = arr1[i] ^ arr2[i];
    }

    m_output->clear();
    m_output->append("两输入区的按位异或结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(arr));
}

void ConversionTools::BitwiseNotBtnClickedSlot()
{
    QByteArray arr1;
    QByteArray arr2;
    if (m_hexStatus)
    {
        arr1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
        arr2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());
    }
    else
    {
        arr1 = m_input1->toPlainText().toLatin1();
        arr2 = m_input2->toPlainText().toLatin1();
    }
    for (int i = 0; i < arr1.size(); ++i)
    {
        arr1[i] = ~arr1[i];
    }
    for (int i = 0; i < arr2.size(); i++)
    {
        arr2[i] = ~arr2[i];
    }
    m_output->clear();
    if (arr1.size() > 0)
    {
        m_output->append("输入区1按位取反结果为：");
        m_output->append(CommonTools::ByteArrayToHexString(arr1));
    }
    if (arr2.size() > 0)
    {
        m_output->append("");
        m_output->append("输入区2按位取反结果为：");
        m_output->append(CommonTools::ByteArrayToHexString(arr2));
    }

}

void ConversionTools::ToUpperBtnClickedSlot()
{
    if (m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Ascii"));
        return;
    }
    QString str1 = m_input1->toPlainText().toUpper();
    QString str2 = m_input2->toPlainText().toUpper();
    m_output->clear();

    if (str1.size() > 0)
    {
        m_output->append("输入区1转换为大写为：");
        m_output->append(str1);
    }
    if (str2.size() > 0)
    {
        m_output->append("");
        m_output->append("输入区2转换为大写为：");
        m_output->append(str2);
    }
}

void ConversionTools::ToLowerBtnClickedSlot()
{
    if (m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Ascii"));
        return;
    }
    QString str1 = m_input1->toPlainText().toLower();
    QString str2 = m_input2->toPlainText().toLower();
    m_output->clear();

    if (str1.size() > 0)
    {
        m_output->append("输入区1转换为大写为：");
        m_output->append(str1);
    }
    if (str2.size() > 0)
    {
        m_output->append("");
        m_output->append("输入区2转换为大写为：");
        m_output->append(str2);
    }
}

void ConversionTools::ToOppositeBtnClickedSlot()
{
    if (!m_hexStatus)
    {
        QMessageBox::warning(this, tr("提示"), tr("请将输入格式转换为Hex"));
        return;
    }
    m_output->clear();
    m_output->append("正数的反码为数字本身，负数的反码为符号位不变，其余位按位取反...");
}

void ConversionTools::InversionBtnClickedSlot()
{
    QByteArray arr1;
    QByteArray arr2;
    if (m_hexStatus)
    {
        arr1 = CommonTools::HexStringToByteArray(m_input1->toPlainText());
        arr2 = CommonTools::HexStringToByteArray(m_input2->toPlainText());
    }
    else
    {
        arr1 = m_input1->toPlainText().toLatin1();
        arr2 = m_input2->toPlainText().toLatin1();
    }
    int beginp = 0;
    int endp = arr1.size() - 1;
    char tmp = 0;
    for (int i = 0; i < arr1.size()/2; i++)
    {
        tmp = arr1.at(beginp);
        arr1[beginp] = arr1.at(endp);
        arr1[endp] = tmp;

        beginp++;
        endp--;
    }
    beginp = 0;
    endp = arr2.size() - 1;
    for (int i = 0; i < arr2.size()/2; i++)
    {
        tmp = arr2.at(beginp);
        arr2[beginp] = arr2.at(endp);
        arr2[endp] = tmp;

        beginp++;
        endp--;
    }
    QString str1;
    QString str2;
    if (m_hexStatus)
    {
        str1 = CommonTools::ByteArrayToHexString(arr1);
        str2 = CommonTools::ByteArrayToHexString(arr2);
    }
    else
    {
        str1 = m_codeConverter->toUnicode(arr1);
        str2 = m_codeConverter->toUnicode(arr2);
    }
    m_output->clear();
    if (str1.size() > 0)
    {
        m_output->append("输入区1字符串反转为：");
        m_output->append(str1);
    }
    if(str2.size() > 0)
    {
        m_output->append("");
        m_output->append("输入区2字符串反转为：");
        m_output->append(str2);
    }
}

void ConversionTools::GbkToChBtnClickedSlot()
{
    QByteArray arr = CommonTools::HexStringToByteArray(m_input1->toPlainText());

    m_output->clear();
    m_output->append("转换结果为：");
    m_output->append(m_codeConverter->toUnicode(arr));
}

void ConversionTools::ChToGbkBtnClickedSlot()
{
    QByteArray arr = m_codeConverter->fromUnicode(m_input1->toPlainText());

    m_output->clear();
    m_output->append("转换结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(arr));
}

void ConversionTools::Utf8ToChBtnClickedSlot()
{
    QByteArray arr = CommonTools::HexStringToByteArray(m_input1->toPlainText());

    m_output->clear();
    m_output->append("转换结果为：");
    m_output->append(m_utf8Converter->toUnicode(arr));
}

void ConversionTools::ChToUtf8BtnClickedSlot()
{
    QByteArray arr = m_utf8Converter->fromUnicode(m_input1->toPlainText());

    m_output->clear();
    m_output->append("转换结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(arr));
}

void ConversionTools::GbkToUtf8BtnClickedSlot()
{
    QString str = m_codeConverter->toUnicode(CommonTools::HexStringToByteArray(m_input1->toPlainText()));
    QByteArray utf8Arr = m_utf8Converter->fromUnicode(str);

    m_output->clear();
    m_output->append("转换结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(utf8Arr));
}

void ConversionTools::Utf8ToGbkBtnClickedSlot()
{
    QString str = m_utf8Converter->toUnicode(CommonTools::HexStringToByteArray(m_input1->toPlainText()));
    QByteArray gbkArr = m_codeConverter->fromUnicode(str);

    m_output->clear();
    m_output->append("转换结果为：");
    m_output->append(CommonTools::ByteArrayToHexString(gbkArr));
}

void ConversionTools::RadioBtnClickedSlot()
{
    if (m_conversionUi->AscRadioButton->isChecked())
    {
        m_hexStatus = false;
        m_conversionUi->CharacterNumBtn->setText("字符个数");
    }
    else
    {
        m_hexStatus = true;
        m_conversionUi->CharacterNumBtn->setText("字节数");
    }
}
