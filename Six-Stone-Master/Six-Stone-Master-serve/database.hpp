#ifndef DATABASE_H
#define DATABASE_H
#include<QString>
#include<QTcpSocket>


 // 用于记录某个棋子是主机落下的还是对手落下的
enum playerRole
{
    HOST,
    GUEST
};
 // 记录当前游戏状态是playingGame（正在游戏）还是readyToPlayingGame（等待对方按下准备按钮，然后开始游戏）
enum playerStatus
{
    playingGame,
    readyToPlayingGame
};
 // 用户程序之间交互协议
enum comm_request_type
{
    //连接服务器失败
    COMM_SERVER_CONN_FAILED,
    //连接服务器成功
    COMM_SERVER_CONN_SUCCESSFUL,
    //服务器关闭
    COMM_SERVER_CLOSE,
    //服务器发送大厅信息
    COMM_SERVER_GAMEINFO,
    //退出游戏
    COMM_CLIENT_QUITGAME,
    //请求断开服务器
    COMM_CLIENT_DISCONN,
    //客户端请求连接服务器
    COMM_CLIENT_CONN,
    //客户端请求创建主机
    COMM_CLIENT_CREATE,
    //客户端请求加入游戏
    COMM_CLIENT_JOIN,
    //双方准备完毕，游戏开始
    COMM_CLIENT_GAMESTART,
    //某方玩家胜利，游戏结束
    COMM_CLIENT_GAMEOVER,
    //玩家游戏操作：落子
 //    COMM_CLIENT_GAMEOP,
 //    //玩家游戏操作：发送聊天信息
 //    COMM_CLIENT_CHAT,
 //    //玩家游戏操作：悔棋
 //    COMM_CLIENT_UNDO,
 //    //玩家游戏操作：悔棋回复yes
 //    COMM_CLIENT_UNDO_YES,
 //    //玩家游戏操作：悔棋回复no
 //    COMM_CLIENT_UNDO_NO,
    //玩家游戏操作：认输
    COMM_CLIENT_LOSE,
    //玩家发来消息，对方掉线
    COMM_CLIENT_LOSTCONN
};
 // 用户程序之间交互数据包的格式
struct msg_request_struct
{
    //请求类型
    qint8 request;//qint8一种数据类型  /* 8 bit signed */
    //数据
    QString data;
};

#endif // DATABASE_H
