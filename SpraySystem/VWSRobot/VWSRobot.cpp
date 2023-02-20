#include "VWSRobot.h"

#include <time.h>

#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <regex>

#include "ftp.h"
#define SPEEDRATIO (10000.0 / 120.0)
#define JOINTDATIO0 1942.957577694336
#define JOINTDATIO1 -1942.872296094253
#define JOINTDATIO2 2410.385886166698
#define JOINTDATIO3 -1115.040075436115
#define JOINTDATIO4 -878.9336967167738
#define JOINTDATIO5 -527.6444772183021
static const double JOINTDATIO[6] = {
    JOINTDATIO0, JOINTDATIO1, JOINTDATIO2, JOINTDATIO3, JOINTDATIO4, JOINTDATIO5};
const std::string VWSRobot::OrderName::CONNECT = "CONNECT Robot_access\r\n";
const std::string VWSRobot::OrderName::CONNECTALIVE = "CONNECT Robot_access Keep-Alive:-1\r\n";
const std::string VWSRobot::OrderName::ClEARALARM = "HOSTCTRL_REQUEST CANCEL 0\r\n";
const std::string VWSRobot::OrderName::STARTJOB = "HOSTCTRL_REQUEST START ";
const std::string VWSRobot::OrderName::PROGRAMCALL = "HOSTCTRL_REQUEST JSEQ ";
const std::string VWSRobot::OrderName::SERVOON = "HOSTCTRL_REQUEST SVON ";
const std::string VWSRobot::OrderName::SERVOOFF = "HOSTCTRL_REQUEST SVON ";
const std::string VWSRobot::OrderName::GETPOS = "HOSTCTRL_REQUEST RPOSC ";
const std::string VWSRobot::OrderName::GETJOINT = "HOSTCTRL_REQUEST RPOSJ 0\r\n";
const std::string VWSRobot::OrderName::WRITEVAR = "HOSTCTRL_REQUEST LOADV ";
const std::string VWSRobot::OrderName::IOREAD = "HOSTCTRL_REQUEST IOREAD ";
const std::string VWSRobot::OrderName::IOWRITE = "HOSTCTRL_REQUEST IOWRITE ";
const std::string VWSRobot::OrderName::READSTATUS = "HOSTCTRL_REQUEST RSTATS 0\r\n";
const std::string VWSRobot::OrderName::HOLD = "HOSTCTRL_REQUEST HOLD ";

const std::string VWSRobot::RespondVar::CONNECT = "OK: DX Information Server( 1.07).\r\n";
const std::string VWSRobot::RespondVar::CONNECTALIVE = "OK: DX Information Server( 1.07) Keep-Alive:-1.\r\n";
const std::string VWSRobot::RespondVar::ClEARALARM = "OK: CANCEL\r\n";
const std::string VWSRobot::RespondVar::STARTJOB = "OK: START\r\n";
const std::string VWSRobot::RespondVar::PROGRAMCALL = "OK: JSEQ\r\n";
const std::string VWSRobot::RespondVar::SERVOON = "OK: SVON\r\n";
const std::string VWSRobot::RespondVar::SERVOOFF = "OK: SVON\r\n";
const std::string VWSRobot::RespondVar::GETPOS = "OK: RPOSC\r\n";
const std::string VWSRobot::RespondVar::GETJOINT = "OK: RPOSJ\r\n";
const std::string VWSRobot::RespondVar::WRITEVAR = "OK: LOADV\r\n";
const std::string VWSRobot::RespondVar::IOREAD = "OK: IOREAD\r\n";
const std::string VWSRobot::RespondVar::IOWRITE = "OK: IOWRITE\r\n";
const std::string VWSRobot::RespondVar::READSTATUS = "OK: RSTATS\r\n";
const std::string VWSRobot::RespondVar::HOLD = "OK: HOLD\r\n";

