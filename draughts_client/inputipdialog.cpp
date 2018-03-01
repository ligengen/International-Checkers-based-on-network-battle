#include "inputipdialog.h"
#include "ui_inputipdialog.h"
#include <QKeyEvent>

inputIPDialog::inputIPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputIPDialog)
{
    ui->setupUi(this);
}

inputIPDialog::~inputIPDialog()
{
    delete ui;
}

void inputIPDialog::on_pushButton_ok_clicked()
{
   QString str_1 = ui->lineEdit->text();
   QString str_2 = ui->lineEdit_2->text();
   quint16 ss = str_2.toUInt();
   emit getIP(str_1,ss);
   close();
}
