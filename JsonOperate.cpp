#include "JsonOperate.h"

JsonOperate::JsonOperate(QString jsonFileName)
{
    m_jsonFileName = jsonFileName;

    //当json文件名不为空时首先读取该文件
    if (!m_jsonFileName.isNull())
    {
        ReadJsonFile();
    }
}

JsonOperate::~JsonOperate()
{

}

void JsonOperate::ReadJsonFile()
{
    QFile *jsonFile = new QFile(QDir::homePath() + m_jsonFileName);
    jsonFile->setFileName(m_jsonFileName);
    //这里读json文件时已可读写的方式进行打开，如果文件不存在则创建该文件
    if (!jsonFile->open(QIODevice::ReadWrite))
    {
        return;
    }

    //读取json文件
    QByteArray jsonFileData = jsonFile->readAll();
    jsonFile->close();

    // 转化为 JSON 文档
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFileData, &jsonError);

    if (!jsonDoc.isNull())
    {
        if (jsonError.error == QJsonParseError::NoError)
        {
            // 解析未发生错误
            if (jsonDoc.isObject())
            {
                // JSON 文档为对象
                m_jsonObject = jsonDoc.object();
            }
        }
    }
    else
    {
        //json文件为空，这里添加内容
        JsonFileInit();
    }
}

bool JsonOperate::WriteJsonFile()
{
    //以读写方式打开当前目录下的json文件，若该文件不存在则会自动创建
    QFile *jsonFile = new QFile(QDir::currentPath() + m_jsonFileName);
    jsonFile->setFileName(m_jsonFileName);
    if (!jsonFile->open(QIODevice::ReadWrite))
    {
        return false;
    }

    // 使用QJsonDocument设置该json对象
    QJsonDocument jsonDoc;
    jsonDoc.setObject(m_jsonObject);

    // 将json以文本形式写入文件并关闭文件。
    jsonFile->write(jsonDoc.toJson());
    jsonFile->close();
    return true;
}

void JsonOperate::JsonRemoveItem(QJsonObject &jsonObject, QString key)
{
    jsonObject.remove(key);
}

void JsonOperate::JsonFileInit()
{
    //构建GLOBAL_CONFIG
    QJsonObject globalObject;
    QJsonObject portSelectObj;
    portSelectObj.insert("PORT", "");
    globalObject.insert("PORT_SELECT", QJsonValue(portSelectObj));

    QJsonObject displaySetObj;
    displaySetObj.insert("BACKGROUND_COLOR", "");
    displaySetObj.insert("FONT_COLOR", "");
    displaySetObj.insert("FONT_SIZE", "");
    globalObject.insert("DISPLAY_SET", QJsonValue(displaySetObj));

    QJsonObject receiveSetObj;
    receiveSetObj.insert("HEX", false);
    receiveSetObj.insert("TIME", false);
    globalObject.insert("RECEIVE_SET", QJsonValue(receiveSetObj));

    QJsonObject sendSetObj;
    sendSetObj.insert("HEX", false);
    globalObject.insert("SEND_SET", QJsonValue(sendSetObj));

    m_jsonObject.insert("GLOBAL_CONFIG", QJsonValue(globalObject));

    //构建SENDING_AREA_CONFIG
    QJsonObject sendingAreaObject;
    sendingAreaObject.insert("TIMER", false);
    sendingAreaObject.insert("INTERVAL", 1000);
    sendingAreaObject.insert("SEND_STR", "");
    m_jsonObject.insert("SENDING_AREA_CONFIG", QJsonValue(sendingAreaObject));

    //构建SERIAL_SETTING_CONFIG
    QJsonArray setArray;
    QJsonObject configObj;
    configObj.insert("NAME", "");   //串口名
    configObj.insert("BAUD", 0);    //波特率
    setArray.append(QJsonValue(configObj));
    QJsonObject serialSetObj;
    serialSetObj.insert("LAST_SERIAL_NAME", "");
    serialSetObj.insert("SETTING_CFG", QJsonValue(setArray));
    m_jsonObject.insert("SERIAL_SETTING_CONFIG", QJsonValue(serialSetObj));

    //构建NET_SETTING_CONFIG
    m_jsonObject.insert("NET_SETTING_CONFIG", "");

    //构建MUL_STRING_CONFIG
    m_jsonObject.insert("MUL_STRING_CONFIG", "");

    WriteJsonFile();
}

QJsonObject JsonOperate::JsonObjectGet(void)
{
    return m_jsonObject;
}

void JsonOperate::WriteJson(QString key, QJsonValue value)
{
    // 使用QJsonObject对象插入键值对。
    JsonAddItem(m_jsonObject, key, value);
    WriteJsonFile();
}

