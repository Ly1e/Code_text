#include "SinglePlay.h"
#include <QTimer>

void SinglePlay::click(int id, int row, int col)
{
    if(!this->_bRedTurn)
        return;
    Board::click(id,row,col);

    if(!this->_bRedTurn)
    {
        //启动0.1ms定时器，用户下完棋子后在0.1ms后电脑再思考
        QTimer::singleShot(100,this,SLOT(computerMove()));
    }
}

void SinglePlay::computerMove()
{
    Step*  step = getBestMove();
    moveStone(step->_moveid,step->_killid,
              step->_rowTo,step->_colTo);

    delete step;
    update();
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
    static int chessScore[] = {1000,499,501,200,15000,100,100};

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
    //如果黑棋走，从16到32去遍历，如果红旗走，从0到16遍历
    int min = 16,max = 32;
    if(this->_bRedTurn)
    {
        min = 0;
        max = 16;
    }
    for(int i=min;i<max;++i)
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

int SinglePlay::getMaxScore(int level)
{
    if(level == 0)return calcScore();

    //看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);                              //这里是红旗的所有可能的走法

    int maxScore = -1000000;
    while(steps.count())
    {

        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);                                     //尝试这样走
        int score = getMinScore(level-1);                            //计算这样走的得分
        unfakeMove(step);                                   //撤销这次尝试

        if(score > maxScore)
        {
            maxScore = score;                               //与最高得分比较，保存最高得分
        }
        delete step;
    }
    return maxScore;
}

int SinglePlay::getMinScore(int level)
{
    if(level == 0)return calcScore();

    //看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);                              //这里是红旗的所有可能的走法

    int minScore = 1000000;
    while(steps.count())
    {

        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);                                     //尝试这样走
        int score = getMaxScore(level-1);                            //计算这样走的得分
        unfakeMove(step);                                   //撤销这次尝试

        if(score < minScore)
        {
            minScore = score;                               //与最高得分比较，保存最高得分
        }
        delete step;
    }
    return minScore;
}

Step* SinglePlay::getBestMove()
{
    //1看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //试着走一下
    int maxScore = -999999;
    Step* ret = NULL;
    while(steps.count())       //遍历所有可走的步法
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);                                     //尝试这样走
        int score = getMinScore(_level-1);                          //计算这样走的得分
        unfakeMove(step);                                   //撤销这次尝试

        if(score > maxScore)
        {
            maxScore = score;                               //与最高得分比较，保存最高得分
            if(ret)
            {
                delete ret;
            }
            ret = step;                                     //保存最高得分的走法
        }
        else
        {
            delete step;
        }
    }
    return ret;
}
