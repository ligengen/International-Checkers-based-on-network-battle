#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
#include <QDebug>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QtNetwork>
#include <QMessageBox>
#include <QSound>
///棋盘数字等于i*5+(j+2)/2
/// srand((unsigned)time(0));
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    color = 1;
    ui->setupUi(this);

    ui->lineEdit->setDisabled(1);
    ui->lineEdit_2->setDisabled(1);

    this->setWindowTitle("Checkers 10x10");
    checkers = new Checkerboard(color+1);
    ui->gridLayout->addWidget(checkers,0,0);

    //connect(this,SIGNAL(textChanged(QString)),this,SLOT(tellClientIP(QString)));
    connect(checkers,SIGNAL(lose()),this,SLOT(lose()));
    connect(checkers->draughts,SIGNAL(oneStep(QString)),this,SLOT(sendMessage(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    inputIPDialog w(this);
    connect(&w, SIGNAL(getIP(QString,quint16)), this, SLOT(connectHost(QString,quint16)));
    w.exec();
}

void MainWindow::connectHost(QString str,quint16 port)
{
    this->readWriteSocket = new QTcpSocket;
    qDebug() << port <<endl;
    this->readWriteSocket->connectToHost(QHostAddress(str),port);
    QObject::connect(this->readWriteSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));
    //startgame();
}

void MainWindow::recvMessage()
{
    QString info;
    info = this->readWriteSocket->readAll();
    //addopponent(info);
    if(info[info.size()-1] == '^')
    {
        win();
    }
    bool flag = 0;
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == '!')
        {
            flag = 1;
            break;
        }
    }
    bool flgg = 0;
    qDebug() << info << endl;
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == ')')
        {
            flgg = 1;
            qDebug(")");
            break;
        }
    }
    bool flagggg = 0;
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == '=')
        {
            flagggg = 1;
            int ret = QMessageBox::information(0,"TIE","Tie",QMessageBox::Yes);
            if(ret == QMessageBox::Yes)
                exit(0);
        }
    }
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == '$')
        {
            int ret = QMessageBox::information(0,"Sorry","Your opponent refused your request",QMessageBox::Yes);
        }
    }
    if(!flag)
    {
        checkers->recv(info);
    }
    else if(flag)
    {
        int ret = QMessageBox::information(0,"Congratulations","Your opponent had given up. \n You won!",QMessageBox::Yes);
        if(ret == QMessageBox::Yes)
            exit(0);
    }
    if(flgg)
    {
        int ret = QMessageBox::information(0,"Ask for a tie","Your opponent is asking for a tie. Do you agree?",
                                           QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(ret == QMessageBox::Yes)
        {
            writ("(");
            int ret = QMessageBox::information(0,"TIE","Tie",QMessageBox::Yes);
            if(ret == QMessageBox::Yes)
                exit(0);
        }
        if(ret == QMessageBox::No)
        {
            writ("+");
        }
    }
}

void MainWindow::sendMessage(QString info)
{
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(info);
    this->readWriteSocket->write(array->data());
}

void MainWindow::on_pushButton_2_clicked()
{
    QString info = "!";
    QByteArray* array = new QByteArray;
    array->clear();
    array->append(info);
    this->readWriteSocket->write(array->data());
    array->clear();

    int ret = QMessageBox::information(NULL,"Sorry...","You've admitted yourself beaten.",QMessageBox::Yes);
    if(ret == QMessageBox::Yes)
        exit(0);
}

void MainWindow::writ(QString ord)
{
    qDebug() << ord;
    QByteArray array;
    array.clear();
    array.append(ord);
    readWriteSocket->write(array.data());
}

void MainWindow::on_pushButton_clicked()
{
    int ret = QMessageBox::information(NULL,"Ask for a tie.","Please wait your opponent's decision",
                                       QMessageBox::Yes);
    writ("@");
}

void MainWindow::lose()
{
    writ("^");
    int ret = QMessageBox::information(NULL,"Lose","You lose the game!",QMessageBox::Yes);
    if(ret == QMessageBox::Yes)
        exit(0);
}

void MainWindow::win()
{
    int ret = QMessageBox::information(NULL,"Win","You win the game!",QMessageBox::Yes);
    if(ret == QMessageBox::Yes)
    {
        exit(0);
    }
}
