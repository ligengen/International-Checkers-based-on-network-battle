#ifndef CHESS_H
#define CHESS_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPalette>
#include <QFrame>
#include <QPoint>
#include <QColor>
#include <QRect>
#include <QPixmap>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>

class Chess : public QWidget
{
    Q_OBJECT
public:
    explicit Chess(int _x, int _y, int who, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* ev);
    void mousePressEvent(QMouseEvent *event);
    void setStatusValue(int a) {status = a;}
    bool chosen;
    //void mouseReleaseEvent(QMouseEvent *event);
signals:
    void chosen_one(int,int);
    void clicked(int,int);
public slots:
    void setHighlight(int highlight){this->highlight = highlight;}
    void setStatus(int a)
    {
        this->status = a;
    }

private:
    int x,y;
    bool queen;
    int highlight;
    int who;
    int status,back;
};


#endif // CHESS_H
