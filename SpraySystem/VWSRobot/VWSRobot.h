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
        track_1 = 0,
        track_2 = 1,
        track_3 = 2,
        track_4 = 3,
        IO = 4,
    };

    struct RobotTask {
        TaskType taskType;
        float speed[2];                           //第一位： 线速度mm/s 第二位：角速度  度/s
        std::array<float, 7> point;  // mm/s
        int IO;                                   // IO号
        int Singal;                               // IO操作
    };
    struct RobotStatus
    {
        uint8_t data_1;
        uint8_t data_2;
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
        static const std::string IOREAD;
        static const std::string IOWRITE;
        static const std::string READSTATUS;
        static const std::string HOLD;
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
        static const std::string IOREAD;
        static const std::string IOWRITE;
        static const std::string READSTATUS;
        static const std::string HOLD;
        /* data */
    };
    static const std::string PROGRAMNAME;
    static const std::string INITPROGRAM;
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
    int getRobotStatus(RobotStatus &status);
    int sendData(const std::vector<RobotTask> &taskData);
    int getRobotPosition(RobotPosition& data,int coordNum = 0,int tcpNum = 0);
};

#endif  // YASKAWA_H