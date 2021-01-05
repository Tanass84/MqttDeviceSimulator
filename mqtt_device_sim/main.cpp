#include "mainwindow.h"
#include "dialog.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QMqttClient>
#include <QTcpSocket>

using namespace std;

QVector <QString> shapeIdVector;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWin;
    mainWin.show();
    qDebug()<<&mainWin<<Qt::endl;

    Dialog ConfigWin(&mainWin);

    return app.exec();

}
