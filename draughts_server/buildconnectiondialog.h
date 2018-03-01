#ifndef BUILDCONNECTIONDIALOG_H
#define BUILDCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class buildConnectionDialog;
}

class buildConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit buildConnectionDialog(QWidget *parent = 0);
    ~buildConnectionDialog();
    void setTitle(QString);

signals:
    void buildConnection();
    void tryToCloseServer();
    void sendPort(int);
   // void setPort(int);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public slots:
    void updateButton();

private:
    Ui::buildConnectionDialog *ui;
};

#endif // BUILDCONNECTIONDIALOG_H
