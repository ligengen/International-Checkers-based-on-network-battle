#include "draughts.h"
#include <QDebug>
#include <cmath>
#include <fstream>
#include <QSound>
Store::Store(QObject* parent):QObject(parent)
{
    step.clear();
}

Draughts::Draughts(int color,QWidget *parent) : QWidget(parent),color(color)
{
    //chess[i][j]为0表示不能放棋子,1为黑旗，2为白旗，-1为黑王，-2为百王（1、2可以对调角色）
    std::ifstream fin;
    fin.open("/Users/ligen/Desktop/QT/draughts_client/b.txt");
    if(!fin)
    {
        qDebug()<< "fin not success!" <<endl;
        //return;
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
        for(int j=0;j<10;j++)
        {
            chess[i][j] = s[i][j];
        }
    }

    lock(1);
    nowStep = new Store;
    nowStep->dep = 0;
    nowStep->maxdep = 0;
    nowStep->x = nowStep->y = -1;
    initStore();
}

void Draughts::searchForPossible(Store* p)
{
    if(p->dep == 0)
    {
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                if(abs(chess[i][j]) == color + 1)
                {
                    Store* st = new Store;
                    st->dep = 1;
                    st->maxdep = 0;
                    st->x = i;
                    st->y = j;
                    p->step.push_back(st);
                    searchForPossible(st);
                    if (st->maxdep > p->maxdep)
                        p->maxdep = st->maxdep;
                }
            }
        }
    }
    else
    {
        int r = p->x , c = p->y;
        if(chess[r][c]>0)
        {
            if(p->dep == 1)
            {
                for(int i=0;i<2;i++)
                {
                    int qr = r + dr[i];
                    int qc = c + dc[i];
                    if(!isOK(qr,qc))
                        continue;
                    if(chess[qr][qc]!=0)
                        continue;
                    Store* nx = new Store;
                    nx->dep = 1;
                    nx->maxdep = 1;
                    nx->x = qr;
                    nx->y = qc;
                    p->step.push_back(nx);
                    if(nx->maxdep > p->maxdep)
                        p->maxdep = nx->maxdep;
                }
            }
            for(int i=0;i<4;i++)
            {
                int pr = r + dr[i];
                int pc = c + dc[i];
                int qr = pr+ dr[i];
                int qc = pc+ dc[i];
                if(!isOK(pr,pc)) continue;
                if(!isOK(qr,qc)) continue;
                if(abs(chess[pr][pc])!=3-chess[r][c])
                    continue;
                if(chess[qr][qc]!=0)
                    continue;
                Store* nx = new Store;
                nx->dep = p->dep +1;
                nx->maxdep = nx->dep;
                nx->x = qr;
                nx->y = qc;
                p->step.push_back(nx);
                chess[qr][qc] = chess[r][c];
                chess[r][c] = 0;
                int tmp = chess[pr][pc];
                chess[pr][pc] =3;
                searchForPossible(nx);
                chess[r][c] = chess[qr][qc];
                chess[qr][qc] = 0;
                chess[pr][pc] = tmp;
                if(nx->maxdep > p->maxdep)
                    p->maxdep = nx->maxdep;
            }
        }
        else
        {
            for(int i=0;i<4;i++)
            {
                int cnt = 0;
                int pr,pc,qr = r,qc = c;
                for(;;)
                {
                    qr += dr[i];
                    qc += dc[i];
                    if(!isOK(qr,qc))
                        break;
                    if(abs(chess[qr][qc]) == 3 + chess[r][c])
                    {
                        if(cnt == 0)
                        {
                            cnt ++;
                            pr = qr;
                            pc = qc;
                            continue;
                        }
                        else break;//只搜跳了一次的情况，多次跳多次调用
                    }
                    if(chess[qr][qc]!=0)
                        break;
                    if(cnt == 0)
                    {
                        if(p->dep == 1)
                        {
                            Store* nx = new Store;
                            nx->dep = 1;
                            nx->maxdep = 1;
                            nx->x = qr;
                            nx->y = qc;
                            p->step.push_back(nx);
                            if(nx->maxdep > p->maxdep)
                                p->maxdep = nx->maxdep;
                        }
                    }
                    else
                    {
                        Store* nx = new Store;
                        nx->dep = p->dep+1;
                        nx->maxdep = nx->dep;
                        nx->x = qr;
                        nx->y = qc;
                        p->step.push_back(nx);
                        chess[qr][qc] = chess[r][c];
                        chess[r][c] = 0;
                        int tmp = chess[pr][pc];
                        chess[pr][pc] = 3;
                        searchForPossible(nx);
                        chess[r][c] = chess[qr][qc];
                        chess[qr][qc] = 0;
                        chess[pr][pc] = tmp;
                        if(nx->maxdep > p->maxdep)
                            p->maxdep = nx->maxdep;
                    }
                }
            }
        }
    }
}

bool Draughts::isOK(int x, int y)
{
    if(x<0 || x>9)
        return 0;
    if(y<0 || y>9)
        return 0;
    return 1;
}

void Draughts::getInfo(int _x,int _y)
{
    begin_x = _x; begin_y = _y;
}

void Draughts::getColor(int a)
{
    color = a;
}

