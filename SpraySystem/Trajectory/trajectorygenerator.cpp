#include "trajectorygenerator.h"

#include <openrave-core.h>
#include <openrave/openrave.h>

#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <ctime>
#include <list>

#include "osgviewer/osgviewer.hpp"
#include "sco/optimizers.hpp"
#include "trajopt/collision_checker.hpp"
#include "trajopt/collision_terms.hpp"
#include "trajopt/common.hpp"
#include "trajopt/problem_description.hpp"
#include "trajopt/rave_utils.hpp"
#include "utils/clock.hpp"
#include "utils/config.hpp"
#include "utils/eigen_conversions.hpp"
#include "utils/stl_to_string.hpp"
// ompl includes
#include <ompl/base/ConstrainedSpaceInformation.h>
#include <ompl/base/Constraint.h>
#include <ompl/base/DiscreteMotionValidator.h>
#include <ompl/base/OptimizationObjective.h>
#include <ompl/base/objectives/PathLengthOptimizationObjective.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/base/spaces/constraint/AtlasStateSpace.h>
#include <ompl/base/spaces/constraint/ConstrainedStateSpace.h>
#include <ompl/base/spaces/constraint/ProjectedStateSpace.h>
#include <ompl/base/spaces/constraint/TangentBundleStateSpace.h>
#include <ompl/geometric/PathGeometric.h>
#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/est/BiEST.h>
#include <ompl/geometric/planners/est/EST.h>
#include <ompl/geometric/planners/est/ProjEST.h>
#include <ompl/geometric/planners/informedtrees/BITstar.h>
#include <ompl/geometric/planners/kpiece/BKPIECE1.h>
#include <ompl/geometric/planners/kpiece/KPIECE1.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/prm/SPARS.h>
#include <ompl/geometric/planners/prm/SPARStwo.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>

#include <IK/utils.hpp>
#include <deque>

#include "planner/ompl_planning_class.h"

//#include "planner/utils.h"

namespace po = boost::program_options;
namespace ob = ompl::base;
namespace og = ompl::geometric;
namespace om = ompl::magic;
using namespace trajopt;
using namespace std;
using namespace OpenRAVE;
using namespace util;
using namespace boost::assign;


TrajectoryGenerator::TrajectoryGenerator()
{
    robotBeltDirection = Eigen::Vector3d( -0.00321569, -0.419444, -0.001071);
    handEyeMatrix.affine().block(0,0,3,4)
          <<  -0.03743671625852585,
            -0.001139160362072289,
            -0.9992983341217041,
            2654.87060546875,
            0.009748989716172219,
            -0.9999521374702454,
            0.0007746600895188749,
            -1332.0638427734376,
            -0.9992514252662659,
            -0.009713122621178627,
            0.03744605928659439,
            481.78564453125;
    extraAxisDirection = Eigen::Vector3d( 0, -0.0002153165405616164,0);
}

TrajectoryGenerator::~TrajectoryGenerator()
{
//    delete pt;
}

void TrajectoryGenerator::GenerateEnvirInfo()
{
    //*******************************************************
    // ???????????????
    // URDFTest pt;
      pt = new VwsPlanEnv(
        "/home/vws/Demo/QT/Spray/build-SpraySystem-Desktop_Qt_5_12_8_GCC_64bit-Debug/urdf/abb_irb1410_urdf/urdf/"
        "abb_irb1410_urdf.urdf",
        "/home/vws/Demo/QT/Spray/build-SpraySystem-Desktop_Qt_5_12_8_GCC_64bit-Debug/urdf/abb_irb1410_urdf/urdf/"
        "abb_irb1410_urdf.srdf");
    auto manip_rot =   Eigen::AngleAxisd(M_PI, Vector3d::UnitZ()) *
                               Eigen::AngleAxisd(-M_PI / 2, Vector3d::UnitY()) *
                               Eigen::AngleAxisd(M_PI / 2.0, Vector3d::UnitX());
    robot = GetRobot(*pt->env);
    //?????????????????????
    pt->addManipulator(robot, "tool", "base", "link_flange", manip_rot,
                       Eigen::Vector3d(0.00, 0.2, 0.285));
}

