#ifndef SINGLEPLAY_H
#define SINGLEPLAY_H

#include "Board.h"
#include "Step.h"

class SinglePlay : public Board                     //从board派生
{
    Q_OBJECT
public:
    SinglePlay()
    {
        _level = 4;
    }
    virtual void click(int id, int row, int col);

    Step* getBestMove();                            //获取最好的走法
    void getAllPossibleMove(QVector<Step*>& steps); //获取所有可能的走法

    void fakeMove(Step* step);                      //尝试移动
    void unfakeMove(Step* step);                    //撤销移动尝试
    int calcScore();                                //计算得分

    int getMinScore(int level,int curMaxScore);
    int getMaxScore(int level,int curMinScore);
    \
    int _level;                                     //考虑的层次
public slots:
    void computerMove();
};

#endif // SINGLEPLAY_H
