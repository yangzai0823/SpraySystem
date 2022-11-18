#ifndef YASKAWA_H
#define YASKAWA_H
#include "tcpClient.h"
#include <vector>
#include <unordered_map>
class VWSRobot {
public:
    struct RobotPosition {
        float RobJoint[6];
        float pos[3];
        float orient[4];
    };
    enum TaskType {
        IO = 1,
        MOVEL = 2,
        MOVEJ = 3,
        MOVEABSJ = 4,
    };

    struct RobotTask {
        TaskType task;
        float speed[2];                           //第一位： 线速度mm/s 第二位：角速度  度/s
        std::vector<std::array<float, 7>> track;  // mm/s
        int IO;                                   // IO号
        int Singal;                               // IO操作
    };
private:
    struct OrderName {
        static const std::string CONNECT;
        static const std::string CONNECTALIVE;
        static const std::string ClEARALARM;
        static const std::string STARTJOB;
        static const std::string PROGRAMCALL;
        static const std::string SERVOON;
        static const std::string SERVOOFF;
        static const std::string GETPOS;
        static const std::string GETJOINT;
        static const std::string WRITEVAR;
        static const std::string IOWRITE;
    };
    
    struct RespondVar
    {
        static const std::string CONNECT;
        static const std::string CONNECTALIVE;
        static const std::string ClEARALARM;
        static const std::string STARTJOB;
        static const std::string PROGRAMCALL;
        static const std::string SERVOON;
        static const std::string SERVOOFF;
        static const std::string GETPOS;
        static const std::string GETJOINT;
        static const std::string WRITEVAR;
        static const std::string IOWRITE;
        /* data */
    };
    static const std::string PROGRAMNAME;
    std::unordered_map<std::string,std::string> map;  
    std::string ip;
    int port = 80;
private:
    bool orderOK(TcpClient &tcp,const std::string & str);
    std::vector<double> getNumFromStr(const std::string &str);
    int orderExe(TcpClient &tcp,const std::string &order,bool needSure = true);
    int orderExe(TcpClient &tcp,const std::string &req,const std::string &order,bool needSure = true);
    int createFile(std::string &fileData,const std::vector<RobotTask> &taskData);
public:
    VWSRobot(/* args */);
    ~VWSRobot();
    int Init(const std::string &ip, int port = 80);
    int state();
    int start();
    int close();
    int sendData(const std::vector<RobotTask> &taskData);
    int getRobotPosition(RobotPosition& data,int coordNum = 0,int tcpNum = 0);
};

#endif  // YASKAWA_H