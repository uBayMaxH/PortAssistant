/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "SerialSettings.h"
#include "ui_SerialSettings.h"

#include <QIntValidator>
#include <QLineEdit>
#include <QSerialPortInfo>
#include <string.h>

static const char blankString[] = QT_TRANSLATE_NOOP("SerialSettings", "N/A");

SerialSettings::SerialSettings(QJsonObject obj, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SerialSettings),
    m_intValidator(new QIntValidator(0, 4000000, this))
{
    m_ui->setupUi(this);
    /*阻塞除当前窗口以外的其他所有窗口*/
    setWindowModality(Qt::ApplicationModal);

    m_ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    m_jsonObject = obj;

    connect(m_ui->applyButton, &QPushButton::clicked,
            this, &SerialSettings::Apply);
    connect(m_ui->refreshButton, &QPushButton::clicked,
            this, &SerialSettings::Refresh);
    connect(m_ui->serialPortInfoListBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SerialSettings::ShowPortInfo);
    connect(m_ui->baudRateBox,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SerialSettings::CheckCustomBaudRatePolicy);
    connect(m_ui->serialPortInfoListBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SerialSettings::CheckCustomDevicePathPolicy);

    FillPortsParameters();
    FillPortsInfo();
    UpdateSettings();
}

SerialSettings::~SerialSettings()
{
    delete m_ui;
}

SerialSettings::Settings SerialSettings::settings() const
{
    return m_currentSettings;
}

void SerialSettings::ShowPortInfo(int idx)
{
    if (idx == -1)
        return;

    const QStringList list = m_ui->serialPortInfoListBox->itemData(idx).toStringList();
    m_ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    m_ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    m_ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    m_ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    m_ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    m_ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SerialSettings::Apply()
{
    UpdateSettings();
    ApplyClicked();
    hide();
}

void SerialSettings::Refresh()
{
    FillPortsInfo();
    UpdateSettings();
}

void SerialSettings::CheckCustomBaudRatePolicy(int idx)
{
    //判断是否是自定义
    const bool isCustomBaudRate = !m_ui->baudRateBox->itemData(idx).isValid();
    //设置是否可编辑
    m_ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
    {
        m_ui->baudRateBox->clearEditText();
        QLineEdit *edit = m_ui->baudRateBox->lineEdit();
        edit->setValidator(m_intValidator);
    }
}

void SerialSettings::CheckCustomDevicePathPolicy(int idx)
{
    const bool isCustomPath = !m_ui->serialPortInfoListBox->itemData(idx).isValid();
    m_ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
    {
        m_ui->serialPortInfoListBox->clearEditText();
    }
}

void SerialSettings::FillPortsParameters()
{
    m_ui->baudRateBox->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    m_ui->baudRateBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    m_ui->baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    m_ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    m_ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    m_ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    m_ui->baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    m_ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    m_ui->baudRateBox->addItem(QStringLiteral("128000"), QSerialPort::Baud128000);
    m_ui->baudRateBox->addItem(QStringLiteral("256000"), QSerialPort::Baud256000);
    m_ui->baudRateBox->addItem(tr("Custom"));
    m_ui->baudRateBox->setCurrentIndex(7);

    m_ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    m_ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    m_ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    m_ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    m_ui->dataBitsBox->setCurrentIndex(3);

    m_ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    m_ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    m_ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    m_ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    m_ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    m_ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    m_ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    m_ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    m_ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SerialSettings::FillPortsInfo()
{
    m_ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        m_ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    m_ui->serialPortInfoListBox->addItem(tr("Custom"));
    /*在此读取json配置文件并根据结果进行相应初始化*/
    QString lastSerialName = m_jsonObject.value("LAST_SERIAL_NAME").toString();
    bool existFlag = false;
    for (int i = 0; i < m_ui->serialPortInfoListBox->count(); i++)
    {
        if (m_ui->serialPortInfoListBox->itemText(i) == lastSerialName)
        {
            m_ui->serialPortInfoListBox->setCurrentIndex(i);
            existFlag = true;
        }
    }
    if(existFlag)
    {
        if (JsonOperate::JsonContains(m_jsonObject, "SETTING_CFG"))
        {
            QJsonValue value = m_jsonObject.value("SETTING_CFG");
            if (value.isArray())
            {
                QJsonArray array = value.toArray();
                for (int i = 0; i < array.size(); i++)
                {
                    if (array.at(i).isObject())
                    {
                        QJsonObject obj = array.at(i).toObject();
                        if ((obj.value("NAME").isString()) && (obj.value("NAME").toString() == lastSerialName))
                        {
                            bool existBaud = false;
                            for (int i = 0; i < m_ui->baudRateBox->count(); i++)
                            {
                                if (m_ui->baudRateBox->itemText(i) == QString("%1").arg(obj.value("BAUD").toInt()))
                                {
                                    m_ui->baudRateBox->setCurrentIndex(i);
                                    existBaud = true;
                                }
                            }
                            if (!existBaud)
                            {
                                //自定义的波特率
                                m_ui->baudRateBox->setCurrentText(QString("%1").arg(obj.value("BAUD").toInt()));
                            }
                        }
                    }
                }
            }
        }
    }
}

void SerialSettings::SerialPortInfoGet(QStringList *list)
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        *list << info.portName() + " " + info.description();
    }
}

void SerialSettings::CurrentSerialNameSet(QString name)
{
    if (name != nullptr)
    {
        m_currentSettings.name = name;
    }
    //将设置界面同步到选择的串口
    for (int i = 0; i < m_ui->serialPortInfoListBox->count(); i++)
    {
        if (m_ui->serialPortInfoListBox->itemText(i) == name)
        {
            m_ui->serialPortInfoListBox->setCurrentIndex(i);
            UpdateSettings();
            return;
        }
    }
}

QString SerialSettings::CurrentSerialNameGet(void)
{
    return m_currentSettings.name;
}

void SerialSettings::UpdateSettings()
{
    //串口名
    m_currentSettings.name = m_ui->serialPortInfoListBox->currentText();

    //波特率
    if (m_ui->baudRateBox->itemData(m_ui->baudRateBox->currentIndex()).isValid())
    {
        //如果是自定义的时候就直接转换
        m_currentSettings.baudRate = m_ui->baudRateBox->currentText().toInt(); 
    }
    else
    {
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    m_ui->baudRateBox->itemData(m_ui->baudRateBox->currentIndex()).toInt());
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    //数据位
    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                m_ui->dataBitsBox->itemData(m_ui->dataBitsBox->currentIndex()).toInt());
    m_currentSettings.stringDataBits = m_ui->dataBitsBox->currentText();

    //校验位
    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                m_ui->parityBox->itemData(m_ui->parityBox->currentIndex()).toInt());
    m_currentSettings.stringParity = m_ui->parityBox->currentText();

    //停止位
    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                m_ui->stopBitsBox->itemData(m_ui->stopBitsBox->currentIndex()).toInt());
    m_currentSettings.stringStopBits = m_ui->stopBitsBox->currentText();

    //流控
    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                m_ui->flowControlBox->itemData(m_ui->flowControlBox->currentIndex()).toInt());
    m_currentSettings.stringFlowControl = m_ui->flowControlBox->currentText();

}
