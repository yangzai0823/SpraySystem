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
const std::string VWSRobot::OrderName::IOWRITE = "HOSTCTRL_REQUEST IOREAD ";

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
const std::string VWSRobot::RespondVar::IOWRITE = "OK: IOREAD\r\n";

const std::string VWSRobot::PROGRAMNAME = "MAIN";
VWSRobot::VWSRobot(/* args */) {
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
    map[OrderName::IOWRITE] = RespondVar::IOWRITE;
}

VWSRobot::~VWSRobot() {
}

std::vector<double> VWSRobot::getNumFromStr(const std::string& str) {
    std::regex t("\\-?\\d+\\.?\\d*");
    std::string strtmp = str;
    std::smatch match;
    std::vector<double> var;
    while (std::regex_search(strtmp, match, t)) {
        // std::cout<<match.str()<<std::endl;
        var.push_back(std::stof(match.str()));
        strtmp = match.suffix();
    }
    return var;
}

bool VWSRobot::orderOK(TcpClient& tcp, const std::string& str) {
    // return true;
    char* reStr;
    int len;
    if (tcp.receiveData(reStr, len) == -1)
        return false;
    if (str.size() != len) {
        return false;
    }
    return strncmp((char*)reStr, str.c_str(), len) == 0 ? true : false;
}

int VWSRobot::orderExe(TcpClient& tcp, const std::string& order, bool needSure) {
    tcp.sendData(order);
    if (needSure) {
        if (!orderOK(tcp, map[order]))
            return -1;
    }
    return 1;
}

int VWSRobot::orderExe(TcpClient& tcp, const std::string& req, const std::string& order, bool needSure) {
    std::string tmp;
    if (req == OrderName::GETPOS) {
        tmp = req + std::to_string(order.size()) + "\r";
    } else {
        tmp = req + std::to_string(order.size()) + "\r\n";
    }
    tcp.sendData(tmp);
    if (!orderOK(tcp, map[req]))
        return -1;
    tcp.sendData(order);
    if (needSure) {
        if (!orderOK(tcp, "0000\r\n"))
            return -1;
    }
    return 1;
}

int VWSRobot::createFile(std::string& fileData, const std::vector<RobotTask>& taskData) {
    fileData = "/JOB\r\n";
    fileData += "//NAME RUNCOM\r\n";
    fileData += "//POS\r\n";
    fileData += "///NPOS 0,0,0,0,0,0\r\n";
    fileData += "///TOOL 0\r\n";
    fileData += "///POSTYPE PULSE\r\n";
    fileData += "///PULSE\r\n";
    int num = -1;
    std::string speedStr;
    std::string IOStr;
    for (auto&& track : (taskData)) {
        if (track.task == VWSRobot::TaskType::MOVEABSJ) {
            float speed = (track.task == VWSRobot::TaskType::MOVEABSJ) ? track.speed[0] : track.speed[1];
            speed = speed * SPEEDRATIO;
            int posNum = 0;
            for (auto&& pos : track.track) {
                posNum++;
                if (posNum == 6)
                    break;
                num++;
                std::string posStr = "P";
                std::string speedtmpStr = "SET D";
                if (num < 10) {
                    posStr += "0000" + std::to_string(num) + "=";
                    speedtmpStr += "0000" + std::to_string(num) + " " + std::to_string(speed) + "\r\n";
                } else if (num >= 10 && num < 100) {
                    posStr += "000" + std::to_string(num) + "=";
                    speedtmpStr += "000" + std::to_string(num) + " " + std::to_string(speed) + "\r\n";
                } else if (num >= 100 && num < 1000) {
                    posStr += "00" + std::to_string(num) + "=";
                    speedtmpStr += "00" + std::to_string(num) + " " + std::to_string(speed) + "\r\n";
                } else if (num >= 1000 && num < 1024) {
                    posStr += "0" + std::to_string(num) + "=";
                    speedtmpStr += "0" + std::to_string(num) + " " + std::to_string(speed) + "\r\n";
                } else {
                    return -1;
                }
                int tmpf = 0;
                for (auto&& var : pos) {
                    posStr += std::to_string(var * JOINTDATIO[tmpf]) + ",";
                    tmpf++;
                }
                posStr += "\r\n";
                fileData += posStr;
                speedStr += speedtmpStr;
            }
        } else if (track.IO == VWSRobot::TaskType::IO) {
            std::string ioTmpStr = "SET B";
            if (num < 10) {
                ioTmpStr += "0000 " + std::to_string(num) + " " + std::to_string(2 - track.Singal) + "\r\n";
            } else if (num >= 10 && num < 100) {
                ioTmpStr += "000 " + std::to_string(num) + " " + std::to_string(2 - track.Singal) + "\r\n";
            } else if (num >= 100 && num < 1000) {
                ioTmpStr += "00 " + std::to_string(num) + " " + std::to_string(2 - track.Singal) + "\r\n";
            } else if (num >= 1000 && num < 1024) {
                ioTmpStr += "0 " + std::to_string(num) + " " + std::to_string(2 - track.Singal) + "\r\n";
            } else {
                return -1;
            }
            IOStr += ioTmpStr;
        } else {
            return -1;
        }
    }

    fileData += "//INST\r\n";
    fileData += "///DATE 2022/11/16 14:53\r\n";
    fileData += "///ATTR SC,RW\r\n";
    fileData += "///GROUP1 RB1\r\n";
    fileData += "NOP\r\n";
    fileData += "DOUT OT#(0473) ON\r\n";
    fileData += "'P NUMBER\r\n";
    fileData += "SET I1021 0\r\n";
    // fileData += "SET I1020 100\r\n";
    fileData += "SET I1020 " + std::to_string(num) + "\r\n";
    fileData += "'IO CONTROL\r\n";
    // fileData += "SET B045 1\r\n";
    // fileData += "SET B055 2\r\n";
    fileData += IOStr;
    fileData += "'SET SPEED\r\n";
    // fileData += "SET D000 42\r\n";
    // fileData += "SET D001 43\r\n";
    // fileData += "SET D002 44\r\n";
    // fileData += "SET D003 45\r\n";
    fileData += speedStr;
    fileData += "WHILEEXP I1021<I1020\r\n";
    fileData += "MOVJ P[I1021] VJ=D[I1021]\r\n";
    fileData += "IFTHENEXP B[I1021]=1\r\n";
    fileData += "	DOUT OT#(0474) ON\r\n";
    fileData += "	SET B[I1021] 0\r\n";
    fileData += "ENDIF\r\n";
    fileData += "IFTHENEXP B[I1021]=2\r\n";
    fileData += "	DOUT OT#(0474) OFF\r\n";
    fileData += "	SET B[I1021] 0\r\n";
    fileData += "ENDIF\r\n";
    fileData += "ADD I1021 1\r\n";
    fileData += "ENDWHILE\r\n";
    fileData += "DOUT OT#(0473) OFF\r\n";
    fileData += "END\r\n";
    return 1;
}

