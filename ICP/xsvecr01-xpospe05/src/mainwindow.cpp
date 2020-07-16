/**
 * @file mainwindow.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida MainWindow
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filehandler.h"
#include "coordinate.h"
#include "stop.h"
#include "street.h"
#include "line.h"
#include "autobus.h"

#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QtCore>

#include <QGraphicsLineItem>

/**
 * @brief seznam ulic
 * 
 */
list<Street> listStreets;

/**
 * @brief seznam zastavek
 * 
 */
list<Stop> listStops;

/**
 * @brief seznam linek
 * 
 */
list<Line> listLine;

/**
 * @brief pole grafickych usecek trasy
 * 
 */
vector<QGraphicsLineItem*> allLines;

/**
 * @brief pole grafickych objektu
 * 
 */
vector<QGraphicsItem*> allItems;

/**
 * @brief pole autobusu
 * 
 */
vector<Autobus*> allBuses;

/**
 * @brief kontrola vytvoreneho widgetu
 * 
 */
bool isWdg = false;
/**
 * @brief kontrola pro timer
 * 
 */
bool isReset = true;

/**
 * @brief tabulka zastavek
 * 
 */
QTableWidget *lineView;

/**
 * @brief vybrana linka
 * 
 */
string selectedLine;
/**
 * @brief string casu aplikace
 * 
 */
QString stopTime;

