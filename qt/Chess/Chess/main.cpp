#include <QApplication>
#include "ChooseDlg.h"
#include "MainWindow.h"
#include <QTime>
#include <QMessageBox>

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);

    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);       //生成随机数

    ChooseDlg dlg;                          //选择窗口
    if(dlg.exec() != QDialog::Accepted)
        return 0;

    MainWindow wnd(dlg._selected);          //根据选择创建不同模式的游戏窗口
    wnd.show();  

    return app.exec();
/*
    QApplication app(argc,argv);

    QMessageBox::StandardButton ret;
    ret = QMessageBox::question(NULL,"server or client","作为服务器启动");    //弹出询问消息对话框

    bool bServer = false;
    if(ret == QMessageBox::Yes)
    {
        bServer = true;
    }

    NetworkWar board(bServer);//定义一个Board对象,此时调用NetworkWar的构造函数
    board.show();             //调用void paintEvent(QPaintEvent *)
    return app.exec();
*/
}
