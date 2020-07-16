/**
 * @file mainwindow.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida MainWindow
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <string>

#include "line.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief trida pro MainWindow
 * 
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor tridy MainWindow
     * 
     * @param parent QWidget Rodicovsky widget
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor tridy MainWindow
     * 
     */
    ~MainWindow();

private slots:

    /**
     * @brief Funkce aktivovana po stisku tlacitka otevreni map. Otevira soubor JSON a parsuje JSON objekty,
     *  ze kterych ziskava ulice a zastavky.
     * Ulice a zastavky se pridavaji do seznamu listStreets a listStops.
     * 
     */
    void on_openMapBtn_clicked();

    /**
     * @brief Funkce aktivovana po stisku tlacitka otevreni linek. Otevria soubor JSON a parsuje JSON objekty,
     * ze kterych ziskava linky a jejich jizdni rady.
     * Linky se pridavaji do seznamu listLine
     * Zaroven vytvari pro kazdou linku jeden autobus
     * 
     */
    void on_openLineBtn_clicked();

    /**
     * @brief Funkce aktivovana otevrenim combo boxu. Zvyraznuje vybranou linku, a ukazuje jeji jizdni rad a aktualni zastavku.
     * 
     * @param arg1 const QString vybrana linka
     */
    void on_comboBox_activated(const QString &arg1);

    /**
     * @brief Aktivovana tlacitkem, stara se o zrychlovani casu.
     * 
     */
    void on_timeplus_clicked();

    /**
     * @brief Aktivovana tlacitekm, stara se o zpomalovani casu.
     * 
     */
    void on_timeminus_clicked();

    /**
     * @brief Funkce se stara o aktualizaci casu v aplikaci. Je volana s kazdou zmenou timeru.
     * 
     */
    void updateCountdown();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QGraphicsScene *graphicsScene;

    /**
     * @brief Zvyraznuje vybranou linku na mape
     * 
     * @param line string vybrana linka
     */
    void drawLine(string line);

    /**
     * @brief Vymaze vsechny vykreslene objekty z grafickeho podkladu
     * 
     */
    void clearMap();

    /**
     * @brief Vykresli vsechny ulice a zastavky na graficky podklad
     * 
     */
    void initScene();

    /**
     * @brief Slouzi pro priblizovani a oddalovani mapy, je volana zmenou hodnoty slideru
     * 
     * @param z int hodnota priblizeni
     */
    void zoom(int z);

    /**
     * @brief Zvyraznuje posledni navstivenou zastavku
     * 
     */
    void stopSelector();
};
#endif // MAINWINDOW_H
