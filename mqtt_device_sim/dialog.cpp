#include "dialog.h"
#include "ui_dialog.h"

#include<QAction>
#include<QMainWindow>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QStringList numberDevices;

    for (int i = 1; i <= 10; i++)
        numberDevices<<QString::number(i);

    ui->comboBoxNumDev->addItems(numberDevices);
    ui->comboBoxConnMeth->addItems(QStringList()<<"Auto");

    QAction* actionConfigIcon = this->parent()->findChild<QAction*>("m_actionToolBarConfig");
    QAction* actionConfigMenu = this->parent()->findChild<QAction*>("m_actionConfig");

    connect(actionConfigIcon, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionConfigMenu, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->buttonBox, SIGNAL(accepted()), this->parent(), SLOT(addDevice()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::openConfigWindow()
{
    /*QString sender = this->sender()->objectName();

    if (sender == "m_actionToolBarNew" || sender == "m_actionNewConnexion")
    {
        ui->comboBoxNumDev->addItems(QStringList()<<"1");
        ui->comboBoxNumDev->setEnabled(false);
    }

    if (sender == "m_actionToolBarConfig" || sender == "m_actionConfig")
    {
        ui->comboBoxNumDev->addItems(QStringList()<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10");
        ui->comboBoxNumDev->setEnabled(true);
    }
    ui->comboBoxConnMeth->addItems(QStringList()<<"Auto"<<"Manual");
    this->open();*/

}
