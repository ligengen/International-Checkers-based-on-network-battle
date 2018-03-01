#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "checkerboard.h"
#include "chess.h"
#include <QTcpServer>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QStackedLayout>
#include <QTextEdit>
#include <QLabel>
#include "inputipdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void writ(QString);


signals:

public slots:
    void on_pushButton_3_clicked();
    void sendMessage(QString);
    void recvMessage();
    void lose();
    void win();
    void connectHost(QString str,quint16 port);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Checkerboard* checkers;
    int color;
    int who;
    QTcpSocket* readWriteSocket;
};

#endif // MAINWINDOW_H