int VWSRobot::Init(const std::string& ip, int port) {
    this->ip = ip;
    this->port = port;
    TcpClient tcp;
    if (tcp.toConnect(ip, port) == 1) {
        return orderExe(tcp, OrderName::CONNECT);
    }
    return -1;
}
int VWSRobot::state() {
    TcpClient tcp;
    if (tcp.toConnect(ip, port) == 1) {
        return orderExe(tcp, OrderName::CONNECT);
    }
    return -1;
}
int VWSRobot::start() {
    TcpClient tcp;
    if (tcp.toConnect(ip, port) == -1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECTALIVE) == -1)
        return -1;

    std::string order1 = PROGRAMNAME + ", 0\r";
    if (orderExe(tcp, OrderName::PROGRAMCALL, order1) == -1)
        return -1;
    std::string order2 = "1\r";
    if (orderExe(tcp, OrderName::SERVOON, order2) == -1)
        return -1;
    if (orderExe(tcp, OrderName::ClEARALARM) == -1)
        return -1;

    if (!orderOK(tcp, std::string("0000\r\n")))
        return -1;
    std::string order = PROGRAMNAME + "\r";
    if (orderExe(tcp, OrderName::STARTJOB, order) == -1)
        return -1;
    return 1;
}
int VWSRobot::close() {
    return 1;
}
int VWSRobot::sendData(const std::vector<RobotTask>& taskData) {
    // time_t timep,tim2;
    // time (&timep);
    std::string taskStr;
    if (createFile(taskStr, taskData) < 0)
        return -1;
    MyFFTP ftp;
    if (ftp.loginToServer(ip, "ftp", "") < 0) {
        return -1;
    }
    if (ftp.delFile("RUNCOM.JBI") < 0) {
        return -1;
    }
    if (ftp.uploadFile("RUNCOM.JBI", taskStr) < 0)
        return -1;
    // if (this->start() < 0)
    //     return -1;
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
int VWSRobot::getRobotPosition(RobotPosition& data, int coordNum, int tcpNum) {
    TcpClient tcp;
    if (tcp.toConnect(ip, port) == -1)
        return -1;
    if (orderExe(tcp, OrderName::CONNECTALIVE) == -1)
        return -1;
    // std::string order = "0,0\r";
    std::string order = std::to_string(coordNum) + "," + std::to_string(tcpNum) + "\r";
    if (orderExe(tcp, OrderName::GETPOS, order, false) == -1)
        return -1;
    char* posChar;
    char* jointChar;
    int len;
    if (tcp.receiveData(posChar, len) == -1)
        return -1;
    std::string str(posChar, len);
    if (orderExe(tcp, OrderName::GETJOINT) == -1)
        return -1;
    if (tcp.receiveData(jointChar, len) == -1)
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
