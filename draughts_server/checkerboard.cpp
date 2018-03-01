#include <QDebug>
#include <fstream>
#include "checkerboard.h"
///棋盘数字等于i*5+(j+2)/2
Checkerboard::Checkerboard(int who,QWidget *parent) : QWidget(parent),color(who-1)
{
    draughts = new Draughts(color);
    QGridLayout* gridlayout = new QGridLayout(this);
    gridlayout->setSpacing(1);
    button = new Chess**[10];

    std::ifstream fin;
    fin.open("/Users/ligen/Desktop/QT/draughts_server/b.txt");
    if(!fin)
    {
        qDebug()<< "fin not success!" <<endl;
    }
    int s[10][10];
    while(!fin.eof())
    {
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                fin >> s[i][j];
            }
        }
    }
    fin.close();

    for(int i=0;i<10;i++)
    {
        button[i] = new Chess*[10];
        for(int j=0;j<10;j++)
        {
            this->who = who;
            button[i][j] = new Chess(i,j,who,this);
            button[i][j]->setStatusValue(s[i][j]);
            update();
            gridlayout->addWidget(button[i][j],i,j);
        }
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            connect(this,SIGNAL(_whoColor(int)),draughts,SLOT(getColor(int)));
            connect(button[i][j],SIGNAL(chosen_one(int,int)),this,SLOT(setDisable(int,int)));
            connect(button[i][j],SIGNAL(chosen_one(int,int)),draughts,SLOT(getInfo(int,int)));
            connect(button[i][j],SIGNAL(chosen_one(int,int)),this,SLOT(clicked(int,int)));
        }
    }
    connect(draughts,SIGNAL(_lose()),this,SIGNAL(lose()));
    connect(draughts,SIGNAL(readyView()),this,SLOT(upd()));
    connect(draughts,SIGNAL(oneStep(QString)),this,SIGNAL(writ(QString)));

}

void Checkerboard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(116,52,129)),5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter.drawRect(3,3,width()-5,height()-5);
}

void Checkerboard::whoColor(int a)
{
    color = a;
    emit _whoColor(a);
}

void Checkerboard::setDisable(int a,int b)
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(i==a&&j==b)
                continue;
            button[i][j]->chosen = 0;
            update();
        }
    }
}
void Checkerboard::upd()
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            button[i][j]->setStatus(draughts->_Chess(i,j));
            button[i][j]->setHighlight(0);
            button[i][j]->update();
        }
    }
    std::vector<int> tmp = draughts->nextPos();
    int size = tmp.size();
    for(int i=0;i<size;i++)
        button[tmp[i]/10][tmp[i]%10]->setHighlight(2);
    if(draughts->nowPos() != -1)
        button[draughts->nowPos()/10][draughts->nowPos()%10]->setHighlight(1);
}

void Checkerboard::clicked(int a, int b)
{
    draughts->chosen(a*10+b);
}

void Checkerboard::recv(QString ord)
{
    draughts->startStep(ord);
}
