#include "socketclient.h"
#include <fcntl.h>

socketclient::socketclient()
{

}

const int kMsgSize=1024;

int socketclient::init(char* ip,int port)
{

    try {

        client_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        //设置非阻塞
        int flags = fcntl(client_socket, F_GETFL, 0);
        fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

        //创建服务端地址数据
        sockaddr_in server_addr;
        //先将保存地址的server_addr置为0
        memset(&server_addr,0,sizeof(server_addr));
       //制定地质协议为ipv4
        server_addr.sin_family = AF_INET;
        //指定服务端端口
        server_addr.sin_port = htons(port);
        //指定服务端ipv4地址
        server_addr.sin_addr.s_addr = inet_addr(ip);
        //连接服务器，成功返回0，错误返回-1
        connect(client_socket,(sockaddr*)&server_addr,sizeof(server_addr));

        return 0;
    } catch(char* ex) {
        return -1;
    }
}

void socketclient::SendMsg(char* send_msg)
{
    send(client_socket,send_msg,sizeof(send_msg),0);
    std::cout<<"Send message: "<<send_msg<<std::endl;

    return_num = recv(client_socket,recv_msg_buf,kMsgSize,0);

    //return_num = read(client_socket,recv_msg_buf,kMsgSize);

    std::cout<<"Message from server: "
            <<recv_msg_buf<<std::endl;

    memset(recv_msg_buf,0,kMsgSize);
}
