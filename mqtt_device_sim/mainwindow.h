#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mqttdevice.h"
#include "aboutdialog.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QProgressBar>
#include <QVector>

#include <QTableWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QSize>
#include <QCommonStyle>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QFormLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void adjustMainWindowGeometry(int widthPrec, int heightPrec);
    void creatTooBar(int iconW, int iconH);
    void creatMenus(void);
    void creatActions(void);
    void creatCentralWidget();
    void creatDockWidget();
    bool getConectAllClicked();
    bool getDisconectAllClicked();

private:
    QStackedWidget *m_centralStackedWidget;
    QVector<QVBoxLayout*> m_infoWidgetLayoutVector;
    QVector<QFormLayout*> m_tableWidgetLayoutVector;
    QVector<QTabWidget*>  m_tabWidgetVector;
    QVector<QWidget*> m_optionWidgetVector;
    QVector<QProgressBar*> m_progressBarVector;
    QVector<QTableWidget*> m_tableWidgetVector;
    QVector<MqttDevice*> m_mqttDeviceVector;

    QDockWidget *m_dockWidget;
    QListWidget *m_listWidget;
    QToolBar *m_toolBar;
    QMenuBar *m_menuBar;
    QMenu *m_menuFile, *m_menuTools, *m_menuHelp;
    QAction *m_actionNewConnexion, *m_actionQuit, *m_actionConfig, *m_actionConnectAll, *m_actionDisconnectAll, *m_actionAbout;
    QAction *m_actionToolBarNew, *m_actionToolBarConfig, *m_actionToolBarConnect, *m_actionToolBarDisconnect;

    AboutDialog *aboutWin;

    bool connectAllClicked;
    bool disconnectAllClicked;

    unsigned long long startImei, startImsi, startIccid, startMsisdn;

    void connectDeviceIndex(int deviceIndex);
    void disconnectDeviceIndex(int deviceIndex);

public slots:
    void showDeviceTabData(QListWidgetItem *item );
    void showDeviceTabInfo(QListWidgetItem *item );
    void addDevice();
    void connectDevice();
    void disconnectDevice();
    void errorRaised();
};

#endif // MAINWINDOW_H
