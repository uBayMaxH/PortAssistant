#ifndef CONVERSIONTOOLS_H
#define CONVERSIONTOOLS_H

#include <QWidget>
#include "QTextEditEx.h"
#include <QTextCodec>

#define BUF_MAX     1024

namespace Ui {
class ConversionTools;
}

class ConversionTools : public QWidget
{
    Q_OBJECT

public:
    explicit ConversionTools(QWidget *parent = nullptr);
    ~ConversionTools();

public:
    void Init();

private slots:
    void CharacterNumBtnClickedSlot();
    void RandomNumBtnClickedSlot();
    void HexToDecBtnClickedSlot();
    void DecToHexBtnClickedSlot();
    void HexToAscBtnClickedSlot();
    void AscToHexBtnClickedSlot();
    void HexToFloatBtnClickedSlot();
    void FloatToHexBtnClickedSlot();
    void BitwiseAndBtnClickedSlot();
    void BitwiseOrBtnClickedSlot();
    void BitwiseXorBtnClickedSlot();
    void BitwiseNotBtnClickedSlot();
    void ToUpperBtnClickedSlot();
    void ToLowerBtnClickedSlot();
    void ToOppositeBtnClickedSlot();
    void InversionBtnClickedSlot();
    void GbkToChBtnClickedSlot();
    void ChToGbkBtnClickedSlot();
    void Utf8ToChBtnClickedSlot();
    void ChToUtf8BtnClickedSlot();
    void GbkToUtf8BtnClickedSlot();
    void Utf8ToGbkBtnClickedSlot();

    void RadioBtnClickedSlot();

private:
    Ui::ConversionTools *m_conversionUi;

    QTextCodec      *m_codeConverter;       //用于解决接收和发送中文乱码问题
    QTextCodec      *m_utf8Converter;

    QTextEditEx         *m_input1 = nullptr;
    QTextEditEx         *m_input2 = nullptr;
    QTextEditEx         *m_output = nullptr;

    bool                m_hexStatus = false;
};

#endif // CONVERSIONTOOLS_H