const std::string VWSRobot::INITPROGRAM = "INIT";
const std::string VWSRobot::PROGRAMNAME = "MAINT";
VWSRobot::VWSRobot(/* args */)
{
    map[OrderName::CONNECT] = RespondVar::CONNECT;
    map[OrderName::CONNECTALIVE] = RespondVar::CONNECTALIVE;
    map[OrderName::ClEARALARM] = RespondVar::ClEARALARM;
    map[OrderName::STARTJOB] = RespondVar::STARTJOB;
    map[OrderName::PROGRAMCALL] = RespondVar::PROGRAMCALL;
    map[OrderName::SERVOON] = RespondVar::SERVOON;
    map[OrderName::SERVOOFF] = RespondVar::SERVOOFF;
    map[OrderName::GETPOS] = RespondVar::GETPOS;
    map[OrderName::GETJOINT] = RespondVar::GETJOINT;
    map[OrderName::WRITEVAR] = RespondVar::WRITEVAR;
    map[OrderName::IOREAD] = RespondVar::IOREAD;
    map[OrderName::IOWRITE] = RespondVar::IOWRITE;
    map[OrderName::READSTATUS] = RespondVar::READSTATUS;
    map[OrderName::HOLD] = RespondVar::HOLD;
}

VWSRobot::~VWSRobot()
{
}

std::vector<double> VWSRobot::getNumFromStr(const std::string &str)
{
    std::regex t("\\-?\\d+\\.?\\d*");
    std::string strtmp = str;
    std::smatch match;
    std::vector<double> var;
    while (std::regex_search(strtmp, match, t))
    {
        // std::cout<<match.str()<<std::endl;
        var.push_back(std::stof(match.str()));
        strtmp = match.suffix();
    }
    return var;
}

bool VWSRobot::orderOK(TcpClient &tcp, const std::string &str)
{
    // return true;
    char *reStr;
    int len;
    if (tcp.receiveData(reStr, len) != 1)
        return false;
    if (str.size() != len)
    {
        return false;
    }
    return strncmp((char *)reStr, str.c_str(), len) == 0 ? true : false;
}

int VWSRobot::orderExe(TcpClient &tcp, const std::string &order, bool needSure)
{
    tcp.sendData(order);
    if (needSure)
    {
        if (!orderOK(tcp, map[order]))
            return -1;
    }
    return 1;
}

int VWSRobot::orderExe(TcpClient &tcp, const std::string &req, const std::string &order, bool needSure)
{
    std::string tmp;
    if (req == OrderName::GETPOS)
    {
        tmp = req + std::to_string(order.size()) + "\r";
    }
    else
    {
        tmp = req + std::to_string(order.size()) + "\r\n";
    }
    tcp.sendData(tmp);
    if (!orderOK(tcp, map[req]))
        return -1;
    tcp.sendData(order);
    if (needSure)
    {
        if (!orderOK(tcp, "0000\r\n"))
            return -1;
    }
    return 1;
}

