#ifndef JSONOPERATE_H
#define JSONOPERATE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QFile>
#include <QDir>

class JsonOperate
{
public:
    JsonOperate(QString jsonFileName);
    ~JsonOperate();

private:
    /*读取json文件，读取后将内容存到m_jsonObject，该函数只在构造函数中调用，既在初始化类的时候就读取文件，如果文件不存在则创建该文件*/
    void ReadJsonFile();
    /*将m_jsonObject中的内容写入json文件，如果文件不存在则创建并写入*/
    bool WriteJsonFile();
    /*删除*/
    void JsonRemoveItem(QJsonObject &jsonObject, QString key);
    /*初始化json文件-当没有该文件或文件为空时调用*/
    void JsonFileInit();
public:
    /*使用QJsonObject对象插入键值对*/
    static void JsonAddItem(QJsonObject &jsonObject, QString key, QJsonValue value)
    {
        jsonObject.insert(key, value);
    }
    QJsonObject JsonObjectGet(void);
    /*外部调用写json的接口*/
    void WriteJson(QString key, QJsonValue value);

    static bool JsonContains(QJsonObject jsonObject, QString key)
    {
        if (jsonObject.contains(key))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*获取键值对的值*/
    static QJsonValue JsonGetValue(QJsonObject jsonObject, QString key)
    {
        QJsonValue value = QJsonValue::Null;
        if (jsonObject.contains(key))
        {
            value = jsonObject.value(key);
        }
        return value;
    }

    /*获取jsonObiect*/
    static QJsonObject JsonGetObject(QJsonObject jsonObject, QString key)
    {
        QJsonObject object{};
        if (jsonObject.contains(key))
        {
            QJsonValue value = jsonObject.value(key);
            object = value.toObject();
        }
        return object;
    }

private:
    QString     m_jsonFileName;
    QJsonObject m_jsonObject{};
};

#endif // JSONOPERATE_H
