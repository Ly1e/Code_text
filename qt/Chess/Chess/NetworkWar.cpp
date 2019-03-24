#include "NetworkWar.h"
#include <QDebug>

NetworkWar::NetworkWar(bool server)
{
   _server = NULL;
   _socket = NULL;
   _bServer = server;

   if(_bServer)       //根据参数选择作为服务器端还是客户端
   {
       _server = new QTcpServer(this);              //创建一个服务器
       _server->listen(QHostAddress::Any,9889);     //监听9999端口

       //当有人连接该服务器时，slotNewConnection()函数被调用
       connect(_server,SIGNAL(newConnection()),
               this,SLOT(slotNewConnection()));
   }
   else             //选择作为客户端启动
   {
       _socket = new QTcpSocket(this);              //创建一个客户端
       _socket->connectToHost(QHostAddress("127.0.0.1"),9889);//连接目标服务器

       connect(_socket,SIGNAL(readyRead()),
               this,SLOT(slotRecv()));
   }
}

NetworkWar::~NetworkWar()
{

}

void NetworkWar::click(int id, int row, int col)
{
    if(_selectid == -1 && id != -1)
    {
        if(_s[id]._red != _bSide)//点击的棋子和选边必须相同，防止在一端操控对方棋子的行为
            return;
    }
    Board::click(id,row,col);

    /* 把点击事件发送给对方 */
    char buf[4];
    buf[0] = 2;
    buf[1] = 9-row;     //翻转行和列
    buf[2] = 8-col;
    buf[3] = id;
    _socket->write(buf,4);
}

void NetworkWar::slotRecv()         //服务器与客户端
{
    QByteArray ba =  _socket->readAll();
    char cmd = ba[0];
    if(cmd == 1)                    //判断接收到的报文类型，以地一个字节作为判断
    {
        char data = ba[1];
        if(data == 0)//表示我要走黑棋
        init(data==1);
    }
    else if(cmd ==2)
    {
        int row = ba[1];
        int col = ba[2];
        int id = ba[3];
        Board::click(id,row,col);
    }
}

void NetworkWar::slotNewConnection()
{
    if(_socket) return;     //如果_socket已经有连接了状态不为空，则不再接受其它连接

   _socket = _server->nextPendingConnection();
   //readyRead()这个信号当_socket有数据传来的时候，调用slotRecv()
   connect(_socket,SIGNAL(readyRead()),
           this,SLOT(slotRecv()));

   qDebug() << "connect";         //测试是否连接成功
   //生成随机数决定谁红谁黑
    bool bRedSide = qrand()%2>0;
    init(bRedSide);

    //发送数据给对方
    QByteArray buf;
    buf.append(1);
    buf.append(bRedSide?0:1);
   _socket->write(buf,2);

   init(buf[1] == 0);

}
