/*
 * @Description: 
 * @Version: 1.0
 * @Autor: joker
 * @Date: 2022-11-07 09:17:17
 * @LastEditors: joker
 * @LastEditTime: 2022-11-11 10:33:32
 */
#include "tcpClient.h"
#include <iostream>
#define BUFFER_SIZE 1024
TcpClient::TcpClient(/* args */) {
    revBuf = new char[BUFFER_SIZE];
    memset(revBuf, 0, sizeof(char) * BUFFER_SIZE);
}

TcpClient::~TcpClient() {
    close(client_fd_);
    delete[] revBuf;
}
int TcpClient::toConnect(std::string ip, int port) {
    struct sockaddr_in server;
    socklen_t saddrlen = sizeof(server);
    memset(&server, 0, sizeof(server));
    client_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd_ < 0) {
        printf("socket error!\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    int retr = -1;
    if (connect(client_fd_, (struct sockaddr *)&server, saddrlen) < 0) {
        printf("connect failed!  %d\n", retr);
        return -1;
    }
    /*把可读文件描述符的集合清空*/
    FD_ZERO(&rfds_);
    /*把标准输入的文件描述符加入到集合中*/
    FD_SET(0, &rfds_);
    maxfd_ = 0;
    /*把当前连接的文件描述符加入到集合中*/
    FD_SET(client_fd_, &rfds_);
    /*找出文件描述符集合中最大的文件描述符*/
    if (maxfd_ < client_fd_)
        maxfd_ = client_fd_;
    /*设置超时时间*/
    tv_.tv_sec = 0;
    tv_.tv_usec = 1000000;
    // thd = new std::thread(tcpClientRun, std::ref(tcpOver), client_fd_);
    // thd->detach();
    return 1;
}
int TcpClient::sendData(const void *buf, int datalen) {
    //printf("%s\n",buf);
    return send(client_fd_, buf, datalen, 0) == datalen ? datalen : -1;
}
int TcpClient::sendData(const std::string &buf) {
    //std::cout<<buf<<std::endl;
    int datalen = buf.size();
    return send(client_fd_, buf.c_str(), datalen, 0) == datalen ? datalen : -1;
}
int TcpClient::receiveData(char* &buf, int &datalen) {
    memset(revBuf, 0, sizeof(char) * BUFFER_SIZE);
    auto tv = tv_;
    retval_ = select(maxfd_ + 1, &rfds_, NULL, NULL, &tv);
    if (retval_ == -1) {
        printf("select出错，客户端程序退出\n");
        return -1;
    } else if (retval_ == 0) {
        printf("客户端没有任何输入信息，并且服务器也没有信息到来，waiting...\n");
        return -1;
    } else {
        /*服务器发来了消息*/
        if (!FD_ISSET(client_fd_, &rfds_)) {
            return -1;
        }
    }
    int len = recv(client_fd_, revBuf, BUFFER_SIZE, 0);
    //std::cout<<revBuf<<std::endl;
    if (len <= 0)
        return -1;
    buf = revBuf;
    datalen = len;
    return 1;
}