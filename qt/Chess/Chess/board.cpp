#include "board.h"
#include <QPainter>
#include<QMouseEvent>

Board::Board(QWidget *parent) : QWidget(parent)
{
    for(int i=0;i<32;++i)
    {
        _s[i].init(i);          //32个棋子的初始化
    }
    _selectid = -1;
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 40;             //格子为正方形，一个格子的宽高为d
    _r = d/2;               //棋子的半径为格子的一半长
    //画10条横线
    for (int i = 1; i <= 10; ++i)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //画9条竖线
    for (int i = 1; i <= 9; ++i)
    {
        if (i==1 || i==9)//第一条第九条为全竖线
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else             //否则要从中间断开，形成楚河汉界
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));//从d到5d
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));//从6d画到10d
        }
    }

    //画九宫格
    painter.drawLine(QPoint(4*d,1*d),QPoint(6*d,3*d));      //画上面的\/
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));      //画下面的\/

    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //绘制32个棋子
    for (int i=0;i<32;++i)
    {
        drawStone(painter,i);
    }
}

//通过棋盘的行和列，来获得对应的像素点坐标
QPoint Board::center(int row,int col)
{
    QPoint ret;
    ret.rx() = (col+1)* _r*2;
    ret.ry() = (row+1)* _r*2;
    return ret;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}

//绘制棋子
void Board::drawStone(QPainter& painter,int id)
{
    if(_s[id]._dead)
        return;
    //绘制中心点
    QPoint c = center(id);
    //drawText绘制的矩形，
    QRect rect = QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);

    if(id == _selectid)
        painter.setBrush(QBrush(Qt::gray)); //如果棋子被选中，背景用灰色
    else
        painter.setBrush(QBrush(Qt::yellow));//否则用黄色

    //painter.setBrush(QBrush(QColor(255,255,0)));    //设置黄色画刷
    painter.setPen(Qt::black);                      //设置画笔颜色为黑色

    painter.drawEllipse(center(id),_r,_r);          //画一个黑色的圈，半径为_r

    if(_s[id]._red)                                 //如果棋子为红色棋子
        painter.setPen(Qt::red);                    //设置字的颜色为红色

    painter.setFont(QFont("system",_r,700));        //改变字体的大小
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));//QTextOption(Qt::AlignCenter)这里指定为中间对齐
}
//效率低，需要改进
bool Board::getRowCol(QPoint pt,int &row,int &col)
{
    for(row=0;row<=9;row++)
    {
        for(col=0;col<=8;col++)
        {
            QPoint c = center(row,col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx+dy*dy;
            if(dist<_r*_r)
                return true;
        }
    }
    return false;
}

//鼠标释放
void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt = ev->pos();
    //将pt转化成象棋的行列值
    //判断这个行列值上面有没有棋子
    int row,col;
    bool bRet = getRowCol(pt,row,col);
    if(bRet == false)       //点到棋盘外了
        return;
    int i;
    int clickid = -1;
    for(i=0;i<32;i++)
    {
        if(_s[i]._row == row && _s[i]._col == col && _s[i]._dead == false)
            break;
    }

    if(i<32)
    {
        clickid = i;
    }
    if(_selectid == -1)
    {
        if(clickid != -1)
        {
            _selectid = clickid;
            update();
        }
        if(i<32)
        {
            _selectid = i;
            update();
        }
    }
    else
    {
        _s[_selectid]._row = row;
        _s[_selectid]._col = col;
        if(clickid != -1)
        {
            _s[clickid]._dead =true;
        }
        _selectid = -1;         //走完了置成-1，否则仍然在选中状态
        update();
    }
}
