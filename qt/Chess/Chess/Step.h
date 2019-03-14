#ifndef STEP_H
#define STEP_H

#include <QObject>

//Step，主要悔棋用，人工智能选最优走法时也用到来记录

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    int _moveid;        //走的是哪个棋
    int _killid;        //走了之后哪个棋被杀了
    int _rowFrom;       //从哪个行走过来的
    int _colFrom;       //从哪个列走过来的
    int _rowTo;         //走到哪个行去
    int _colTo;         //走到哪个列去

signals:

public slots:
};

#endif // STEP_H
