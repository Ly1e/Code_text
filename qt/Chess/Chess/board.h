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

    /*返回象棋棋盘行列对应的像素坐标*/
    QPoint center(int row,int col);
    QPoint center(int id);
    bool getRowCol(QPoint pt,int& row,int& col);

    //绘制棋子
    void drawStone(QPainter& painter,int id);

    //绘制函数
    void paintEvent(QPaintEvent *);

    void mouseReleaseEvent(QMouseEvent *);      //鼠标释放
signals:

public slots:
};

#endif // BOARD_H
