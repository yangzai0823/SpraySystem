#include "VWSRobot.h"
#include "Lib/rws_common.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <cmath>
#include <thread>
#include <iostream>

VWSRobot::VWSRobot(/* args */) {
    selfIp_ = getIp();
    serverSocket = new Poco::Net::ServerSocket(9999);
}

VWSRobot::~VWSRobot() {
    if(abbObj_){
        delete abbObj_;
        abbObj_ = nullptr;
    }
    streamSocket.close();
    delete serverSocket;
}

std::string VWSRobot::getIp(){
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        family = ifa->ifa_addr->sa_family;
        if (strcmp(ifa->ifa_name, "lo") == 0)
            continue;
        if (family == AF_INET) {
            std::string tmp(inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr));
            freeifaddrs(ifaddr);
            return tmp;
        }
    }
    freeifaddrs(ifaddr);
}

int VWSRobot::sendData(std::vector<RobotTask>taskData) {
    int packetHeader = 22223333;
    static int packetNum = 0;
    ++packetNum;
    int orderNum = 0;
    for(auto iter = taskData.begin();iter != taskData.end();iter++){
        if(iter->task == TaskType::IO){
            orderNum++;
        }else{
            orderNum += iter->track.size();
        }
    }
    int datalen = 4 + orderNum *10;
    float *data = new float[datalen];
    int packetEnd = 33332222;

    data[0] = *reinterpret_cast<float *>(&packetHeader);
    data[1] = *reinterpret_cast<float *>(&packetNum);
    data[2] = *reinterpret_cast<float *>(&orderNum);
    data[datalen -1] = *reinterpret_cast<float *>(&packetEnd);

    int off = 3;
    for(auto iter = taskData.begin();iter != taskData.end();iter++){
        data[off] = *reinterpret_cast<float *>(&(iter->task));
        if(iter->task == TaskType::IO){
            data[off + 1] = iter->Singal;
            data[off + 2] = iter->IO;
            data[off + 3] = 0;
            data[off + 4] = 0;
            data[off + 5] = 0;
            data[off + 6] = 0;
            data[off + 7] = 0;
            data[off + 8] = 0;
            data[off + 9] = 0;
            off += 10;
        }else{
            for(auto jter = iter->track.begin();jter != iter->track.end();jter++){
                data[off + 0] = *reinterpret_cast<float *>(&(iter->task));
                data[off + 1] = iter->speed[0];
                data[off + 2] = iter->speed[1];
                data[off + 3] = (*jter)[0];
                data[off + 4] = (*jter)[1];
                data[off + 5] = (*jter)[2];
                data[off + 6] = (*jter)[3];
                data[off + 7] = (*jter)[4];
                data[off + 8] = (*jter)[5];
                data[off + 9] = (*jter)[6];
                off += 10;
            }
        }

    }

    int nsend = 0;
    //Poco::Net::ServerSocket *serverSocket = new Poco::Net::ServerSocket(9999);
    //auto streamSocket = serverSocket->acceptConnection();
    nsend = streamSocket.sendBytes(data, datalen*4);
//    char e;
//    std::cin>>e;
    //streamSocket.close();
    //delete serverSocket;
    delete data; //TODO: 删除报错
    return nsend == datalen?1:0;
}

int VWSRobot::Init(std::string ip,int port)
{
    std::cout<<"机器人Init"<<std::endl;

    abbObj_ = new abb::rws::RWSStateMachineInterface(ip, port);
    if (abbObj_->collectRuntimeInfo().rws_connected) {
        while (!abbObj_->stopRAPIDExecution()) {
            //std::cout<<"ssff    "<<std::endl;
            usleep(100000);
        }
        while (!abbObj_->resetRAPIDProgramPointer()) {
            //std::cout<<"ff    "<<std::endl;
            sleep(1);
        }
        std::cout<<"setip   "<<selfIp_<<"  "<<abbObj_->setRAPIDSymbolData("Socket", "RD", "ip", "\"" + selfIp_ + "\"")<<std::endl;
        while (1) {
            auto ret = abbObj_->startRAPIDExecution();
            if(ret==1){
                break;
            }
            else{
                abbObj_->setMotorsOff();
                abbObj_->setMotorsOn();
            }
            usleep(100000);
        }
        streamSocket = serverSocket->acceptConnection();
        return 1;
    } else {
        delete abbObj_;
        abbObj_ = nullptr;
        // TODO::连接失败，请检查IP等
        return -1;
    }
}
int VWSRobot::start() {
    std::cout<<"机器人Start"<<std::endl;
    if (!abbObj_)
        return -1;
    for (int i = 0; i < 3; i++) {
        if (abbObj_->setMotorsOn())
            return 1;
    }
    return -1;
}
int VWSRobot::close(){
    if(!abbObj_) return -1;
    return abbObj_->setMotorsOff()? 1:-1;
}
int VWSRobot::state(){
    if(!abbObj_) return -1;
    return abbObj_->collectRuntimeInfo().rws_connected ? 1:0;
}

int VWSRobot::getRobotPosition(VWSRobot::RobotPosition & data){
    if(!abbObj_) return -1;
    auto robNameVec =  abbObj_->getCFGMechanicalUnits();
    if(robNameVec.empty()) return -1;
    std::string robName;
    for(auto iter = robNameVec.begin();iter != robNameVec.end();iter++){
        robName = iter->name;
        break;
    }
    abb::rws::JointTarget jointData;
    abb::rws::RobTarget robData;
    if(!abbObj_->getMechanicalUnitJointTarget(robName,&jointData)) return -1;
    if(!abbObj_->getMechanicalUnitRobTarget(robName,&robData)) return -1;
    data.RobJoint[0] = jointData.robax.rax_1.value;
    data.RobJoint[1] = jointData.robax.rax_2.value;
    data.RobJoint[2] = jointData.robax.rax_3.value;
    data.RobJoint[3] = jointData.robax.rax_4.value;
    data.RobJoint[4] = jointData.robax.rax_5.value;
    data.RobJoint[5] = jointData.robax.rax_6.value;
    data.pos[0] = robData.pos.x.value;
    data.pos[1] = robData.pos.y.value;
    data.pos[2] = robData.pos.z.value;
    data.orient[0] = robData.orient.q1.value;
    data.orient[1] = robData.orient.q2.value;
    data.orient[2] = robData.orient.q3.value;
    data.orient[3] = robData.orient.q4.value;
    return 1;
}
