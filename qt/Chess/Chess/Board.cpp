#include "Board.h"
#include <QPainter>
#include<QMouseEvent>

Board::Board(QWidget *parent) : QWidget(parent)
{
    this->_r = 30;
    init(true);
}

void Board::init(bool beRedSide)
{
    for(int i=0;i<32;++i)
    {
        _s[i].init(i);                                          //初始化了32个棋子的位置
    }
    _selectid = -1;                                             //此时没有选择棋子，_selectid初始置为-1
    _bRedTurn = true;           //起手红先走
    _beSide = beRedSide;
}
Board::~Board()
{

}

QSize Board::sizeHint() const
{
    return QSize(20*this->_r+1,22*this->_r+1);
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 2 * this->_r;                                                 //格子为正方形，一个格子的宽高为d

    painter.setPen(QPen(Qt::black,2));

    //画10条横线
    for (int i = 1; i <= 10; ++i)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //画9条竖线
    for (int i = 1; i <= 9; ++i)
    {
        if (i==1 || i==9)                                       //第一条第九条为全竖线
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));   //（40,40）->(40*10)
        else                                                    //否则要从中间断开，形成楚河汉界
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));    //从d到5d
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d)); //从6d画到10d
        }
    }

    //画九宫格
    painter.drawLine(QPoint(4*d,1*d),QPoint(6*d,3*d));          //画上面的\/
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));          //画下面的\/

    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //写字
    painter.setFont(QFont("华文隶书", 26));
    painter.drawText(QPoint(2*d, 5.7*d),"   楚 河                     汉 界" );

    //绘制32个棋子
    for (int i=0;i<32;++i)
    {
        drawStone(painter,i);
    }
}
/***************************常用功能函数**********************************/

//通过棋盘的行和列，来获得对应的像素点坐标
QPoint Board::center(int row,int col)
{
    QPoint ret;
    ret.rx() = (col+1)* _r*2;
    ret.ry() = (row+1)* _r*2;
    return ret;
}

//通过棋子的id获取行列值
QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}
//判断关系式
int Board::ralation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}

void Board::GetRowCol(int &row1, int &col1, int id)
{
    row1 = _s[id]._row;
    col1 = _s[id]._col;
    return;
}

int Board::getStoneId(int row, int col)
{
    for(int i=0;i<32;++i)
    {
        if((_s[i]._row == row) && (_s[i]._col == col) && (!_s[i]._dead))
        {
            return i;
        }
    }
    return -1;
}

bool Board::isBottomSide(int id)
{
    return _beSide == _s[id]._red;
}

//效率低，需要改进
bool Board::getClickRowCol(QPoint pt,int &row,int &col)
{
    for(row=0;row<=9;row++)
    {
        for(col=0;col<=8;col++)
        {
            QPoint c = center(row,col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx+dy*dy;
            if(dist<_r*_r)              //不开方就直接用r*r
                return true;
        }
    }
    return false;
}

int Board::getStoneCountAtLine(int row1,int col1,int row2,int col2)
{
    int ret =0;
    if(row1 != row2 && col1 != col2)                //都不等于意味着两点不在一条直线上
    {
        return -1;
    }
    if(row1 == row2 && col1 == col2)                //都相同意味着两点根本就是同一点
    {
        return -1;
    }
    if(row1 == row2)                                //两点所在的行相同
    {
        int min = col1 < col2 ?col1:col2;           //最小的列
        int max = col1 < col2 ?col2:col1;           //最大的列
        for(int col=min+1;col<max;++col)            //从小的列遍历到最大的列
        {
            if(getStoneId(row1,col) != -1)          //这条直线上有棋子
            {
                ++ret;                              //棋子个数
            }
        }
    }
    else
    {
        int min = row1 < row2 ?row1:row2;
        int max = row1 < row2 ?row2:row1;
        for(int row=min+1;row<max;++row)
        {
            if(getStoneId(row,col1) != -1)
            {
                ++ret;
            }
        }
    }
    return ret;
}

//绘制棋子
void Board::drawStone(QPainter& painter,int id)
{
    if(_s[id]._dead)                                            //死了的棋子不画
        return;

    //绘制中心点
    QPoint c = center(id);
    //drawText绘制的矩形，
    QRect rect = QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);

    if(id == _selectid)
        painter.setBrush(QBrush(Qt::gray));                    //如果棋子被选中，背景用灰色
    else
        painter.setBrush(QBrush(Qt::yellow));                  //否则用黄色

    //painter.setPen(Qt::black);                                 //设置画笔颜色为黑色
    //painter.drawEllipse(center(id),_r,_r);                     //画一个黑色的圈，半径为_r

    if(_s[id]._red)                                            //如果棋子为红色棋子
    {
        painter.setPen(QPen(Qt::red,2));
        painter.drawEllipse(center(id),_r,_r);  //_r长半径 _r短半径
        painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));//QTextOption(Qt::AlignCenter)
        painter.setPen(QPen(Qt::black,2));
    }
    else
    {
        painter.drawEllipse(center(id),_r,_r);  //_r长半径 _r短半径
        painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));//QTextOption(Qt::AlignCenter)
    }
}

