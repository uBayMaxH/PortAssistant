#ifndef MULTIPLESTRINGSENDING_H
#define MULTIPLESTRINGSENDING_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QTimer>
#include <QTextCodec>
#include "SendingArea.h"

#define SEND_CLAUSES_MAX        99

namespace Ui {
class MultipleStringSending;
}

class MultipleStringSending : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleStringSending(QWidget *parent = nullptr, SendingArea *sendInstance = nullptr);
    ~MultipleStringSending();

    bool CycleSendStateGet();
    void CycleSendStateReset();
    void CycleTimerSet(bool state);

private:
    void Send(void);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void CycleCheckBoxClicked(bool state);
    void HexCheckBoxClicked(bool state);
    void SendButtonClicked(void);
    void ResetButtonClicked(void);
    void AutoSendingSlot(void);


private:
    Ui::MultipleStringSending *m_mulStrUi;

    SendingArea         *m_sendInstance = nullptr;

    QVBoxLayout         *m_VBoxLayout = nullptr;
    QHBoxLayout         *m_HBoxLayout[SEND_CLAUSES_MAX] = {nullptr};
    QCheckBox           *m_sendEnable[SEND_CLAUSES_MAX] = {nullptr};
    QLineEdit           *m_sendLineEdit[SEND_CLAUSES_MAX] = {nullptr};

    QTextCodec          *m_codeConverter;       //用于解决接收和发送中文乱码问题
    bool                m_hexSending = false;
    bool                m_cycleSend = false;
    QTimer              *m_cycleTimer = nullptr;
    qint8               m_curSendClauses = 0;
    qint8               m_lastSendClauses = 0;
    bool                m_sendClausesReset = false;
};

#endif // MULTIPLESTRINGSENDING_H
