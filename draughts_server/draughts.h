#ifndef DRAUGHTS_H
#define DRAUGHTS_H

#include <QWidget>
#include <QObject>

class Store : public QObject
{
    Q_OBJECT
public:
    explicit Store(QObject* parent = 0);
    int dep,maxdep;
    int x,y;
    std::vector<Store*> step;
private:

};

class Draughts : public QWidget
{
    Q_OBJECT
public:
    explicit Draughts(int color,QWidget *parent = nullptr);
    void searchForPossible(Store* p);
    bool isOK(int,int);
    void initStore();
    bool delStore(Store* p,int maxdep);
    void lose();///
    void lock(bool a) {locked = a;}
    void checkqueen();
    void chosen(int);
    void goStore(std::pair<int,int> p);
    void repeal();
    void finstep();
    void endStep();
    std::vector<int> nextPos();
    int nowPos();
    int _Chess(int r, int c)
    {
        return chess[r][c];
    }
    void startStep(QString text);///
    int sgn(int);
    void goStore(std::vector<int> lis);
signals:
    void readyView();
    void oneStep(QString);
    void _lose();///
public slots:
    void getInfo(int,int);
    void getColor(int);
private:
    std::vector<int> rec;
    bool locked;
    int l_st[10][10];
    Store* nowStep;
    Store* newStep;
    int chess[10][10];
    int begin_x,begin_y;
    int color;
    int dr[4] = {-1,-1,1,1},dc[4]={-1,1,-1,1};
};
#endif // DRAUGHTS_H
