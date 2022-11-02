#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <iostream>

class socketclient
{
public:
    socketclient();
    int init(char* ip,int port);
    void SendMsg(char* send_msg);

private:

    int client_socket;
    //创建接首数据的buf
    char recv_msg_buf[1024];
    //声明int型数据接受recv()的返回值
    int return_num;
};

#endif // SOCKETCLIENT_H
