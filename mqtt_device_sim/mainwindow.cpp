#include "mainwindow.h"
#include "dialog.h"
#include "dialog2.h"
#include "mqttdevice.h"


#include<QStackedLayout>
#include<QBoxLayout>
#include<QFormLayout>
#include<QLineEdit>
#include<QProgressBar>
#include<QTableView>
#include<QListWidgetItem>
#include<QHeaderView>
#include<QComboBox>
#include<QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setObjectName("myMainWindow");
    //param init
    startImei = 865789020999218;
    startImsi = 206300001768584;
    startIccid = 11323000000176858384;
    startMsisdn = 32456173516;

    //Initialisation
    setWindowTitle("MQTT Device Simulator");
    adjustMainWindowGeometry(70,65);
    QIcon iconDevice("../resources/application.png");
    setWindowIcon(iconDevice);

    creatCentralWidget();
    creatDockWidget();
    creatTooBar(20, 20);
    creatMenus();
    creatActions();

    aboutWin = new AboutDialog(this);

    //Connections:
    connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem* )), this, SLOT(showDeviceTabData(QListWidgetItem* )));
    connect(m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem* )), this, SLOT(showDeviceTabInfo(QListWidgetItem* )));
    connect(m_actionToolBarNew, SIGNAL(triggered()), this, SLOT(addDevice()));
    connect(m_actionNewConnexion, SIGNAL(triggered()), this, SLOT(addDevice()));

    connect(m_actionToolBarConnect, SIGNAL(triggered()), this, SLOT(connectDevice()));
    connect(m_actionConnectAll, SIGNAL(triggered()), this, SLOT(connectDevice()));

    connect(m_actionToolBarDisconnect, SIGNAL(triggered()), this, SLOT(disconnectDevice()));
    connect(m_actionDisconnectAll, SIGNAL(triggered()), this, SLOT(disconnectDevice()));

    connect(m_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_actionAbout, SIGNAL(triggered()), aboutWin, SLOT(open()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::adjustMainWindowGeometry(int widthPrec, int heightPrec)
{
    int x, y, w, h;

    QScreen *screen = QGuiApplication::primaryScreen();

    int W = screen->size().width();
    int H = screen->size().height();

    w = (widthPrec/100.0)*W;
    h = (heightPrec/100.0)*H;

    x = (W - w)/2;
    y = (H - h)/2;

    setGeometry(x, y, w, h);
}
void MainWindow::creatCentralWidget()
{
    m_centralStackedWidget = new QStackedWidget;
    setCentralWidget(m_centralStackedWidget);
}

void MainWindow::creatDockWidget()
{
    m_dockWidget = new QDockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockWidget);
    m_dockWidget->setWindowTitle("Device list");
    m_dockWidget->setFeatures(QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
    m_dockWidget->setMaximumWidth(150);

    m_listWidget = new QListWidget;
    m_dockWidget->setWidget(m_listWidget);
}

bool MainWindow::getConectAllClicked()
{
    return  connectAllClicked;
}

bool MainWindow::getDisconectAllClicked()
{
    return  disconnectAllClicked;
}
void MainWindow::connectDeviceIndex(int deviceIndex)
{
    int i = deviceIndex;

    if (m_mqttDeviceVector[i]->state() == QMqttClient::Disconnected)
    {
    QString mqttBroker = m_optionWidgetVector[i]->findChild<QLineEdit *>("brokerLineEdit")->text();
    QString mqttPort = m_optionWidgetVector[i]->findChild<QLineEdit *>("portLineEdit")->text();

    m_mqttDeviceVector[i]->setHostname(mqttBroker);
    m_mqttDeviceVector[i]->setPort(mqttPort.toInt());

    if(!m_mqttDeviceVector[i]->isAuthentified())
    {
        m_mqttDeviceVector[i]->setParam(QString("Device%1").arg(i + 1), QString::number(startImei + i), QString::number(startImsi + i), QString::number(startIccid + i), QString::number(startMsisdn + i));
        m_mqttDeviceVector[i]->setClientId(QString("Device%1").arg(i + 1));
        m_mqttDeviceVector[i]->setKeepAlive(60);
        m_mqttDeviceVector[i]->setCleanSession(true);
        m_mqttDeviceVector[i]->setProtocolVersion(QMqttClient::MQTT_3_1_1);
    }

    m_mqttDeviceVector[i]->connectToHost();
    }
}

void MainWindow::disconnectDeviceIndex(int deviceIndex)
{
    int i = deviceIndex;

    if (m_mqttDeviceVector[i]->state() == QMqttClient::Connected)
    {
        m_mqttDeviceVector[i]->disconnectFromHost();
    }
}

void MainWindow::creatTooBar(int iconW, int iconH)
{
    m_toolBar =  addToolBar("Tool Bar");

    QSize toolBarIconSize(iconW, iconH);
    m_toolBar->setIconSize(toolBarIconSize);

    QIcon iconNew("../resources/new.png");
    QIcon iconConfig("../resources/config.png");
    QIcon iconConnect("../resources/connect.png");
    QIcon iconDisconnect("../resources/disconnect.png");

    m_actionToolBarNew = m_toolBar->addAction(iconNew, "New Connexion");
    m_actionToolBarNew->setObjectName(QString::fromUtf8("m_actionToolBarNew"));
    m_actionToolBarNew->setEnabled(false);

    m_actionToolBarConfig = m_toolBar->addAction(iconConfig, "Configuration");
    m_actionToolBarConfig->setObjectName(QString::fromUtf8("m_actionToolBarConfig"));

    m_actionToolBarConnect = m_toolBar->addAction(iconConnect, "Connect All");
    m_actionToolBarConnect->setObjectName(QString::fromUtf8("m_actionToolBarConnect"));
    m_actionToolBarConnect->setEnabled(false);

    m_actionToolBarDisconnect = m_toolBar->addAction(iconDisconnect, "Disconnect All");
    m_actionToolBarDisconnect->setObjectName(QString::fromUtf8("m_actionToolBarDisconnect"));
    m_actionToolBarDisconnect->setEnabled(false);

}

void MainWindow::creatMenus(void)
{
    m_menuBar = new QMenuBar(this);
    setMenuBar(m_menuBar);
    m_menuFile = m_menuBar->addMenu("File");
    m_menuTools = m_menuBar->addMenu("Tools");
    m_menuHelp = m_menuBar->addMenu("Help");
}

void MainWindow::creatActions(void)
{
    m_actionNewConnexion = m_menuFile->addAction("New Connexion");
    m_actionNewConnexion->setObjectName(QString::fromUtf8("m_actionNewConnexion"));
    m_actionNewConnexion->setEnabled(false);
    m_actionQuit = m_menuFile->addAction("Quit");

    m_actionConfig = m_menuTools->addAction("Configuration");
    m_actionConfig->setObjectName(QString::fromUtf8("m_actionConfig"));

    m_actionConnectAll = m_menuTools->addAction("Connect All");
    m_actionConnectAll->setObjectName(QString::fromUtf8("m_actionConnectAll"));
    m_actionConnectAll->setEnabled(false);

    m_actionDisconnectAll = m_menuTools->addAction("Disconnect All");
    m_actionDisconnectAll->setObjectName(QString::fromUtf8("m_actionDisconnectAll"));
    m_actionDisconnectAll->setEnabled(false);

    m_actionAbout = m_menuHelp->addAction("About");
}

//Slots:
void MainWindow::showDeviceTabData(QListWidgetItem *item)
{
    m_centralStackedWidget->setCurrentIndex(m_listWidget->row(item));
    m_tabWidgetVector[m_listWidget->row(item)]->setCurrentIndex(0);
}

void MainWindow::showDeviceTabInfo(QListWidgetItem *item)
{
    m_centralStackedWidget->setCurrentIndex(m_listWidget->row(item));
    m_tabWidgetVector[m_listWidget->row(item)]->setCurrentIndex(1);
}

void MainWindow::addDevice()
{
    QString senderName = this->sender()->objectName();

    int numberDeviceConnected = m_centralStackedWidget->count();
    int deviceNumber = 0;

    QString brokerDomainName = this->findChild<QLineEdit *>("brokerLineEdit")->text();
    QString port = this->findChild<QLineEdit *>("PortLineEdit")->text();

    if (senderName == "buttonBox")
    {
        delete m_centralStackedWidget;

        //m_infoWidgetLayoutVector.clear();
        m_tableWidgetLayoutVector.clear();
        m_tabWidgetVector.clear();
        m_optionWidgetVector.clear();
        m_progressBarVector.clear();
        m_tableWidgetVector.clear();

        m_listWidget->clear();
        m_mqttDeviceVector.clear();

        creatCentralWidget();//creat new cetral widget

        numberDeviceConnected = 0;
        deviceNumber = (this->findChild<QComboBox *>("comboBoxNumDev")->currentIndex() )+ 1;

        m_actionNewConnexion->setEnabled(true);
        m_actionToolBarNew->setEnabled(true);
        m_actionToolBarConnect->setEnabled(true);

        if (brokerDomainName == "")
        {
            QMessageBox::warning( this, "Warning", "MQTT Broker domaine name is required!" );
            m_actionToolBarConfig->triggered();
            return;
        }

        if (port.toInt() > 65535 ||port.toInt() < 1)
        {
            QMessageBox::warning( this, "Warning", "MQTT Port should be between 1 and 65535!" );
            m_actionToolBarConfig->triggered();
        }
    }

    if (senderName == "m_actionToolBarNew" || senderName == "m_actionNewConnexion")
    {
        deviceNumber = 1;
    }

    for (int i = numberDeviceConnected; i < numberDeviceConnected + deviceNumber; i++)
    {
        m_tabWidgetVector.append(new QTabWidget(this));//
        m_tableWidgetVector.append(new QTableWidget(0, 4, this));//
        //m_tableWidgetVector[i]->setColumnCount(4);
        //m_infoWidgetVector.append(new QWidget);//
        m_optionWidgetVector.append(new Dialog2(m_tabWidgetVector[i]));//
        m_tableWidgetLayoutVector.append(new QFormLayout(m_tableWidgetVector[i]));//
        //m_infoWidgetLayoutVector.append(new QVBoxLayout);//
        //m_progressBarVector.append (new QProgressBar);//

        QStringList headerTableList = (QStringList() << "Event" << "Topic" << "Message" << "Time");
        m_tableWidgetVector[i]->verticalHeader()->setVisible(false);
        m_tableWidgetVector[i]->setHorizontalHeaderLabels(headerTableList);
        //m_tableWidgetVector[i]->horizontalHeader()->setDefaultSectionSize(200);
        m_tableWidgetVector[i]->horizontalHeader()->resizeSection(0, 100);
        m_tableWidgetVector[i]->horizontalHeader()->resizeSection(1, 180);
        m_tableWidgetVector[i]->horizontalHeader()->resizeSection(2, 370);
        m_tableWidgetVector[i]->horizontalHeader()->resizeSection(3, 130);
        m_tableWidgetVector[i]->horizontalHeader()->setEnabled(false);

        /*QTableWidgetItem *headerItem0 = new QTableWidgetItem("Event");
        QTableWidgetItem *headerItem1 = new QTableWidgetItem("Topic");
        QTableWidgetItem *headerItem2 = new QTableWidgetItem("Message");
        QTableWidgetItem *headerItem3 = new QTableWidgetItem("Time");

        headerItem0->setFlags(Qt::NoItemFlags);
        headerItem1->setFlags(Qt::NoItemFlags);
        headerItem2->setFlags(Qt::NoItemFlags);
        headerItem3->setFlags(Qt::NoItemFlags);

        Qt::ItemFlags flag =  headerItem0->flags();

        m_tableWidgetVector[i]->setHorizontalHeaderItem(0, headerItem0);
        m_tableWidgetVector[i]->setHorizontalHeaderItem(1, headerItem1);
        m_tableWidgetVector[i]->setHorizontalHeaderItem(2, headerItem2);
        m_tableWidgetVector[i]->setHorizontalHeaderItem(3, headerItem3);



        Qt::ItemFlags flag2 =  headerItem0->flags();

        m_tableWidgetVector[i]->horizontalHeaderItem(0)->setSizeHint(QSize(100, 30));//QSize(500, 30)
        QSize size;//(500, 30)
        size = m_tableWidgetVector[i]->horizontalHeaderItem(0)->sizeHint();//QSize(500, 30)*/
        m_tableWidgetVector[i]->setSelectionMode(QAbstractItemView::NoSelection);

        m_tabWidgetVector[i]->insertTab(0, m_tableWidgetVector[i], "Data");
        m_tabWidgetVector[i]->insertTab(1, m_optionWidgetVector[i], "Option");

        m_tableWidgetVector[i]->setLayout(m_tableWidgetLayoutVector[i]);
        m_tableWidgetVector[i]->setCurrentCell(1, 0);

        m_optionWidgetVector[i]->findChild<QLineEdit*>("brokerLineEdit")->setText(brokerDomainName);
        m_optionWidgetVector[i]->findChild<QLineEdit*>("portLineEdit")->setText(port);

        m_centralStackedWidget->addWidget(m_tabWidgetVector[i]);

        m_listWidget->addItem(QString("     Device %1").arg(i + 1));
        QIcon iconDevice ("../resources/device.png");
        m_listWidget->item(i)->setIcon(iconDevice);

        m_mqttDeviceVector.append(new MqttDevice(m_optionWidgetVector[i]));

        connect(m_mqttDeviceVector[i], SIGNAL(errorChanged(ClientError )), this, SLOT(errorRaised()));
    }
}

void MainWindow::connectDevice()
{
    QString senderName = this->sender()->objectName();
    int numberDeviceToConnect;

    if (senderName == "m_actionToolBarConnect" || senderName == "m_actionConnectAll")
    {
         numberDeviceToConnect = m_centralStackedWidget->count();

         connectAllClicked = true;
         disconnectAllClicked = false;

         for (int i = 0; i <  numberDeviceToConnect; i++)
         {
            connectDeviceIndex(i);
         }
    }

    if (senderName == "connectButton")
    {
        numberDeviceToConnect = 1;
        QPushButton* pushButtonConnect = (QPushButton*)this->sender();

        MqttDevice *mqttd = (MqttDevice*)pushButtonConnect->parent()->findChild<MqttDevice *>("");

        connectDeviceIndex(m_mqttDeviceVector.indexOf(mqttd));
    }
}

void MainWindow::disconnectDevice()
{
    QString senderName = this->sender()->objectName();
    int numberDeviceToDisconnect;
    int numberDeviceConnected;

    if (senderName == "m_actionToolBarDisconnect" || senderName == "m_actionDisconnectAll")
    {
         numberDeviceToDisconnect = m_centralStackedWidget->count();
         numberDeviceConnected = m_mqttDeviceVector.size();

         disconnectAllClicked = true;
         connectAllClicked = false;

         for (int i = 0; i < numberDeviceConnected; i++)
         {
             disconnectDeviceIndex(i);
         }
    }

    if (senderName == "disconnectButton")
    {
        numberDeviceToDisconnect = 1;
        QPushButton* pushButtonDisconnect = (QPushButton*)this->sender();
        MqttDevice *mqttd = (MqttDevice*)pushButtonDisconnect->parent()->findChild<MqttDevice *>("");

        int pos = m_mqttDeviceVector.indexOf(mqttd);

        disconnectDeviceIndex(pos);
    }
}

void MainWindow::errorRaised()
{
    MqttDevice *device = (MqttDevice*)this->sender();

    QMqttClient::ClientError mqttError = device->error();

    switch (mqttError)
    {
        case QMqttClient::InvalidProtocolVersion :
        case QMqttClient::TransportInvalid:
        QMessageBox::warning(this, "Warning", "MQTT Host name or Protocole invalid!" );
        break;

        default:
        QMessageBox::warning(this, "Warning", "Connection Error!" );
        break;

    }
}