int VWSRobot::createFile(std::string &fileData, const std::vector<RobotTask> &taskData)
{
    fileData = "/JOB\r\n";
    fileData += "//NAME " + VWSRobot::PROGRAMNAME + "\r\n";
    fileData += "//POS\r\n";
    fileData += "///NPOS 0,0,0,0,0,0\r\n";
    fileData += "///TOOL 0\r\n";
    fileData += "///POSTYPE PULSE\r\n";
    fileData += "///PULSE\r\n";
    int numt[4] = {0};
    int num = 0;
    std::string speedStr;
    std::string IOStr;
    std::string posStr;
    for (auto &&task : (taskData))
    {
        if (task.taskType >= TaskType::track_1 && task.taskType <= TaskType::track_4)
        {
            float speed = task.speed[0] * SPEEDRATIO;
            std::string posStr = "P";
            std::string speedtmpStr = "SET D";
            if (num < 10)
            {
                posStr += "0000" + std::to_string(num) + "=";
                speedtmpStr += "0000" + std::to_string(num) + " " + std::to_string((int)speed) + "\r\n";
            }
            else if (num >= 10 && num < 100)
            {
                posStr += "000" + std::to_string(num) + "=";
                speedtmpStr += "000" + std::to_string(num) + " " + std::to_string((int)speed) + "\r\n";
            }
            else if (num >= 100 && num < 1000)
            {
                posStr += "00" + std::to_string(num) + "=";
                speedtmpStr += "00" + std::to_string(num) + " " + std::to_string((int)speed) + "\r\n";
            }
            else if (num >= 1000 && num < 1024)
            {
                posStr += "0" + std::to_string(num) + "=";
                speedtmpStr += "0" + std::to_string(num) + " " + std::to_string((int)speed) + "\r\n";
            }
            else
            {
                return -1;
            }
            int tmpf = 0;
            int posNum = 0;
            for (auto &&var : task.point)
            {
                posNum++;
                posStr += std::to_string(int(var * JOINTDATIO[tmpf]));
                if (posNum == 6)
                    break;
                posStr += +",";
                tmpf++;
            }
            posStr += "\r\n";
            fileData += posStr;
            speedStr += speedtmpStr;
            numt[task.taskType]++;
            num++;
        }
        else if (task.IO == VWSRobot::TaskType::IO)
        {
            std::string ioTmpStr = "SET B";
            if (num < 10)
            {
                ioTmpStr += "0000 " + std::to_string(num) + " " + std::to_string(2 - task.Singal) + "\r\n";
            }
            else if (num >= 10 && num < 100)
            {
                ioTmpStr += "000 " + std::to_string(num) + " " + std::to_string(2 - task.Singal) + "\r\n";
            }
            else if (num >= 100 && num < 1000)
            {
                ioTmpStr += "00 " + std::to_string(num) + " " + std::to_string(2 - task.Singal) + "\r\n";
            }
            else if (num >= 1000 && num < 1024)
            {
                ioTmpStr += "0 " + std::to_string(num) + " " + std::to_string(2 - task.Singal) + "\r\n";
            }
            else
            {
                return -1;
            }
            IOStr += ioTmpStr;
        }
        else
        {
            return -1;
        }
    }

    fileData = fileData + "//INST\r\n" + "///DATE 2022/11/24 09:47\r\n" + "///ATTR SC,RW\r\n" + "///GROUP1 RB1\r\n" + "NOP\r\n" + "DOUT OT#(0001) ON\r\n" + "'number of pointers\r\n" + "SET I000 0\r\n" + "SET I001 " + std::to_string(numt[0]) + "\r\n" + "SET I002 " + std::to_string(numt[1]) + "\r\n" + "SET I003 " + std::to_string(numt[2]) + "\r\n" + "SET I004 " + std::to_string(numt[3]) + "\r\n" + "'path SELECT WHILE  CONTROL\r\n" + "'I0006 SET1 MEAN way 2to3\r\n" + "'I0007 set1 mean way 4\r\n" + "SET I0005 1\r\n" + "'IO CONTROL,1 mean on,2 mean off\r\n"
               //  +  "SET B045 1\r\n" //开关枪IO
               //  +  "SET B055 2\r\n"
               + IOStr + "'SET SPEED\r\n"
               //  +  "SET D000 42\r\n"
               //  +  "SET D001 43\r\n"
               //  +  "SET D002 44\r\n"
               //  +  "SET D003 45\r\n"
               + speedStr + "'.....\r\n" + "\t\t'-----------WAY1----------\r\n" + "\t\tWHILEEXP I000<I001\r\n" + "\t\t\tMOVJ P[I000] VJ=D[I000] NWAIT\r\n" + "\t\t\tADD I000 1\r\n" + "\t\tENDWHILE\r\n" + "\t\t'--------WAY1 END------\r\n" + "WHILEEXP I005=1\r\n" + "\tIFTHENEXP IN#(1)=ON\r\n" + "\t\tSET I005 0\r\n" + "\t\tSET I006 0\r\n" + "SET I000 0\r\n" + "\t\tADD I000 I001\r\n" + "\t\t'-----------WAY2----------\r\n" + "\t\tWHILEEXP I000<I002\r\n" + "\t\t\tMOVJ P[I000] VJ=D[I000] NWAIT\r\n"
               //  +  "\t\t\tIFTHENEXP B[I000]=1\r\n"
               //  +  "\t\t\t\tDOUT OT#(2) ON\r\n"
               //  +  "\t\t\t\tSET B[I000] 0\r\n"
               //  +  "\t\t\tENDIF\r\n"
               //  +  "\t\t\tIFTHENEXP B[I000]=2\r\n"
               //  +  "\t\t\t\tDOUT OT#(2) OFF\r\n"
               //  +  "\t\t\t\tSET B[I000] 0\r\n"
               //  +  "\t\t\tENDIF\r\n"
               + "\t\t\tADD I000 1\r\n" + "\t\tENDWHILE\r\n" + "\t\t'--------WAY2 END------\r\n" + "\t\tPULSE OT#(0002)\r\n" + "\t\t'----------------------\r\n" + "\t\t'********WAY3*********\r\n" + "SET I000 0\r\n" + "\tADD I000 I001\r\n" + "\tADD I000 I002\r\n" + "\t\tWHILEEXP I000<I003\r\n" + "\t\t\tMOVJ P[I000] VJ=D[I000] NWAIT\r\n" + "\t\t\tADD I000 1\r\n" + "\t\tENDWHILE\r\n" + "\tENDIF\r\n" + "\t'********WAY3 END*********\r\n" + "\t'SELECT WAY4\r\n" + "\tIFTHENEXP IN#(2)=ON\r\n" + "\t\tSET I005 0\r\n" + "\t\tSET I000 0\r\n" + "\t\tPULSE OT#(0002)\r\n" + "\tADD I000 I001\r\n" + "\t\tADD I000 I002\r\n" + "\t\tADD I000 I003\r\n" + "\t\tWHILEEXP I000<I004\r\n" + "\t\t\tMOVJ P[I000] VJ=D[I000] NWAIT\r\n" + "\t\t\tADD I000 1\r\n" + "\t\tENDWHILE\r\n" + "\tENDIF\r\n" + "\t'********WAY4 END*********\r\n" + "ENDWHILE\r\n" + "DOUT OT#(0001) OFF\r\n" + "END\r\n";
    return 1;
}

