#ifndef INPUTIPDIALOG_H
#define INPUTIPDIALOG_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
class inputIPDialog;
}

class inputIPDialog : public QDialog
{
    Q_OBJECT

signals:
    void getIP(QString,quint16);
public:
    explicit inputIPDialog(QWidget *parent = 0);
    ~inputIPDialog();
    QString str;

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::inputIPDialog *ui;
};

#endif // INPUTIPDIALOG_H
