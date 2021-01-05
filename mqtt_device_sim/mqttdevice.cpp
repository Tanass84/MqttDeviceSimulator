#include "mqttdevice.h"
#include "mainwindow.h"
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>


MqttDevice::MqttDevice(QWidget *parent) : QMqttClient(parent)
{
    fileTrajects = new QFile("../resources/trajects.txt", this);
    textStreamTrajects = new QTextStream(fileTrajects);
    firstLine = new QString;
    m_authentification = false;

    connect(this, SIGNAL(connected()), this, SLOT(Authentifaction()));
    connect(this, SIGNAL(messageReceived(const QByteArray, const QMqttTopicName)), this, SLOT(receivedMsgHandle(const QByteArray, const QMqttTopicName)));
    connect(this, SIGNAL(disconnected()), this, SLOT(deviceDisconnect()));
}

MqttDevice::~MqttDevice()
{
    if (fileTrajects->isOpen())
        fileTrajects->close();
    delete fileTrajects;
    delete textStreamTrajects;
    delete firstLine;
}

void MqttDevice::setParam(QString name, QString imei, QString imsi, QString iccid, QString msisdn)
{
    m_deviceName = name;
    m_imei = imei;
    m_imsi = imsi;
    m_iccid = iccid;
    m_msisdn = msisdn;
    m_cliendId = "c000";
    m_deviceId = "d0000";

    eventTablebWidgetItemVector.append(new QTableWidgetItem("Connected"));
    eventTablebWidgetItemVector.append(new QTableWidgetItem("Disconnected"));
    eventTablebWidgetItemVector.append(new QTableWidgetItem("Subscribed"));
    eventTablebWidgetItemVector.append(new QTableWidgetItem("Unsubscribed"));
    eventTablebWidgetItemVector.append(new QTableWidgetItem("Received"));
    eventTablebWidgetItemVector.append(new QTableWidgetItem("Published"));

    m_authDeviceTopic = new QMqttTopicName("auth/device/"+ m_imei);
    m_dataTopic = new QMqttTopicName("data/"+ m_cliendId + "/" + m_deviceId);
    m_statusTopic = new QMqttTopicName("status/"+ m_cliendId + "/" + m_deviceId);

    m_authSysTopic = new QMqttTopicFilter("auth/sys/"+ m_imei);
    m_commandTopic = new QMqttTopicFilter("command/" + m_cliendId + "/" + m_deviceId);

    topicTablebWidgetItemVector.append(new QTableWidgetItem(QString(m_authDeviceTopic->name())));
    topicTablebWidgetItemVector.append(new QTableWidgetItem(QString(m_authSysTopic->filter())));

    deviceTimer = new QTimer(this);
}

bool MqttDevice::isAuthentified()
{
    return m_authentification;
}

void MqttDevice::tableAddInfo(QString Event, QString topicName, QString msg)
{
    QTabWidget *tab = (QTabWidget*)parent()->parent();
    QTableWidget *table = (QTableWidget*)tab->findChild<QTableWidget *>("");

    table->insertRow(table->currentRow() + 1);
    table->setCurrentCell(table->currentRow() + 1, 0);

    QFont font;
    font.setBold(true);

    QTableWidgetItem *tableWidgetItem0 = new QTableWidgetItem(Event);
    tableWidgetItem0->setFlags(Qt::NoItemFlags);
    tableWidgetItem0->setFont(font);
    table->setItem(table->currentRow(), 0, tableWidgetItem0);

    QTableWidgetItem *tableWidgetItem1 = new QTableWidgetItem(QString(topicName));
    tableWidgetItem1->setFlags(Qt::NoItemFlags);
    tableWidgetItem1->setFont(font);
    table->setItem(table->currentRow(), 1, tableWidgetItem1);

    QTableWidgetItem *tableWidgetItem2 = new QTableWidgetItem(msg);
    tableWidgetItem2->setFlags(Qt::NoItemFlags);
    tableWidgetItem2->setFont(font);
    table->setItem(table->currentRow(), 2, tableWidgetItem2);

    QTableWidgetItem *tableWidgetItem3 = new QTableWidgetItem(QDate::currentDate().toString(Qt::ISODate) +" "+QTime::currentTime().toString());
    tableWidgetItem3->setFlags(Qt::NoItemFlags);
    tableWidgetItem3->setFont(font);
    table->setItem(table->currentRow(), 3, tableWidgetItem3);
}

void MqttDevice::Authentifaction()
{
    tableAddInfo("Connected");

    if (m_authentification == false)
    {
        this->subscribe(*m_authSysTopic);
        tableAddInfo("Subscribed", m_authSysTopic->filter());

        QString infoFrame = m_imei+","+m_imsi+","+m_iccid+","+m_msisdn;
        this->publish(*m_authDeviceTopic, infoFrame.toUtf8());
        tableAddInfo("Published", m_authDeviceTopic->name(), infoFrame);
    }
    else
    {
        int period = m_sendFreq.digitValue() * 1000;
        deviceTimer->start(period);
        //connect(deviceTimer, SIGNAL(timeout()), this, SLOT(sendData()));
    }

    parent()->findChild<QPushButton *>("connectButton")->setEnabled(false);
    parent()->findChild<QPushButton *>("disconnectButton")->setEnabled(true);
    parent()->findChild<QLabel *>("deviceStatusLabel")->setText("Connected");
    parent()->findChild<QLineEdit *>("brokerLineEdit")->setEnabled(false);
    parent()->findChild<QLineEdit *>("portLineEdit")->setEnabled(false);

    MainWindow *myMainWindow = (MainWindow*)parent()->parent()->parent()->parent()->parent();

    myMainWindow->findChild<QAction *>("m_actionConfig")->setEnabled(false);
    myMainWindow->findChild<QAction *>("m_actionToolBarConfig")->setEnabled(false);

    if (myMainWindow->getConectAllClicked() == true)
    {
        myMainWindow->findChild<QAction *>("m_actionToolBarConnect")->setEnabled(false);
        myMainWindow->findChild<QAction *>("m_actionToolBarDisconnect")->setEnabled(true);
        myMainWindow->findChild<QAction *>("m_actionConnectAll")->setEnabled(false);
        myMainWindow->findChild<QAction *>("m_actionDisconnectAll")->setEnabled(true);
    }
}

