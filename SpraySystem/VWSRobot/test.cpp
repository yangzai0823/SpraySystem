#include "VWSRobot.h"
int main() {
    VWSRobot l;
    std::cout<<l.Init("192.168.0.20")<<std::endl;
    VWSRobot::RobotPosition p;
    l.getRobotPosition(p);
    printf("%f %f %f %f %f %f \n",p.RobJoint[0],p.RobJoint[1],p.RobJoint[2],p.RobJoint[3],p.RobJoint[4],p.RobJoint[5]);
    printf("%f %f %f\n",p.pos[0],p.pos[1],p.pos[2]);
    printf("%f %f %f %f\n",p.orient[0],p.orient[1],p.orient[2],p.orient[3]);
    std::vector<VWSRobot::RobotTask> dVec;
    VWSRobot::RobotTask d;
    d.task = VWSRobot::TaskType::IO;
    d.IO = 0;
    d.Singal = 0;
    //dVec.push_back(d);
    auto d1 = d;
    d1.Singal = 1;
    VWSRobot::RobotTask d2;
    d2.task = VWSRobot::TaskType::MOVEL;
    d2.speed[0] = 3;
    d2.speed[1] = 4;
    std::array<float,7> tr = {0,1,2,3,4,5,6};
    for(int i = 0;i<10;i++){
        tr[0] += 1;
        d2.track.push_back(tr);
    }
    dVec.push_back(d);
    dVec.push_back(d1);
    dVec.push_back(d2);
    l.sendData(dVec);
    return 0;
}
