#include "MainWindow.h"
#include<QHBoxLayout>
#include "NetworkWar.h"
#include "SinglePlay.h"
#include "MultiGame.h"

MainWindow::MainWindow(int gameTpye, QWidget *parent) : QWidget(parent)
{
    _gameType = gameTpye;

    if(_gameType == 0)                          //人人对战
    {
        MultiGame* game = new MultiGame;
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }
    else if(_gameType == 1)                     //人机对战
    {
        SinglePlay* game = new SinglePlay;      //生成人机对战
        CtrlPanel* panel = new CtrlPanel;       //控制面板

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }
    else if(_gameType == 2)                     //网络对战(服务器端)
    {
        NetworkWar* game = new NetworkWar(true);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }
    else if(_gameType == 3)                     //网络对战(客户端端)
    {
        NetworkWar* game = new NetworkWar(false);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }
}
MainWindow::~MainWindow()
{

}
