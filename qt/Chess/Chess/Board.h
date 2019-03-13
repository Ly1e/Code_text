#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "Stone.h"      //棋子类
#include "Step.h"
#include <QPoint>
#include <QVector>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    ~Board();

    int _r;                     /*棋子的半径,格子的一半宽*/
    int _selectid;              //被选中的棋子
    bool _bRedTurn;             //交换玩家,true表示红走
    bool _beSide;                //红方 黑方

    QVector<Step *>_steps; //记录每走一步棋，悔棋用
    Stone _s[32];               //32个棋子


    void init(bool beRedSide);  //摆棋子，beRedSide为true表示红在下

    /*将棋子在棋盘中的坐标（行，列）转换成在窗口中的坐标*/

    /* 绘制 */
    void paintEvent(QPaintEvent *);
    virtual QSize sizeHint () const;
    void drawStone(QPainter& painter,int id);

    /* 坐标转换 */
    QPoint center(int row,int col);             //某行某列对应的棋子像素中心点
    QPoint center(int id);                      //棋子的中心点
    QPoint windowCenter(int row,int col);       //将窗口中的位置转换成棋盘上的位置

    bool getClickRowCol(QPoint pt,int &row,int &col);   //点击在那个棋子圆范围内都会当做选中该棋子，如吃对方棋时用

    /* help */
    int getStoneId(int row,int col);                    //根据当前行列获取棋子id,没有棋子返回-1
    void killStone(int id);                             //吃掉这个id的棋子
    void reliveStone(int id);                           //恢复死去的棋子，悔棋用
    void moveStone(int moveid, int row, int col);       //moveStone只是改坐标

    /* 移动棋子 */
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint point);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);     //尝试选中棋子，但未必能选中，如红走时是选不中黑的棋的
    void tryMoveStone(int killid, int row, int col);    //****moveStone还要记录吃掉的棋子，悔棋用
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void backOne();
    void back(Step* step);  //悔棋两步以上
    virtual void back();

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
    int getStoneCountAtLine(int row1,int col1,int row,int col);     //返回两点之间的棋子的个数，0表示没有，-1表示两点不在一条直线上
    void GetRowCol(int& row1,int& col1,int id);         //根据id号获取行列值
    bool isBottomSide(int id);
    int ralation(int row1,int col1,int row,int col);

signals:

public slots:
    void slotBack();
};

#endif // BOARD_H
