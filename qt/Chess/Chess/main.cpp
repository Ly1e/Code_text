
#include <QApplication>
#include "board.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    Board board;                //定义一个Board对象,此时调用了Board::Board(QWidget *parent) : QWidget(parent)
    board.show();               //调用void paintEvent(QPaintEvent *)
    return app.exec();
}
