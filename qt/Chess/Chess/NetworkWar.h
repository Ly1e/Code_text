#ifndef NETORKWAR_H
#define NETORKWAR_H

#include"Board.h"
#include <QTcpServer>
#include <QTcpSocket>
/*  */

/*
 *报文格式定义
 * 1.初始化        两个字节
 * 2.鼠标单击     四个字节
 * 3.毁棋         一个字节
*/
class NetworkWar : public Board
{
    Q_OBJECT
public:
    bool _bServer;
    NetworkWar(bool server);
    ~NetworkWar();

    QTcpServer* _server;    //服务器
    QTcpSocket* _socket;    //客户端

    void click(int id, int row, int col);   //重载基类的虚函数

public slots:
    void slotNewConnection();       //槽函数
    void slotRecv();
};

#endif // NETORKWAR_H
