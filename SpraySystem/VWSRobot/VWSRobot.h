#ifndef VWSROBOT_H
#define VWSROBOT_H
#include <Poco/Net/NetException.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/TCPServer.h>
#include "Lib/rws_state_machine_interface.h"
class VWSRobot {
private:
    abb::rws::RWSStateMachineInterface *abbObj_ = nullptr;
    int netState_ = 0;
    std::string selfIp_;
public:
    VWSRobot(/* args */);
    ~VWSRobot();
    enum TaskType{
        IO = 1,
        MOVEL = 2,
        MOVEJ = 3,
        MOVEABSJ = 4,
    };

    struct RobotTask
    {
        TaskType task;
        float speed[2]; //第一位： 线速度mm/s 第二位：角速度  度/s
        std::vector<std::array<float,7>> track;//mm/s
        int IO;//IO号
        int Singal;//IO操作
    };

    struct RobotPosition
    {
        float RobJoint[6];
        float pos[3];
        float orient[4];
    };
    
    
    int Init(std::string ip,int port = abb::rws::SystemConstants::General::DEFAULT_PORT_NUMBER);
    int state();
    int start();
    int close();
    int sendData(std::vector<RobotTask>taskData);
    int getRobotPosition(RobotPosition & data);

private:
    std::string getIp();
    Poco::Net::ServerSocket *serverSocket;
    Poco::Net::StreamSocket streamSocket;
};
#endif  // VWSROBOT_H
