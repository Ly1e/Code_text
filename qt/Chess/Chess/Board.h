#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QVector>

class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    /* 游戏数据 */
    Stone _s[32];//
    int _r;                                                     //棋子半径
    QPoint _off;                                                //車与窗口角落的偏移
    bool _bSide;                                                //红方 黑方

    QVector<Step*> _steps;                                      //记录每走一步棋，悔棋用

    /* 游戏状态 */
    int _selectid;                                              //选中的棋子编号，0-31
    bool _bRedTurn;                                             //true表当前红方走
    bool _gameOver;
    void init(bool bRedSide);                                   //摆棋子，bRedSide为true表红在下，网络对战时用

    /* 绘制功能 */
    void paintEvent(QPaintEvent *);
    void drawPlate(QPainter& p);                                //画棋盘
    void drawPlace(QPainter& p);                                //画九宫格
    void drawInitPosition(QPainter& p);                         //初始位置
    void drawInitPosition(QPainter& p, int row, int col);       //炮和兵的初始位置处的折角
    void drawStone(QPainter& p);
    void drawStone(QPainter& p, int id);

    /* 坐标转换*/
    QPoint center(int row, int col);                            //某行某列对应的棋子像素中心点
    QPoint center(int id);                                      //棋子的中心点
    QPoint topLeft(int row, int col);                           //某行某列对应的棋子覆盖方框的左上角点
    QPoint topLeft(int id);
    QRect cell(int row, int col);                               //某行某列对应的棋子覆盖方框范围
    QRect cell(int id);

    bool getClickRowCol(QPoint pt, int& row, int& col);         //点击在那个棋子圆范围内都会当做选中该棋子，如吃对方棋时用

    /* 帮助功能 */
    QString name(int id);                                       //exp：数字0转换为字符車
    bool red(int id);                                           //判断颜色，id是红的则返回true;
    bool sameColor(int id1, int id2);                           //判断两个棋子颜色是否相同，颜色相同返回true;
    int getStoneId(int row, int col);                           //获取当前行列位置的棋子的id，没有棋子则返回-1
    void killStone(int id);                                     //吃掉对方的id的棋子
    void reliveStone(int id);                                   //恢复死去的棋子，悔棋用
    void moveStone(int moveid, int row, int col);               //moveStone只是改坐标
    bool isDead(int id);                                        //判断这个id的棋子是否已经死亡

    /* 移动棋子 */
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);                                //尝试选中棋子，但未必能选中，如红走时是选不中黑的棋的
    void tryMoveStone(int killid, int row, int col);            //尝试移动棋子，但未必能移动，如移到相同颜色的棋子上
    void moveStone(int moveid, int killid, int row, int col);   //****moveStone还要记录吃掉的棋子，悔棋用
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);//每步走棋保存
    void backOne();                                             //悔棋一步
    void back(Step* step);                                      //悔棋两步以上
    virtual void back();

    /* 走法规则*/
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    bool canSelect(int id);                                     //红方只可选红棋

    /* 规则助手 */
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);//直线上两坐标点之间getStoneCount有多少棋子，不在一条直线返回-1
    int relation(int row1, int col1, int row, int col);         //算法规则
    bool isBottomSide(int id);                                  //判断上下方

    virtual void gameOver(int killid);
signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
