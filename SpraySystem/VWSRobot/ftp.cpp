#include "ftp.h"
#define FTP_DEFAULT_PORT 21  // FTP默认端口号
MyFFTP::MyFFTP(/* args */) {
}

MyFFTP::~MyFFTP() {
    if (tcp_)
        delete tcp_;
}
static int SplitString(std::string strSrc, std::list<std::string> &strArray, std::string strFlag) {
    int pos = 1;
    while ((pos = (int)strSrc.find_first_of(strFlag.c_str())) > 0) {
        strArray.insert(strArray.end(), strSrc.substr(0, pos));
        strSrc = strSrc.substr(pos + 1, strSrc.length() - pos - 1);
    }

    strArray.insert(strArray.end(), strSrc.substr(0, strSrc.length()));

    return 0;
}
bool MyFFTP::orderOK(const std::string &str) {
    // return true;
    char *reStr;
    int len;
    if (tcp_->receiveData(reStr, len) == -1)
        return false;
    if (str.size() != len) {
        return false;
    }
    return strncmp((char *)reStr, str.c_str(), len) == 0 ? true : false;
}

int MyFFTP::ParseString(std::list<std::string> strArray, unsigned long &nPort, std::string &strServerIp) {
    if (strArray.size() < 6)
        return -1;

    std::list<std::string>::iterator citor;
    citor = strArray.begin();
    strServerIp = *citor;
    strServerIp += ".";
    citor++;
    strServerIp += *citor;
    strServerIp += ".";
    citor++;
    strServerIp += *citor;
    strServerIp += ".";
    citor++;
    strServerIp += *citor;
    citor = strArray.end();
    citor--;
    nPort = atol((*citor).c_str());
    citor--;
    nPort += atol((*(citor)).c_str()) * 256;
    return 0;
}

int MyFFTP::getIPPort(const char *str, unsigned long &nPort, std::string &strServerIp) {
    std::string parseStr(str);

    size_t nBegin = parseStr.find_first_of("(");
    size_t nEnd = parseStr.find_first_of(")");
    std::string strData = parseStr.substr(nBegin + 1, nEnd - nBegin - 1);
    std::list<std::string> strArray;
    if (SplitString(strData, strArray, ",") < 0)
        return -1;
    if (ParseString(strArray, nPort, strServerIp) < 0)
        return -1;
}

int MyFFTP::loginToServer(const std::string &serverIP, const std::string &userName, const std::string &password) {
    tcp_ = new TcpClient();
    if (tcp_->toConnect(serverIP, FTP_DEFAULT_PORT) != 1)
        return -1;
    if (!orderOK("220 DX FTP server ( 1.07) ready.\r\n"))
        return -1;
    std::string userStr = "USER " + userName + "\r\n";
    if (tcp_->sendData(userStr) == -1)
        return -1;
    if (!orderOK("331 Password required for ftp.\r\n"))
        return -1;
    std::string passStr = "PASS " + password + "\r\n";
    if (tcp_->sendData(passStr) == -1)
        return -1;
    if (!orderOK("230 User ftp logged in.\r\n"))
        return -1;
    return 1;
}
int MyFFTP::delFile(const std::string &fileName) {
    std::string str = "DELE " + fileName + "\r\n";
    if (tcp_->sendData(str) == -1)
        return 1;
    if (!orderOK("250 DELE command successful.\r\n"))
        return -1;
}
int MyFFTP::uploadFile(const std::string &fileName, const std::string &taskData) {
    if (tcp_->sendData("PASV\r\n") == -1)
        return -1;
    char *data;
    int len;
    if (tcp_->receiveData(data, len) != 1)
        return -1;
    unsigned long nPort;
    std::string strServerIp;
    getIPPort(data, nPort, strServerIp);
    TcpClient *tcptmp = new TcpClient();
    if (tcptmp->toConnect(strServerIp, nPort) != 1)
        return -1;
    std::string fileStr = "STOR " + fileName + "\r\n";
    tcp_->sendData(fileStr);
    if (!orderOK("150 Opening ASCII mode data connection. (0,0,0,0,0)\r\n"))
        return -1;
    int nLen = taskData.size();
    const char *strBuf = taskData.c_str();
    int sendLen = 0;
    while (sendLen < nLen) {
        int thisLen = tcptmp->sendData(strBuf + sendLen, nLen - sendLen);
        if (thisLen < 0) {
            return -1;
        }
        sendLen += thisLen;
    }
    char *reStr;
    delete tcptmp;
    if (!orderOK("226 Transfer complete.\r\n"))
        return -1;
    return 1;
}