void MqttDevice::receivedMsgHandle(const QByteArray &message, const QMqttTopicName &topic)
{
    QString msgReceiv (message);

    tableAddInfo("Received", topic.name(), msgReceiv);

    if (topic.name() == m_authSysTopic->filter())
    {
        if (msgReceiv[0] == '0')
        {
            m_authentification = true;
            m_cliendId = msgReceiv.mid(1,4);
            m_deviceId = msgReceiv.mid(5,5);
            m_sendFreq = msgReceiv[10];

            m_dataTopic->setName("data/"+ m_cliendId + "/" + m_deviceId);// = new QMqttTopicName("data/"+ m_cliendId + "/" + m_deviceId);
            m_statusTopic->setName("status/"+ m_cliendId + "/" + m_deviceId);// = new QMqttTopicName("status/"+ m_cliendId + "/" + m_deviceId);
            m_commandTopic->setFilter("command/" + m_cliendId + "/" + m_deviceId);// = new QMqttTopicFilter("command/" + m_cliendId + "/" + m_deviceId);

            this->subscribe(*m_commandTopic);

            tableAddInfo("Subscribe", m_commandTopic->filter());

            topicTablebWidgetItemVector.append(new QTableWidgetItem(QString(m_dataTopic->name())));
            topicTablebWidgetItemVector.append(new QTableWidgetItem(QString(m_commandTopic->filter())));
            topicTablebWidgetItemVector.append(new QTableWidgetItem(QString(m_statusTopic->name())));

            if (!fileTrajects->open(QIODevice::ReadOnly | QIODevice::Text))
                qDebug()<<"Can't open the resource file: trajects.txt"<<Qt::endl;
            QString line;

            do{
            line = textStreamTrajects->readLine();
            shapeId = line.mid(0, 8);
            }
            while( shapeIdVector.indexOf(shapeId) != -1);

            posFirstLine = textStreamTrajects->pos();
            shapeIdVector.append(shapeId);
            *firstLine = line;

            if (m_sendFreq.digitValue()> 0 && m_sendFreq.digitValue() < 3600)
                deviceTimer->start(m_sendFreq.digitValue() * 1000);
            connect(deviceTimer, SIGNAL(timeout()), this, SLOT(sendData()));
        }
        else
            m_authentification = false;

    }
    if(topic.name() == m_commandTopic->filter())
    {
        m_sendFreq = msgReceiv[0];

        if (m_sendFreq.digitValue() == 0)
           deviceTimer->stop();
        if (m_sendFreq.digitValue()> 0 && m_sendFreq.digitValue() < 3600)
        {
            deviceTimer->stop();
            deviceTimer->setInterval(m_sendFreq.digitValue() * 1000);
            deviceTimer->start();
        }
    }
}

void MqttDevice::sendData()
{
    QString latitude, longitude, time, gpsFrame;
    QString line;

    line = textStreamTrajects->readLine();

    if( line.mid(0, 8) != shapeId)
    {
        textStreamTrajects->seek(posFirstLine);
        line = textStreamTrajects->readLine();
    }

    gpsFrame = QTime::currentTime().toString()+ "," +line.mid(9, 18);

    this->publish(*m_dataTopic, gpsFrame.toUtf8());
    tableAddInfo("Publish", m_dataTopic->name(), gpsFrame);
}

void MqttDevice::deviceDisconnect()
{
    bool timeract = deviceTimer->isActive();

    if (timeract)
        deviceTimer->stop();
    tableAddInfo("Disconnected");

    parent()->findChild<QPushButton *>("connectButton")->setEnabled(true);
    parent()->findChild<QPushButton *>("disconnectButton")->setEnabled(false);
    parent()->findChild<QLabel *>("deviceStatusLabel")->setText("Disconnected");
    //parent()->findChild<QLineEdit *>("brokerLineEdit")->setEnabled(true);
    //parent()->findChild<QLineEdit *>("portLineEdit")->setEnabled(true);

    MainWindow *myMainWindow = (MainWindow*)parent()->parent()->parent()->parent()->parent();

    myMainWindow->findChild<QAction *>("m_actionConfig")->setEnabled(true);
    myMainWindow->findChild<QAction *>("m_actionToolBarConfig")->setEnabled(true);

    myMainWindow->getDisconectAllClicked();


    if (myMainWindow->getDisconectAllClicked() == true)
    {
        myMainWindow->findChild<QAction *>("m_actionConnectAll")->setEnabled(true);
        myMainWindow->findChild<QAction *>("m_actionToolBarConnect")->setEnabled(true);
        myMainWindow->findChild<QAction *>("m_actionDisconnectAll")->setEnabled(false);
        myMainWindow->findChild<QAction *>("m_actionToolBarDisconnect")->setEnabled(false);
    }
}




