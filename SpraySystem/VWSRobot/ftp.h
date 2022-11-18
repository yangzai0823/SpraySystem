#include <string>
#include "tcpClient.h"
#include <list>
class MyFFTP
{
private:
    TcpClient *tcp_ = nullptr; 
private:
    bool orderOK(const std::string& str);
    int ParseString(std::list<std::string> strArray, unsigned long &nPort, std::string &strServerIp);
    int getIPPort(const char *str,unsigned long &nPort, std::string &strServerIp);
public:
    MyFFTP(/* args */);
    ~MyFFTP();
    int loginToServer(const std::string &serverIP,const std::string &userName,const std::string &password);
    int delFile(const std::string &fileName);
    int uploadFile(const std::string &fileName,const std::string &taskData);
};


