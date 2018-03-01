#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include "chess.h"
#include "draughts.h"

class Checkerboard : public QWidget
{
    Q_OBJECT
public:
    explicit Checkerboard(int who,QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    Chess*** button;
    Draughts* draughts;
signals:
    void _whoColor(int);
    void writ(QString);
    void lose();
public slots:
    void whoColor(int);
    void setDisable(int,int);
    void upd();
    void clicked(int,int);
    void recv(QString ord);
private:

    int who;
    int color;
};


#endif // CHECKERBOARD_H