/***************************规则函数**********************************/
bool Board::canMoveJiang(int moveid,int row,int col,int)
{
    /*目标位置只能在九宫格内
    移动的步长为一个格子（d）
    面将可以直接吃*/
    if(_s[moveid]._red)
    {
        if(row > 2)
        {
            return false;
        }
    }
    else
    {
        if(row < 7 )
        {
            return false;
        }
    }
    if(col < 3 || col > 5)
    {
        return false;
    }

//    int dr = _s[moveid]._row - row;     //dr 和 dc 必然有一个要等于0，因为是在一条直线上移动
//    int dc = _s[moveid]._col - col;     //dr 和 dc 必然有一个要等于0，另一个必然为1或者-1
//    int d = abs(dr)*10 + abs(dc);       // 马12,21 相22 兵10,1
    int row1,col1;
    GetRowCol(row1,col1,moveid);
    int r = ralation(row1,col1,row,col);
    if(r == 1 || r == 10)
    {
        return true;
    }
    return false;
}

bool Board::canMoveShi(int moveid,int row,int col,int)
{
    /*目标位置只能在九宫格内
    移动的步长为一个格子（d）*/
    if(_s[moveid]._red)
    {
        if(row > 2)
        {
            return false;
        }
    }
    else
    {
        if(row < 7 )
        {
            return false;
        }
    }
    if(col < 3 || col > 5)
    {
        return false;
    }

    int row1,col1;
    GetRowCol(row1,col1,moveid);
    int r = ralation(row1,col1,row,col);
    if(r == 11)
    {
        return true;
    }
    return false;
}

bool Board::canMoveXiang(int moveid,int row,int col,int)
{
    int row1,col1;
    GetRowCol(row1,col1,moveid);
    int r = ralation(row1,col1,row,col);
    if(r != 22)
    {
        return false;
    }

    int r_eye = (row + row1)/2;
    int c_eye = (col + col1)/2;
    if(getStoneId(r_eye,c_eye) != -1)
    {
        return false;
    }
    if(_s[moveid]._red)
    {
        if(row > 4)
            return false;
    }
    else
    {
        if(row < 5)
            return false;
    }
    return true;
}