int VWSRobot::Init(const std::string &ip, int port)
{
#ifdef LOCALTEST
    return 1;
#endif
    this->ip = ip;
    this->port = port;
    TcpClient tcp;
    if (tcp.toConnect(ip, port) == 1)
    {
        return orderExe(tcp, OrderName::CONNECT);
    }
    return -1;
}
int VWSRobot::state()
{
#ifdef LOCALTEST
    return 1;
#endif
    TcpClient tcp;
    if (tcp.toConnect(ip, port) == 1)
    {
        return orderExe(tcp, OrderName::CONNECT);
    }
    return -1;
}
int VWSRobot::start()
{
#ifdef LOCALTEST
    return 1;
#endif
    TcpClient tcp;
    if (tcp.toConnect(ip, port) != 1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECTALIVE) != 1)
        return -1;
    if (orderExe(tcp, OrderName::ClEARALARM) != 1)
        return -1;

    if (!orderOK(tcp, std::string("0000\r\n")))
        return -1;

    VWSRobot::RobotStatus status;
    if (getRobotStatus(status) != 1)
    {
        return -1;
    }

    uint8_t servonSta = status.data_2 & 0x40;
    if (servonSta != 0x40)
    {
        std::string order2 = "1\r";
        if (orderExe(tcp, OrderName::SERVOON, order2) != 1)
        {
            if (getRobotStatus(status) != 1)
            {
                return -1;
            }
            if (status.data_2 & 0x20 != 0x20)
            {
                return -1;
            }
        }
    }

    if ((int)(status.data_1 & 0x08) == 0x08)
    {
        if (orderExe(tcp, OrderName::HOLD, std::string("1\r")) != 1)
            return -1;
    }

    std::string order1 = INITPROGRAM + ", 0\r";
    if (orderExe(tcp, OrderName::PROGRAMCALL, order1) != 1)
        return -1;

    if (orderExe(tcp, OrderName::HOLD, std::string("0\r")) != 1)
        return -1;

    std::string order = INITPROGRAM + "\r";
    if (orderExe(tcp, OrderName::STARTJOB, order) != 1)
        return -1;
    return 1;
}
int VWSRobot::close()
{
#ifdef LOCALTEST
    return 1;
#endif
    TcpClient tcp;
    if (tcp.toConnect(ip, port) != 1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECTALIVE) != 1)
        return -1;
    VWSRobot::RobotStatus status;
    if (getRobotStatus(status) != 1)
    {
        return -1;
    }
    if ((int)(status.data_1 & 0x08) == 0x08)
    {
        if (orderExe(tcp, OrderName::HOLD, std::string("1\r")) != 1)
            return -1;
    }
    if ((int)(status.data_2 & 0x40) == 0x40)
    {
        if (orderExe(tcp, OrderName::SERVOOFF, std::string("0\r")) != 1)
            return -1;
    }
    return 1;
}