void Draughts::initStore()
{
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            l_st[i][j] = chess[i][j];
    nowStep->dep = 0;
    nowStep->step.clear();
    nowStep->maxdep = 0;
    searchForPossible(nowStep);
    if (nowStep->maxdep < 1)
        nowStep->maxdep = 1;
    delStore(nowStep,nowStep->maxdep);
    newStep  = nowStep;
    rec.clear();
    if(nowStep->step.size() == 0)
    {
        lock(1);
        lose();
    }
}

void Draughts::lose()
{
    emit _lose();
}

bool Draughts::delStore(Store *p, int maxdep)
{
    int size = p->step.size();
    std::vector<Store*> newStep;
    newStep.clear();
    for(int i=0;i<size;i++)
    {
        if(delStore(p->step[i],maxdep))
            newStep.push_back(p->step[i]);
    }
    if(p->maxdep < maxdep)
    {
        delete p;
        return 0;
    }
    p->step = newStep;
    return 1;
}

void Draughts::checkqueen()
{
    for(int i=0;i<10;i++)
        if(chess[0][i] == color + 1)
        {
            chess[0][i] = -(color+1);
        }
    for(int i=0;i<10;i++)
        if(chess[9][i] == 2 - color)
        {
            chess[9][i] = color - 2;
        }
}

void Draughts::chosen(int k)
{
    if(locked) return;
    int r = k/10 , c = k%10;
    int sr = newStep->x , sc = newStep->y;
    int size = newStep->step.size();
    bool yes = 0;
    qDebug("--------");
    for(int i=0;i<size;i++)
    {
        qDebug("%d,%d", newStep->step[i]->x, newStep->step[i]->y);
        if(newStep->step[i]->x == r && newStep->step[i]->y == c)
        {
            newStep = newStep->step[i];
            yes = 1;
            break;
        }
    }
    if(yes)
    {
        rec.push_back((10-r-1)*10+10-c-1);
        if(sr!=-1&&sc!=-1)
        {
            goStore(std::make_pair(sr*10+sc,r*10+c));
            emit oneStep(tr("%1/").arg((9-sr)*10+(9-sc))
                         +tr("%1/#").arg((9-r)*10+(9-c)));
        }

    }
    else
    {
        if(newStep->x != r || newStep->y != c)
        {
            newStep = nowStep;
            repeal();
        }
    }
    if(newStep->step.size() == 0)
        finstep();
}

void Draughts::goStore(std::pair<int, int> p)
{
    int sr = p.first/10;
    int sc = p.first%10;
    int tr = p.second/10;
    int tc = p.second%10;
    int d_r = sgn(tr-sr);
    int d_c = sgn(tc-sc);
    int r = sr, c=sc;
    for(;;)
    {
        r += d_r;
        c += d_c;
        if(r == tr) break;
        if(c == tc) break;
        if(!isOK(r,c)) break;
        if(chess[r][c] != 0)
            chess[r][c] = 3;
    }
    chess[tr][tc] = chess[sr][sc];
    chess[sr][sc] = 0;
    QSound::play("/Users/ligen/Desktop/QT/draughts_server/resource/5390.wav");
    emit readyView();
}

void Draughts::repeal()
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            chess[i][j] = l_st[i][j];
        }
    }
    rec.clear();
    emit readyView();
}

void Draughts::finstep()
{
    endStep();

    emit oneStep("*");
    lock(1);
}
void Draughts::endStep()
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
            if(chess[i][j] == 3)
                chess[i][j] = 0;
    }
    checkqueen();
    emit readyView();
}
void Draughts::startStep(QString text)
{
    std::vector<int> tmp;
    tmp.clear();
    qDebug()<<text;
    int size = text.size();
    int cnt = 0;
    for(int i=0;i<size;i++)
        if('0'<=text[i] && text[i] <= '9')
        {
            cnt = cnt *10 + (text.toStdString()[i] - '0');
        }
        else
        {
            if(text[i] == '#')
            {
                goStore(tmp);
                tmp.clear();
            }
            else if (text[i] == '*')
            {
                checkqueen();
                initStore();
                lock(0);
            }
            else if (text[i] == '/')
            {
                tmp.push_back(cnt);
                cnt = 0;
            }
        }
    //goStore(tmp);

    emit readyView();
}

int Draughts::sgn(int a)
{
    if(a>0)
        return 1;
    else if(a<0)
        return -1;
    return 0;
}

void Draughts::goStore(std::vector<int> lis)
{
    int size = lis.size();
        for (int i = 0; i < size - 1; i ++)
        {
            goStore(std::make_pair(lis[i], lis[i + 1]));
        }
        endStep();
}
std::vector<int> Draughts::nextPos()//highlight
{
    std::vector<int> tmp;
    tmp.clear();
    if (newStep->dep == 0) return tmp;
    int size = newStep->step.size();
    for (int i = 0; i < size; i ++)
        tmp.push_back(newStep->step[i]->x * 10
                      + newStep->step[i]->y);
    return tmp;
}

int Draughts::nowPos()//highlight
{
    if (newStep->dep == 0) return -1;
    return newStep->x * 10 + newStep->y;
}