bool Board::canMoveChe(int moveid,int row,int col,int)
{
    int row1,col1;
    GetRowCol(row1,col1,moveid);
    int count = getStoneCountAtLine(row1,col1,row,col);         //row1,col1对应的是当前要移动的棋子,row,col是目标位置
    if(count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Board::canMoveMa(int moveid,int row,int col,int)
{
    int row1,col1;
    GetRowCol(row1,col1,moveid);
    int r = ralation(row1,col1,row,col);
    if(r != 12 && r !=21)
    {
        return false;
    }
    int r_eye = (row + row1)/2;
    int c_eye = (col + col1)/2;
    if(r == 12)
    {
        if(getStoneId(row1,c_eye) != -1)
        {
            return false;
        }
    }
    else
    {
        if(getStoneId(r_eye,col1) != -1)
        {
            return false;
        }
    }

    return true;
}

bool Board::canMovePao(int moveid,int row,int col,int killid)
{
    int row1,col1;
    GetRowCol(row1,col1,moveid);
    int count = getStoneCountAtLine(row1,col1,row,col);         //row1,col1对应的是当前要移动的棋子,row,col是目标位置
    if(killid != -1)        //如果这次炮执行的操作是吃子
    {
        if(count == 1)      //那么炮中间必须有一个棋子
        {
            return true;
        }
    }
    else                    //如果这次炮执行的操作是走子
    {
        if(count == 0)      //那么中间不需要有棋子，走法和“车”一样
        {
            return true;
        }
    }
    return false;
}

bool Board::canMoveBing(int moveid,int row,int col,int)
{
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(_s[moveid]._red)             //红兵
    {
        if(row < _s[moveid]._row)
        {
            return false;
        }
        if(row <= 4)
        {
            if(d == 10)
            {
                return true;
            }
        }
        if(row > 4)
            if(d == 10 || d == 1)
            {
                return true;
            }
    }
    else                            //黑兵
    {
        if(row > _s[moveid]._row)
        {
            return false;
        }
        if(row >= 5)
        {
            if(d == 10)
            {
                return true;
            }
        }
        if(row < 5)
        {
            if(d == 10 || d == 1)
            {
                return true;
            }
        }
    }
    return false;
}

bool Board::canMove(int moveid,int row,int col,int killid)
{
    //如果移动的棋子和杀死的棋子颜色相同，则不能移动
    if(_s[moveid]._red == _s[killid]._red)
    {
        _selectid = killid;        //选到相同颜色的棋子则换成点击这个新的棋子并且重绘
        update();
        return false;
    }
    if(moveid == killid)
    {
        return false;
    }

    switch (_s[moveid]._type) {
    case Stone::JIANG:
        return canMoveJiang(moveid,row,col,killid);
        break;
    case Stone::SHI:
        return canMoveShi(moveid,row,col,killid);
        break;
    case Stone::XIANG:
        return canMoveXiang(moveid,row,col,killid);
        break;
    case Stone::CHE:
        return canMoveChe(moveid,row,col,killid);
        break;
    case Stone::MA:
        return canMoveMa(moveid,row,col,killid);
        break;
    case Stone::PAO:
        return canMovePao(moveid,row,col,killid);
        break;
    case Stone::BING:
        return canMoveBing(moveid,row,col,killid);
        break;
    default:
        break;
    }

    return true;
}

//鼠标释放
void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt = ev->pos();

    click(pt);
    /*
    //将pt转化成象棋的行列值
    //判断这个行列值上面有没有棋子
    int row,col;
    bool bRet = getClickRowCol(pt,row,col);
    if(bRet == false)       //点到棋盘外了
        return;
    int i;
    int clickid = -1;       //没有点到棋子
    for(i=0;i<32;i++)       //根据行列值获得在这个点的棋子的id
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
            if(_bRedTurn == _s[clickid]._red)
            {
                _selectid = clickid;        //第一次点击，获得点击的棋子的id并赋值给_selectid
                update();
            }
             //重绘
        }
    }
    else                                //第二次点击，
    {
        if(canMove(_selectid,row,col,clickid))
        {
            _s[_selectid]._row = row;           //把点击位置的行和列作为被选中的棋子新的行和列
            _s[_selectid]._col = col;
            if(clickid != -1)                   //第二次点击的位置点击到了棋子
            {
                _s[clickid]._dead =true;        //那么把这个棋子杀死
            }
            _selectid = -1;                     //走完了置成-1，为没有棋子被选中，否则仍然在选中状态
            _bRedTurn = !_bRedTurn;
            update();                           //重绘
        }
        else                                    //不能移动
        {
            if(clickid == _selectid)            //因为第二次点击还是点击相同的棋子
            {
                _selectid = -1;                 //取消选择
                clickid = -1;
                update();                       //重绘
            }
            if(_s[_selectid]._red == _s[clickid]._red)//因为第二次点击的是相同颜色的棋子
            {
                _selectid = clickid;            //换选择并且重绘
                update();
            }
        }
    }*/
}

void Board::click(QPoint point)
{
    int row, col;

    if (!getClickRowCol(point, row, col))   //如果在棋盘外,不做任何反应
        return;

    int id = getStoneId(row, col);
    click(id, row, col);
}

void Board::click(int clickid, int row, int col)
{
    if(_selectid == -1)  //如果之前没有棋子被选中
    {
        trySelectStone(clickid);
    }
    else
    {
        tryMoveStone(clickid, row, col);
    }
}

void Board::trySelectStone(int id)
{
    if (id == -1)
        return ;

    if (_bRedTurn == _s[id]._red)
    {
        _selectid = id;
        update();
    }
}

void Board::tryMoveStone(int killid, int row, int col)
{
    if (canMove(_selectid, row, col, killid))
    {
        //这里的selectid是第一次点击选中的棋子
        _s[_selectid]._row = row;
        _s[_selectid]._col = col;
        if (killid != -1)  //确保第二次也选中了棋子
            _s[killid]._dead = true;
        else
            trySelectStone(killid);
        update();

        //交换玩家
        _bRedTurn = !_bRedTurn;
        _selectid = -1;
    }
}

void Board::reliveStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = false;
}

void Board::killStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = true;
}

void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

void Board::moveStone(int moveid, int killid, int row1, int col1)
{
    saveStep(moveid, killid, row1, col1, _steps);
    killStone(killid);
    moveStone(moveid, row1, col1);
}
/***************************悔棋函数**********************************/
void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    int row1,col1;
    GetRowCol(row1,col1,moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

void Board::back(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

void Board::backOne()
{
    if(this->_steps.size() == 0) return;

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}
