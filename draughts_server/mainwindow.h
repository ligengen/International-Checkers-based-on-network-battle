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
#include "buildconnectiondialog.h"
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
    void acceptConnection();
    void sendMessage(QString);
    void closeServer();
    void recvMessage();
    void on_pushButton_3_clicked();
    void lose();
    void win();
    void on_pushButton_2_clicked();

private slots:
    void on_pushButton_clicked();
    void inputPort();
    void getPort();
private:
    Ui::MainWindow *ui;
    int port;
    QWidget* widget;
    Checkerboard* checkers;
    int color;
    int who;
    QString initServer(buildConnectionDialog*);
    QTcpServer* listenSocket;
    QTcpSocket* readWriteSocket;
    QPushButton* button_;
    QLineEdit* edit_;
    QWidget *widget_;
};

#endif // MAINWINDOW_H
