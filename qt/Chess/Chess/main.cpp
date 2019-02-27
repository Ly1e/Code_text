
#include <QApplication>
#include "board.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    Board board;                //定义一个Board对象
    board.show();
    return app.exec();
}
