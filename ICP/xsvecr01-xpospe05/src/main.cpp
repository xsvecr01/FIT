/**
 * @file main.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Hlavni soubor.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "mainwindow.h"
#include "coordinate.h"
#include "street.h"
#include "stop.h"

#include <QApplication>
#include <string>

/**
 * @brief Hlavni fuknce programu.
 * 
 * @param argc int pocet argumentu
 * @param argv char pole argumentu
 * @return int navratovy kod
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
