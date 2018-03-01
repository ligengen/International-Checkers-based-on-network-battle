#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
#include <QDebug>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QtNetwork>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    color = 0;
    ui->setupUi(this);

    ui->lineEdit->setDisabled(1);
    ui->lineEdit_2->setDisabled(1);

    this->setWindowTitle("Checkers 10x10");
    checkers = new Checkerboard(color+1);
    ui->gridLayout->addWidget(checkers,0,0);
    connect(checkers,SIGNAL(lose()),this,SLOT(lose()));
    connect(ui->actionInput_Port,SIGNAL(triggered(bool)),this,SLOT(inputPort()));
    connect(checkers->draughts,SIGNAL(oneStep(QString)),this,SLOT(sendMessage(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    buildConnectionDialog w(this);
    QString str = initServer(&w);
    w.setTitle(str);
    QObject::connect(&w, SIGNAL(tryToCloseServer()), this, SLOT(closeServer()));
    w.exec();

}

void MainWindow::closeServer()
{
    this->listenSocket->close();
}

QString MainWindow::initServer(buildConnectionDialog* w)
{
    this->listenSocket = new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,port);
    qDebug()<< port <<endl;
    QObject::connect(this->listenSocket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    QObject::connect(this->listenSocket, SIGNAL(newConnection()), w, SLOT(updateButton()));

    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach (QHostAddress address,info.addresses())
    {
         if(address.protocol() == QAbstractSocket::IPv4Protocol)
         {
             //ui->myIP->setText(address.toString());
             return address.toString();
         }
    }
    return " ";
}

void MainWindow::acceptConnection()
{
    //ui->label->gameStart();。。。需要完成???好像不需要  ><
    this->readWriteSocket = this->listenSocket->nextPendingConnection();
    QString str = this->readWriteSocket->peerAddress().toString();
    while (str[0] >= '9' || str[0] <= '0') str.remove(0, 1);
    QObject::connect(this->readWriteSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));
}

void MainWindow::recvMessage()
{
    QString info;
    info = this->readWriteSocket->readAll();
   // ui->label->addOppo(info);
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
    bool flagg = 0;
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == '(')
        {
            flagg = 1;
            int ret = QMessageBox::information(0,"TIE","Tie",QMessageBox::Yes);
            if(ret == QMessageBox::Yes)
                exit(0);
        }
    }
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == '+')
        {
            int ret = QMessageBox::information(0,"Sorry","Your opponent refused your request",QMessageBox::Yes);
        }
    }
    bool flaggg = 0;
    for(int i=0;i<info.toStdString().size();i++)
    {
        if(info.toStdString()[i] == '@')
        {
            flaggg = 1;
            break;
        }
    }
    if(!flag)
    {
        checkers->recv(info);
    }
    if(flag)
    {
        int ret = QMessageBox::information(0,"Congratulations","Your opponent had given up. \n You won!",QMessageBox::Yes);
        if(ret == QMessageBox::Yes)
            exit(0);
    }
    if(flaggg)
    {
        int ret = QMessageBox::information(0,"Ask for a tie","Your opponent is asking for a tie. Do you agree?",
                                           QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(ret == QMessageBox::Yes)
        {
            writ("=");
            int ret = QMessageBox::information(0,"TIE","Tie",QMessageBox::Yes);
            if(ret == QMessageBox::Yes)
                exit(0);
        }
        if(ret == QMessageBox::No)
        {
            writ("$");
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

void MainWindow::writ(QString ord)
{
    //qDebug() << ord;
    QByteArray array;
    array.clear();
    array.append(ord);
    readWriteSocket->write(array.data());
}

void MainWindow::on_pushButton_clicked()
{
    int ret = QMessageBox::information(NULL,"Ask for a tie.","Please wait your opponent's decision",
                                       QMessageBox::Yes);
    writ(")");
}

void MainWindow::inputPort()
{
    widget_ = new QWidget;

    edit_ = new QLineEdit;
    button_ = new QPushButton(tr("Confirm"));
    connect(button_,SIGNAL(clicked(bool)),this,SLOT(getPort()));
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(edit_);
    layout->addWidget(button_);
    widget_->setLayout(layout);
    widget_->show();
}

void MainWindow::getPort()
{
    auto i = edit_->text().toUInt();
    port = i;
    widget_->hide();
}
