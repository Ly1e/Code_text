#ifndef SINGLEPLAY_H
#define SINGLEPLAY_H

#include "Board.h"
#include "Step.h"

class SinglePlay : public Board     //从board派生
{
public:

    virtual void click(int id, int row, int col);

    Step* getBestMove();
    void getAllPossibleMove(QVector<Step*>& steps);

    void fakeMove(Step* step);
    void unfakeMove(Step* step);
    int calcScore();
};

#endif // SINGLEPLAY_H
