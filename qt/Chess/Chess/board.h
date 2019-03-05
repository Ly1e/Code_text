#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "stone.h"      //棋子类

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

    Stone _s[32];           //32个棋子
    int _r;                 /*棋子的半径*/
    int _selectid;
    bool _bRedTurn;         //轮到谁走了

    /*返回象棋棋盘行列对应的像素坐标*/
    QPoint center(int row,int col);
    QPoint center(int id);
    bool getRowCol(QPoint pt,int& row,int& col);

    //绘制棋子
    void drawStone(QPainter& painter,int id);

    //绘制函数
    void paintEvent(QPaintEvent *);

    void mouseReleaseEvent(QMouseEvent *);      //鼠标释放
\
    //规则
    bool canMove(int moveid,int row,int col,int killid);
    bool canMoveJiang(int moveid,int row,int col,int killid);
    bool canMoveShi(int moveid,int row,int col,int killid);
    bool canMoveXiang(int moveid,int row,int col,int killid);
    bool canMoveChe(int moveid,int row,int col,int killid);
    bool canMoveMa(int moveid,int row,int col,int killid);
    bool canMovePao(int moveid,int row,int col,int killid);
    bool canMoveBing(int moveid,int row,int col,int killid);

    //规则Ext
    void GetRowCol(int& row1,int& col1,int id);         //根据id号获取行列值
    int getStoneId(int row,int col);                    //根据当前行列获取棋子id,没有棋子返回-1
    int ralation(int row1,int col1,int row,int col);
    int getStoneCountAtLine(int row1,int col1,int row,int col);     //返回两点之间的棋子的个数，0表示没有，-1表示两点不在一条直线上
signals:

public slots:
};

#endif // BOARD_H
