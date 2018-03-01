#include "buildconnectiondialog.h"
#include "ui_buildconnectiondialog.h"
#include <QPushButton>
#include <QSignalMapper>

buildConnectionDialog::buildConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::buildConnectionDialog)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(0);
}

buildConnectionDialog::~buildConnectionDialog()
{
    delete ui;
}

void buildConnectionDialog::on_pushButton_clicked()
{
    close();
}

void buildConnectionDialog::updateButton()
{
    ui->pushButton->setEnabled(1);
}

void buildConnectionDialog::on_pushButton_2_clicked()
{
    emit tryToCloseServer();
    close();
}

void buildConnectionDialog::setTitle(QString str) {
    ui->lineEdit->setText(str);
}
