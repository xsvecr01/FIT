/**
 * @file filehandler.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Funkce pro otevirani a nacitani souboru.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>
#include <QJsonObject>

/**
 * @brief Funkce pro nacitani souboru, a vytvoreni JSON objektu
 * 
 * @param filename QString nazev souboru
 * @return QJsonObject Vytvoreny JSON objekt ze souboru
 */
QJsonObject fileHandle(QString filename);

#endif // FILEHANDLER_H
