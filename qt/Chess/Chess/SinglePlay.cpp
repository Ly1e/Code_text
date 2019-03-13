#include "SinglePlay.h"

void SinglePlay::click(int id, int row, int col)
{
    if(!this->_bRedTurn)
        return;
    Board::click(id,row,col);

    if(!this->_bRedTurn)
    {
        Step*  step = getBestMove();
        moveStone(step->_moveid,step->_killid,
                  step->_rowTo,step->_colTo);
    }
}

void SinglePlay::fakeMove(Step* step)
{
    killStone(step->_killid);
    moveStone(step->_moveid,step->_rowTo,step->_colTo);
}

void SinglePlay::unfakeMove(Step* step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid,step->_rowFrom,step->_colFrom);
}

/* 评价局面分 */
int SinglePlay::calcScore()
{
    int redTotalScore = 0;
    int blackTotalScore = 0;
//    enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int chessScore[] = {100,50,50,20,1500,10,10};

    //黑棋分的总数 - 红棋分的总数
    for(int i=0;i<16;++i)
    {
        if(_s[i]._dead) continue;

        redTotalScore += chessScore[_s[i]._type];
    }
    for(int i=16;i<32;++i)
    {
        if(_s[i]._dead) continue;

        blackTotalScore += chessScore[_s[i]._type];
    }

    return blackTotalScore - redTotalScore;
}

//保存黑棋当前所有可能走的走法
void SinglePlay::getAllPossibleMove(QVector<Step *> &steps)
{
    for(int i=16;i<32;++i)
    {
        if(_s[i]._dead) continue;       //

        for(int row=0;row<9;++row)
        {
            for(int col=0;col<8;++col)
            {
                int killid = this->getStoneId(row,col);
                if(sameColor(killid,i)) continue;

                if(canMove(i,killid,row,col))
                {
                    saveStep(i,killid,row,col,steps);
                }
            }
        }
    }
}

Step* SinglePlay::getBestMove()
{
    //1看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //试着走一下
    int maxScore = -999999;
    Step* ret;
    for(QVector<Step*>::iterator it = steps.begin();it!=steps.end();++it)       //遍历所有可走的步法
    {
        Step* step = *it;                                   //保存到step
        fakeMove(step);                                     //尝试这样走
        int score = calcScore();                        //计算这样走的得分
        unfakeMove(step);                                   //撤销这次尝试

        if(score > maxScore)
        {
            maxScore = score;                               //与最高得分比较，保存最高得分
            ret = step;                                     //保存最高得分的走法
        }
    }
    return ret;
}
