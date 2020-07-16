/**
 * @file filehandler.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Funkce pro otevirani a nacitani souboru. 
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QJsonObject fileHandle(QString filename)
{
    QFile file_obj(filename);
    QJsonObject json_obj;
    QString data_string;
    if(!file_obj.open(QIODevice::ReadOnly)){
        return json_obj;
    }

    QByteArray jsonData = file_obj.readAll();
    file_obj.close();

    QJsonDocument data_doc = QJsonDocument::fromJson(jsonData);
    if(data_doc.isNull()){
        return json_obj;
    }

    json_obj = data_doc.object();
    if (json_obj.isEmpty())
    {
        return json_obj;
    }
    return json_obj;
}
