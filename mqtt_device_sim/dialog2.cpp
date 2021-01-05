#include "dialog2.h"
#include "ui_dialog2.h"

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);

    connect(ui->connectButton, SIGNAL(clicked()), this->parent()->parent(), SLOT(connectDevice()));
    connect(ui->disconnectButton, SIGNAL(clicked()), this->parent()->parent(), SLOT(disconnectDevice()));
}

Dialog2::~Dialog2()
{
    delete ui;
}