int VWSRobot::sendData(const std::vector<RobotTask> &taskData)
{
#ifdef LOCALTEST
    return 1;
#endif
    // time_t timep,tim2;
    // time (&timep);
    std::string taskStr;
    if (createFile(taskStr, taskData) < 0)
        return -1;
    // std::cout<<taskStr<<std::endl;
    MyFFTP ftp;
    if (ftp.loginToServer(ip, "ftp", "") < 0)
    {
        return -1;
    }
    if (ftp.delFile(VWSRobot::PROGRAMNAME + ".JBI") < 0)
    {
        // return -1;
    }
    if (ftp.uploadFile(VWSRobot::PROGRAMNAME + ".JBI", taskStr) < 0)
        return -1;
    TcpClient tcp;
    if (tcp.toConnect(ip, port) != 1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECTALIVE) != 1)
        return -1;
    VWSRobot::RobotStatus status;
    if (getRobotStatus(status) != 1)
    {
        return -1;
    }
    if ((int)(status.data_1 & 0x08) == 0x08)
    {
        if (orderExe(tcp, OrderName::HOLD, std::string("1\r")) != 1)
            return -1;
    }

    std::string order1 = PROGRAMNAME + ", 0\r";
    if (orderExe(tcp, OrderName::PROGRAMCALL, order1) != 1)
        return -1;

    if (orderExe(tcp, OrderName::HOLD, std::string("0\r")) != 1)
        return -1;
    while (1)
    {
        std::string order = PROGRAMNAME + "\r";
        if (orderExe(tcp, OrderName::STARTJOB, order) != 1)
            return -1;
        if (getRobotStatus(status) != 1)
        {
            return -1;
        }
        if ((int)(status.data_1 & 0x08) == 0x08)
        {
            return 1;
        }
    }
    return 1;
    // time (&tim2);
    // std::cout<<"time   "<<tim2 - timep<<std::endl;
}
/**
 * @description: 获取当前位置，角度精度大概0.1度
 * @param {RobotPosition&} data
 * @param {int} coordNum
 *  Specification of coordinate system
 *  0: Base coordinate
 *  1: Robot coordinate
 *  2: User coordinate 1
 *  65: User coordinate 64
 * @param {int} tcpNum
 * With or Without external axis
 *  0: Without external axis
 *  1: With external axis
 * @return {*}
 * @author: joker
 */
int VWSRobot::getRobotPosition(RobotPosition &data, int coordNum, int tcpNum)
{
    TcpClient tcp;
    if (tcp.toConnect(ip, port) != 1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECTALIVE) != 1)
        return -1;
    // std::string order = "0,0\r";
    std::string order = std::to_string(coordNum) + "," + std::to_string(tcpNum) + "\r";
    if (orderExe(tcp, OrderName::GETPOS, order, false) != 1)
        return -1;
    char *posChar;
    char *jointChar;
    int len;
    if (tcp.receiveData(posChar, len) != 1)
        return -1;
    std::string str(posChar, len);
    if (orderExe(tcp, OrderName::GETJOINT) != 1)
        return -1;
    if (tcp.receiveData(jointChar, len) != 1)
        return -1;
    std::string jointStr(jointChar, len);
    auto joint = getNumFromStr(jointStr);
    auto pos = getNumFromStr(str);
    if (pos.size() < 8 || joint.size() != 12)
        return -1;
    data.pos[0] = pos[0];
    data.pos[1] = pos[1];
    data.pos[2] = pos[2];

    Eigen::AngleAxisd rollAngle(pos[3], Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd pitchAngle(pos[4], Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd yawAngle(pos[5], Eigen::Vector3d::UnitZ());

    Eigen::Quaterniond quaternion;
    quaternion = yawAngle * pitchAngle * rollAngle;
    data.orient[0] = quaternion.w();
    data.orient[1] = quaternion.x();
    data.orient[2] = quaternion.y();
    data.orient[3] = quaternion.z();

    data.RobJoint[0] = joint[0] / JOINTDATIO0;
    data.RobJoint[1] = joint[1] / JOINTDATIO1;
    data.RobJoint[2] = joint[2] / JOINTDATIO2;
    data.RobJoint[3] = joint[3] / JOINTDATIO3;
    data.RobJoint[4] = joint[4] / JOINTDATIO4;
    data.RobJoint[5] = joint[5] / JOINTDATIO5;
    return 1;
}
int VWSRobot::getRobotStatus(RobotStatus &status)
{
    TcpClient tcp;
    if (tcp.toConnect(ip, port) != 1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECT) != 1)
        return -1;
    orderExe(tcp, OrderName::READSTATUS);
    char *statusStr;
    int len;
    if (tcp.receiveData(statusStr, len) != 1)
        return -1;
    auto statusNum = getNumFromStr(statusStr);
    if (statusNum.size() != 2)
        return -1;
    status.data_1 = statusNum[0];
    status.data_2 = statusNum[1];
    return 1;
}