static int sec = 0;
static int minute = 0;
static int hour = 12;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphicsScene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    connect(ui->zoom, &QSlider::valueChanged, this, &MainWindow::zoom);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),graphicsScene, SLOT(advance()));
    timer->start(1);
    timer->setInterval(1000);

    connect(timer, SIGNAL(timeout()),this, SLOT(updateCountdown()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openMapBtn_clicked()
{

    for (QGraphicsItem *item : allLines){

        QGraphicsLineItem *lines = qgraphicsitem_cast<QGraphicsLineItem *>(item);
        if (!lines) continue;

        lines->setPen(QPen({Qt::transparent, 0}));
        lines->update();
    }

    listStreets.clear();
    listStops.clear();
    listLine.clear();

    for(Autobus *bus : allBuses){
        delete(bus);
    }
    allBuses.clear();
    for(QGraphicsItem *itemos : allItems){
        delete(itemos);
    }
    allItems.clear();

    ui->graphicsView->setScene(graphicsScene);

    ui->comboBox->clear();

    QString file_name = QFileDialog::getOpenFileName(this, "Soubor s mapovym podkladem", "./");
    QJsonObject json_obj = fileHandle(file_name);

    if(json_obj.isEmpty()){
        QMessageBox::warning(this, tr("Chyba v souboru"), tr("Zkontrolujte format JSON"));
        return;
    }

    //ulice
    QJsonValue value;
    QJsonArray array;
    value = json_obj.value("Streets");
    array = value.toArray();
    foreach (const QJsonValue &v, array){
        string id = v.toObject().value("id").toString().toUtf8().constData();
        QJsonArray cArray = v.toObject().value("coords").toArray();

        list<Coordinate> coordinates;
        foreach (const QJsonValue &v1, cArray) {
            Coordinate c(v1.toArray()[0].toInt(), v1.toArray()[1].toInt());
            coordinates.push_back(c);
        }
        Street street(id, coordinates);
        listStreets.push_back(street);
    }


    if(listStreets.empty()){
        QMessageBox::warning(this, tr("Chyba v nacitani"), tr("V souboru nejsou zadne ulice."));
        return;
    }

    //zastavky
    value = json_obj.value("Stops");
    array = value.toArray();
    foreach (const QJsonValue &v, array){
        string id = v.toObject().value("id").toString().toUtf8().constData();
        Coordinate c(v.toObject().value("coords").toArray()[0].toInt(),v.toObject().value("coords").toArray()[1].toInt());
        Stop stop(id, c);
        listStops.push_back(stop);
    }


    //pridani zastavek do ulic
    for (Street &stre : listStreets){
        for(Stop sto : listStops){
            stre.addStop(sto);
        }
    }
    initScene();
}

void MainWindow::initScene()
{
    //kresleni jednotlivych ulic
    for (Street s : listStreets){
        int z = 0;

        Coordinate prevc;
        for (Coordinate c : s.getCoordinates()){
           if (z == 0)
           {
              prevc = c;
              z++;
              continue;
           }
           auto line = graphicsScene->addLine(prevc.getX(), prevc.getY(), c.getX(), c.getY());
           line->setPen(QPen({Qt::gray, 1}));
           line->setFlag(QGraphicsItem::ItemIsSelectable);
           allItems.push_back(line);

           prevc = c;
           z++;
        }
    }

    //kresleni zastavek
    for (Street stre : listStreets){
        for (Stop sto : stre.getStops()){
            int cx = sto.getCoordinate().getX() - 1;
            int cy = sto.getCoordinate().getY() - 1;
            auto elipse = graphicsScene->addEllipse(cx, cy, 2, 2, QPen({Qt::red, 1}), QBrush(QColor("#FFFFFF")));
            allItems.push_back(elipse);
        }
    }
}

void MainWindow::zoom(int z){
    auto org = ui->graphicsView->transform();
    qreal scale = z/10;
    ui->graphicsView->setTransform(QTransform(scale, org.m12(), org.m21(), scale, org.dx(), org.dy()));
}

void MainWindow::on_openLineBtn_clicked()
{
    listLine.clear();
    isReset = true;

    QString file_name = QFileDialog::getOpenFileName(this, "Soubor s linkami", "./");
    QJsonObject json_obj = fileHandle(file_name);

    if(listStreets.empty()){
        QMessageBox::warning(this, tr("Chyba"), tr("Nejsou nahrany ulice"));
        return;
    }

    if(json_obj.isEmpty()){
        QMessageBox::warning(this, tr("Chyba v souboru"), tr("Zkontrolujte format JSON"));
        return;
    }

    //linka
    QJsonValue value;
    QJsonArray array;
    value = json_obj.value("Lines");
    array = value.toArray();
    foreach (const QJsonValue &v, array){
        string id = v.toObject().value("id").toString().toUtf8().constData();
        QJsonArray cArray = v.toObject().value("route").toArray();

        Line line(id);

        foreach (const QJsonValue &v1, cArray) {
            string streetname = v1.toArray()[0].toString().toUtf8().constData();
            string stopname = v1.toArray()[1].toString().toUtf8().constData();
            int lineHour = stoi(v1.toArray()[2].toString().toUtf8().constData());
            int lineMin = stoi(v1.toArray()[3].toString().toUtf8().constData());

            Street tempStre;
            Stop tempSto;

            for(Street s : listStreets){
                if(s.getId().compare(streetname) == 0){
                    tempStre = s;
                    //je zadana zastavka?
                    if (stopname.compare("0") != 0){
                        //je zastavka v dane ulici?
                        for(Stop sto : tempStre.getStops()){
                            if(sto.getId().compare(stopname) == 0){
                                tempSto = sto;
                                line.addStreet(tempStre, tempSto);

                                if((lineHour != 99) && (lineMin != 99)){
                                    line.stopHour.push_back(lineHour);
                                    line.stopMinute.push_back(lineMin);
                                }
                            }
                        }
                    }
                    else line.addStreet(tempStre);
                }
            }
        }
        listLine.push_back(line);
    }

    if(listLine.empty()){
        QMessageBox::warning(this, tr("Chyba v nacitani"), tr("V souboru nejsou zadne linky."));
        return;
    }


    for(Autobus *bus : allBuses){
        delete(bus);
    }
    allBuses.clear();
    clearMap();

    ui->comboBox->clear();
    ui->comboBox->addItem("Linky");

    for(Line lin : listLine){

        string str = lin.getId();
        QString qstr = QString::fromStdString(str);
        ui->comboBox->addItem(qstr);

        Autobus *bus = new Autobus();
        bus->setLine(lin);
        allBuses.push_back(bus);
        graphicsScene->addItem(bus);
    }

    isReset = false;
    return;
}

void MainWindow::clearMap(){
    for (QGraphicsItem *item : allLines){

        QGraphicsLineItem *lines = qgraphicsitem_cast<QGraphicsLineItem *>(item);
        if (!lines) continue;

        lines->setPen(QPen({Qt::transparent, 0}));
        lines->update();
    }

    for(Autobus *bus : allBuses){
        bus->Brush.setColor(Qt::red);
    }
}

void MainWindow::drawLine(string line)
{
    for (Line linka : listLine){
        if (linka.getId().compare(line) == 0){
            for (Street lstre : linka.getStreets()){
                for (Street s2 : listStreets){
                   if (lstre.getId().compare(s2.getId()) == 0){
                       int z1 = 0;
                       Coordinate prevc1;
                       for (Coordinate c1 : s2.getCoordinates()){
                          if (z1 == 0){
                             prevc1 = c1;
                             z1++;
                             continue;
                          }
                          auto line1 = graphicsScene->addLine(prevc1.getX(), prevc1.getY(), c1.getX(), c1.getY());
                          line1->setPen(QPen({Qt::green, 0.5}));
                          allLines.push_back(line1);
                          prevc1 = c1;
                        }
                    }
                }
            }
        }
    }

    for(Autobus *bus : allBuses){
        if(bus->getLineId().compare(line) == 0){
            bus->Brush.setColor(Qt::green);
        }
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    string line = arg1.toUtf8().constData();
    clearMap();

    drawLine(line);

    selectedLine = line;


    for (Line linka : listLine){
        if (linka.getId().compare(line) == 0){

            QWidget *wdg = new QWidget;
            wdg->setWindowTitle(arg1);
            wdg->setMinimumHeight(350);
            wdg->setMaximumWidth(300);
            wdg->setMinimumWidth(300);
            QGridLayout *wdgrid = new QGridLayout();
            wdg->setLayout(wdgrid);
            lineView = new QTableWidget();
            lineView->setColumnCount(2);
            lineView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            lineView->verticalHeader()->setVisible(false);
            lineView->horizontalHeader()->setVisible(false);

            wdgrid->addWidget(lineView);
            wdg->show();

            lineView->insertRow(lineView->rowCount());

            auto headertext = new QTableWidgetItem("Poslední zastávka");
            headertext->setBackgroundColor(Qt::black);
            headertext->setTextColor(Qt::white);
            lineView->setItem(0, 0, headertext);
            lineView->setColumnWidth(0, 130);

            auto headertext1 = new QTableWidgetItem("Čas příjezdu");
            headertext1->setBackgroundColor(Qt::black);
            headertext1->setTextColor(Qt::white);
            lineView->setItem(0, 1, headertext1);
            isWdg = true;

            for(Stop stopis : linka.getStops()){
                lineView->insertRow(lineView->rowCount());
                string str = stopis.getId();
                QString qstr = QString::fromStdString(str);
                auto current = new QTableWidgetItem(qstr);
                current->setTextColor(Qt::gray);
                lineView->setItem(lineView->rowCount()-1, 0, current);
            }


        }
    }
}

void MainWindow::on_timeplus_clicked()
{
    if(timer->interval() > 100){
        timer->setInterval(timer->interval()-100);
    }
    else if(timer->interval() > 20){
        timer->setInterval(timer->interval()-20);
    }
}

void MainWindow::on_timeminus_clicked()
{
    if(timer->interval() < 100){
        timer->setInterval(timer->interval()+20);
    }

    else if(timer->interval() < 1000){
        timer->setInterval(timer->interval()+100);
    }
}

void MainWindow::updateCountdown()
{
    stopTime = "";
    stopTime.append(QString::number(hour).rightJustified(2, '0'));
    stopTime.append(":");
    stopTime.append(QString::number(minute).rightJustified(2, '0'));
    stopTime.append(":");
    stopTime.append(QString::number(sec).rightJustified(2, '0'));

    sec++;
    if(isReset){
        sec = minute = hour = 0;
        hour = 12;
    }

    if (sec == 60)
    {
        minute++;
        sec = 0;
    }
    if (minute == 60)
    {
        hour++;
        minute = 0;
    }
    if (hour == 24){
        hour = 0;
    }

    ui->countdown->setText(stopTime);

    if((minute == 20 || minute == 40 || minute == 0) && sec == 0){
        for(Autobus *bus : allBuses){
            bus->speed = 1;
        }
        for(Line &linka : listLine){
            for(unsigned long i = 0; i < linka.stopHour.size(); i++){
                linka.stopMinute[i] = 20+linka.stopMinute[i];
                if(linka.stopMinute[i] >= 60){
                    linka.stopMinute[i] -= 60;
                    linka.stopHour[i]++;
                }
            }
        }
    }


    for(Autobus *bus : allBuses){
        if(bus->lastStop.getId().compare(bus->tempStop.getId()) != 0)
        {
            if(bus->lastStop.getId().compare(bus->getLine().getStops().front().getId()) == 0){
                bus->speed = 0;
            }
            bus->tempStop = bus->lastStop;
        }
    }

    if(isWdg){
        stopSelector();
    }
}

void MainWindow::stopSelector()
{
    for(int i = 1; i < lineView->rowCount(); i++)
    {
        QTableWidgetItem* item = lineView->item(i, 0);
        for(Autobus *bus : allBuses){
            if(bus->getLineId().compare(selectedLine) == 0){
                if(bus->lastStop.getId().compare(item->text().toUtf8().constData()) == 0){
                    item->setTextColor(Qt::darkGreen);
                }
                else item->setTextColor(Qt::gray);
            }
        }

        for(Line linka : listLine){
            if(linka.getId().compare(selectedLine) == 0){
                QString stopHourMin;
                for(int i = 0; i < (int)(linka.stopHour.size()); i++){
                    stopHourMin = "";
                    stopHourMin.append(QString::number(linka.stopHour[i]).rightJustified(2, '0'));
                    stopHourMin.append(":");
                    stopHourMin.append(QString::number(linka.stopMinute[i]).rightJustified(2, '0'));
                    auto stopTimeItem = new QTableWidgetItem(stopHourMin);
                    lineView->setItem(i+1, 1, stopTimeItem);
                }
            }
        }
    }
}
