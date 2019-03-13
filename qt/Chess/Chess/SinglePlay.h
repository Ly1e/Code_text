#ifndef SINGLEPLAY_H
#define SINGLEPLAY_H

#include "Board.h"

class SinglePlay : public Board     //从board派生
{
public:

    virtual void click(int id, int row, int col);
};

#endif // SINGLEPLAY_H
