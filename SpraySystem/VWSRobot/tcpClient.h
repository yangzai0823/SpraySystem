#ifndef TCPAPP_H
#define TCPAPP_H
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <thread>
class TcpClient
{
private:
    int client_fd_ = 0;
    char *revBuf = nullptr;
    fd_set rfds_;
    struct timeval tv_;
    int retval_, maxfd_;
public:
    TcpClient(/* args */);
    ~TcpClient();
    int toConnect(std::string ip,int port);
    int sendData(const void *buf,int datalen);
    int sendData(const std::string &buf);
    int receiveData(char* &buf,int &datalen);
};



#endif // TCPAPP_H