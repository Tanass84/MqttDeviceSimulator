#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include<QPicture>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("About MQTT Device Similulator");
    this->setFixedSize(300, 130);
    this->setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    QPixmap logo;
    logo.load("../resources/application.png");
    logo = logo.scaled(60,60);
    ui->logoLabel->setPixmap(logo);
}

AboutDialog::~AboutDialog()
{
    delete ui;

}
