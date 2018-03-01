#include "Chess.h"
#include <QDebug>
///棋盘数字等于i*5+(j+2)/2
Chess::Chess(int _x, int _y,int who,QWidget *parent) : QWidget(parent)
  ,x(_x),y(_y),who(who)
{

}

void Chess::paintEvent(QPaintEvent *ev)
{
    //if (x == 0 && y == 1) qDebug() << status;
    QPainter p(this);
    if((x+y)%2 == 0)
    {
        p.setBrush(QBrush(QColor(255,245,238)));
        p.drawRect(0,0,1000,1000);

    }
    else
    {
        p.setBrush(QBrush(QColor(128,128,105)));
        p.drawRect(0,0,1000,1000);
    }
    if(status ==1)
    {
        QPixmap pp;
        pp.load("/Users/ligen/Desktop/QT/draughts_client/resource/black.png");
        p.drawPixmap(5,5,30,30,pp);
    }
    else if(status == 2)
    {
        QPixmap pp;
        pp.load("/Users/ligen/Desktop/QT/draughts_client/resource/white.png");
        p.drawPixmap(5,5,30,30,pp);
    }
    else if(status == -2)
    {
        QPixmap pp;
        pp.load("/Users/ligen/Desktop/QT/draughts_client/resource/WechatIMG11.png");
        p.drawPixmap(5,5,30,30,pp);
    }
    else if(status == -1)
    {
        QPixmap pp;
        pp.load("/Users/ligen/Desktop/QT/draughts_client/resource/black_queen.png");
        p.drawPixmap(5,5,30,30,pp);
    }
    if(chosen)
    {
        QPen ppp;
        ppp.setColor(QColor(0,255,255));
        ppp.setWidth(3);
        p.setPen(ppp);
        p.drawLine(1,0,1,40);
        p.drawLine(1,40,41,40);
        p.drawLine(41,40,41,0);
        p.drawLine(41,0,1,0);
    }
    if(highlight == 2)
    {
        QPen ssss;
        ssss.setColor(QColor(127,255,0));
        ssss.setWidth(3);
        p.setPen(ssss);
        p.drawLine(1,0,1,40);
        p.drawLine(1,40,41,40);
        p.drawLine(41,40,41,0);
        p.drawLine(41,0,1,0);
    }
}

void Chess::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        chosen = 1;
        emit chosen_one(x,y);
        update();
    }
}