void TrajectoryGenerator::BoxEnvirInfo(Eigen::Vector3d boxcenter, Eigen::Vector3d boxsize, Eigen::Quaterniond boxq)
{
    double box1centerx,box1centery,box1centerz;
     double box1height,box1width,box1length;
     double box1q1,box1q2,box1q3,box1q4;
     box1centerx =boxcenter[0];
     box1centery = boxcenter[1];
     box1centerz = boxcenter[2];
     box1length = boxsize[0]/2;
     box1width = boxsize[1]/2;
     box1height = boxsize[2]/2;


     // ?????????????????????
     pt->addBox(boxcenter, boxsize/2.0, boxq, "box1");
     //      pt.addBox(Eigen::Vector3d(box2centerx, box2centery, box2centerz), Eigen::Vector3d(box2length, box2width, box2height), Eigen::Quaterniond(box2q1,box2q2,box2q3,box2q4), "box2");
     // ????????????????????????
     pt->addBox(Eigen::Vector3d(box1centerx, box1centery+0.4, box1centerz+1.2), Eigen::Vector3d(box1length, box1width, box1height), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box2");
     // ????????????????????????
     pt->addBox(Eigen::Vector3d(box1centerx, box1centery+1.8, box1centerz), Eigen::Vector3d(box1length, box1width, box1height), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box3");
     // ??????????????????????????????
     pt->addBox(Eigen::Vector3d(box1centerx, box1centery+1.8, box1centerz), Eigen::Vector3d(0.05, 0.2, box1height+2), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "hook");
     // ????????????
     pt->addBox(Eigen::Vector3d(-1, 0, 1), Eigen::Vector3d(0.01, 5, 2), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "wall");

}

void TrajectoryGenerator::GenerateTrajectory(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize,Eigen::Quaterniond boxq)
{
    //???????????????????????????
    Eigen::Matrix3d rot;
    // rot ?????????????????????????????????????????????
    rot << 1, 0, 0, //
            0, 0, -1, //
            0, 1, 0;
    Eigen::Quaterniond quat(rot);
    quat = boxq * quat;
    Eigen::Vector3d p1, p2, p3, dn(0, -1, 0);
    Eigen::Vector4d ori_dn(quat.w(), quat.x(), quat.y(), quat.z());
    rot = (Eigen::AngleAxisd(-M_PI / 4.0, Eigen::Vector3d::UnitZ()) * Eigen::AngleAxisd(M_PI, Eigen::Vector3d::UnitX()) * rot);
    // rot = (Eigen::AngleAxisd(3 * M_PI / 4.0, Eigen::Vector3d::UnitZ()) * rot);
    quat = Eigen::Quaterniond(rot);
    Eigen::Vector4d dn_weld(quat.w(), quat.x(), quat.y(), quat.z());

    // ????????????????????????????????? p1,p2,p3 ???????????????????????????ori_dn??????????????????
    // ???????????????????????????
    // ???????????????????????????????????????
    //      Eigen::Vector3d c(1.0, -0.7, 0.6);
    //      double dx = 0.4, dy = 0.6, dz = 0.4;
    //      p1 = c + Eigen::Vector3d(dx, dy, dz);
    //      p2 = c + Eigen::Vector3d(-dx, dy, dz);
    //      p3 = c + Eigen::Vector3d(-dx, dy, -dz);

    p1 = topfarpoint(boxCenterPoint,boxSize);
    p2 = topnearpoint(boxCenterPoint,boxSize);
    p3 = bottomnearpont(boxCenterPoint, boxSize);


    Eigen::VectorXd p, dir, ori;
    // genInitPathForRect(p1, p2, p3, dn, p, dir);
    // genInitOrientedPathForRect(p1, p2, p3, ori_dn, p, ori);
    genInitOrientedPathForBoxPlane(p1, p2, p3, ori_dn ,
                                   0,  // 0: x, 1: y
                                   0.2, 0.15, 0.1,
                                   0.1, 0, p, ori);


    // Eigen::Isometry3d painter_tf = Eigen::Isometry3d::Identity();
    // Eigen::Quaterniond tcp_2_painter(Eigen::AngleAxisd(3 / 4.0 * M_PI, Eigen::Vector3d::UnitY()));
    // painter_tf.prerotate(tcp_2_painter);
    // painter_tf.translate(Eigen::Vector3d(0, 0, 0.2));
    // genInitOrientedPathForLine(p2, p3, dn_weld, painter_tf, 0.2, 0.2, p, ori);

    int nsteps = p.size() / 3;
    Eigen::VectorXd init_pos(nsteps * 6);
    init_pos = Eigen::VectorXd::Zero(nsteps * 6);
    for (int i = 0; i < nsteps; i++) {
        init_pos[6 * i + 0] = 1.5;
        init_pos[6 * i + 1] = 0;
        init_pos[6 * i + 2] = 0;
        init_pos[6 * i + 3] = 0;
        init_pos[6 * i + 4] = 0;
        init_pos[6 * i + 5] = 0;
    }



    // pt.viewer->Idle();
    std::cout << "stage 1" << std::endl;
    ProblemConstructionInfo pci(pt->env);


    int collision_cnt, cnst_voil_cnt;
    double elapsed_time = 0;
    bool valid = false;


    //************************************************************************
    // step 1: ??????????????????
    // init_pos =
    //     planWeldPathFixed(pt.env, "toolB_6", ik, p, dir, 0, 0.1, init_pos,
    //                       collision_cnt, cnst_voil_cnt, elapsed_time, true);
    // init_pos =
    //     planWeldPathFixed(pt.env, "toolB_6", ik, p, dir, 100, 0.1, init_pos,
    //                       collision_cnt, cnst_voil_cnt, elapsed_time, true);
    init_pos =
            planOrientedPathFixed(pt->env, "tool", p, ori, 0, 0.1, init_pos,
                                  collision_cnt, cnst_voil_cnt, elapsed_time, false);
    init_pos =
            planOrientedPathFixed(pt->env, "tool", p, ori, 100, 0.1, init_pos,
                                  collision_cnt, cnst_voil_cnt, elapsed_time, false);
    std::cout << "voil_cnt " << cnst_voil_cnt << ", collision cnt "
              << collision_cnt << std::endl;

    // step 2???????????????????????????????????????????????????????????????
    // step 2.1?????????ompl??????????????????
    std::vector<double> j1, j2;
    for (int i = 0; i < 6; i++){
        j1.push_back(0);
        j2.push_back(init_pos[i]);
    }
    j1[0] = 1.5;

    if(robot==nullptr){
        std::cout<<"robot ??????"<<std::endl;
    }

    auto traj = planFreePathWithOMPL(robot, pt->env, "tool", j1, j2,1);
    int N = traj.size() * traj[0].size();
    //      while(traj.size() < nsteps){
    //        traj = planFreePathWithOMPL(robot, pt.env, "tool", j1, j2,1);
    //        N = traj.size() * traj[0].size();
    //      }
    Eigen::VectorXd init_pos_ompl;
    init_pos_ompl.resize(N);
    int current_n = 0;
    for (int i = 0; i < traj.size(); i++){
        for (int n = 0; n < traj[0].size(); n++){
            init_pos_ompl[current_n++] = traj[i][n];
        }
    }

    // step 2.2: ????????????????????????
    Eigen::VectorXd goal;
    goal.resize(6);
    goal[0] = init_pos[0];
    goal[1] = init_pos[1];
    goal[2] = init_pos[2];
    goal[3] = init_pos[3];
    goal[4] = init_pos[4];
    goal[5] = init_pos[5];
    if(nsteps <= init_pos_ompl.size() / 6){
        std::cout << "track" << std::endl;
    }
    planRet = planFreePathJoint(pt->env, "tool", goal, 20, 100, 0.2, init_pos_ompl, collision_cnt,
                                cnst_voil_cnt, elapsed_time, true);

}

VWSRobot::RobotTask TrajectoryGenerator::GenerateSprayTask()
{
    //    std::vector<VWSRobot::RobotTask> tasks;
    VWSRobot::RobotTask tk;
    tk.task = VWSRobot::TaskType::MOVEABSJ;  // 1:movel, 2:movej, 3:moveabsj
    tk.speed[0] = 500;
    tk.speed[1] = 10;
    int cnt = 0;
    std::cout << planRet.transpose() << std::endl;
    for(int i = 0; i < planRet.size() / 6; i++){
        std::cout << i << ": ";
        std::array<float,7> jv;
        for(int n = 0; n < 6; n++){
            jv[n] = planRet[cnt++] / M_PI * 180.0;
            // IRB 1410?????????3??????2?????????????????????????????????????????????
            if(n == 2){
                jv[n] += jv[n-1];
            }
            std::cout << jv[n] << ", ";
        }
        std::cout << std::endl;
        tk.track.push_back(jv);
    }
//    //        tasks.push_back(tk);
    return tk;
}

std::vector<float> TrajectoryGenerator::calChainZeroPoint(TrajParam param)
{

    chainFactor = 0.4198727819755431f;
//    std::cout<<"????????? "<<chainFactor<<std::endl;

    //1. ?????????????????????????????????????????????????????????
    //auto rbtBoxCenter = handEyeMatrix*param.boxCenterPoint;
    //2. ?????????????????????????????????????????????????????????P1?????????
    auto p1= bottomnearpont(param.boxCenterPoint, param.boxSize);
    //3. ?????????p1???????????????????????????p2????????? l2
    Eigen::Vector3d robotZeroPoint = extraAxisDirection*param.sevenEncoder;
    double l2 = robotZeroPoint[1] - p1[1];
    //4. ???????????????????????????????????????(diffL)???l2-??????????????? mm
    double diffL = l2 - param.offsetOfTraj;
    //5. diffL??????????????????????????? e1
    double e1 = diffL / chainFactor;
    //6. ?????????0??????????????????
    double ret = -e1+param.encoder;   //?????????????????????????????????????????????????????????

    std::vector<float> result;
    result.push_back(ret);
    result.push_back(param.offsetOfTraj);
//    result.push_back(0);
    return result;
}

Eigen::Vector3d TrajectoryGenerator::topfarpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize)
{
    double x= boxCenterPoint[0] + boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] + boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;
}

Eigen::Vector3d TrajectoryGenerator::topnearpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize)
{
    double x= boxCenterPoint[0] - boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] + boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;
}

Eigen::Vector3d TrajectoryGenerator::bottomnearpont(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize)
{
    double x= boxCenterPoint[0] - boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] - boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;
}
