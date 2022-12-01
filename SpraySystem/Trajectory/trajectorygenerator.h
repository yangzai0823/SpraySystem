#ifndef TRAJECTORYGENERATOR_H
#define TRAJECTORYGENERATOR_H
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <iostream>

#include "planner/ompl_planning_class.h"
#include "planner/planning_utils.h"
#include "planner/utils.h"

#include "VWSRobot/VWSRobot.h"
#include "Data/StructData.h"

using TrajParam = vws::TrajParam;
class TrajectoryGenerator
{
public:

    static TrajectoryGenerator* Instance();
    ~TrajectoryGenerator();


    /**
     * @brief 箱体环境信息
     */
    void AddBoxEnvirInfo(Eigen::Vector3d boxcenter,Eigen::Vector3d boxsize,Eigen::Quaterniond boxq, const std::string &name);

    void AddBoxHookEnvirInfo(Eigen::Vector3d boxcenter, Eigen::Vector3d boxsize,
                             Eigen::Quaterniond boxq, double hook_height,
                             const std::string &name);
    /**
     * @brief
     *
     * @param start         起始关节角
     * @param end           结束关节角
     * @param nsteps        规划步数
     * @param entry_traj    返回的轨迹
     * @param ndof          关节数
     * @param plan_total_num    内部规划尝试次数
     * @return true             结果有效
     * @return false            结果无效
     */
    bool GenerateEntryTrajectory(
        Eigen::VectorXd start, Eigen::VectorXd end, int nsteps,
        Eigen::VectorXd &entry_traj, int ndof = 6, int plan_total_num = 3, bool vis = false);

    /**
     * @brief 
     * 
     * @param init_dof          起始关节角
     * @param p                 喷涂位置约束，单位：mm
     * @param ori               喷涂姿态约束：四元数
     * @param traj              返回的轨迹
     * @param ndof              关节数
     * @return true             结果有效
     * @return false            结果无效
     */
    bool GeneratePaintTrajectory(Eigen::VectorXd init_dof,
                                                      Eigen::VectorXd p,
                                                      Eigen::VectorXd ori,
                                                      Eigen::VectorXd &traj,
                                                      int ndof = 6);
    /**
     * @brief 利用箱体信息，生成喷涂约束
     * 
     * @param boxCenterPoint            箱子中心点，单位：mm
     * @param boxSize                   箱子尺寸：单位：mm
     * @param boxq                      箱子姿态：四元数
     * @param painter_ori               喷涂姿态：四元数，箱子坐标系下的喷涂姿态，箱子坐标系同机器人基坐标系
     * @param front                     是否喷涂箱子前方，true：前方，false：后方
     * @param invert                    箱子行进方向是否与机器人基坐标系的y正方向相反，true：相反，false：相同
     * @param p                         输出的位置约束：单位：mm
     * @param ori                       输出的姿态约束：四元数
     */
    void GeneratePaintConstraint(
        Eigen::Vector3d boxCenterPoint, Eigen::Vector3d boxSize,
        Eigen::Quaterniond boxq, Eigen::Quaterniond painter_ori, bool front,
        bool invert, Eigen::VectorXd &p, Eigen::VectorXd &ori);

    /**
     * @brief 利用箱体信息，生成喷涂约束
     * 
     * @param boxCenterPoint            箱子中心点，单位：mm
     * @param boxSize                   箱子尺寸：单位：mm
     * @param boxq                      箱子姿态：四元数
     * @param painter_ori               喷涂姿态：四元数，箱子坐标系下的喷涂姿态，箱子坐标系同机器人基坐标系
     * @param front                     是否喷涂箱子前方，true：前方，false：后方
     * @param invert                    箱子行进方向是否与机器人基坐标系的y正方向相反，true：相反，false：相同
     * @param shrink_size_x             x方向缩减的喷涂距离
     * @param shrink_size_z             z方向缩减的喷涂距离
     * @param paint_dir                 主喷涂方向： 0-X方向喷涂，1-Z方向喷涂
     * @param p                         输出的位置约束：单位：mm
     * @param ori                       输出的姿态约束：四元数
     */
    void GenerateShrinkedPaintConstraint(
        Eigen::Vector3d boxCenterPoint, Eigen::Vector3d boxSize,
        Eigen::Quaterniond boxq, Eigen::Quaterniond painter_ori,
        float shrink_size_x, float shrink_size_z, bool front, bool invert,
        int paint_dir,  // 0: x, 1: z
        Eigen::VectorXd &p, Eigen::VectorXd &ori);


/**
 * @brief 
 * 
 * @param boxCenterPoint 
 * @param boxSize 
 * @param boxq 
 * @param painter_ori               box坐标系下的喷涂姿态，box坐标系与模式1（悬挂链运行方向与机器人y正方向一致）下的机器人坐标系一致。
 * @param weld_y_offset 
 * @param shrink_z 
 * @param front 
 * @param invert 
 * @param p 
 * @param ori 
 */
    void GenerateSeamPaintConstraint(Eigen::Vector3d boxCenterPoint,
                                     Eigen::Vector3d boxSize,
                                     Eigen::Quaterniond boxq,
                                     Eigen::Quaterniond painter_ori,
                                     float weld_y_offset, float shrink_z,
                                     bool front, bool invert,
                                     Eigen::VectorXd &p, Eigen::VectorXd &ori);
    /**
     * @brief 生成机器人喷涂任务
     */
    VWSRobot::RobotTask GenerateSprayTask();

    /**
     * @brief 计算悬挂链的跟随零点,  跟随0点为p1走到机器人0点位置时的编码器值
     * 
     * @param p1                    箱体上的参考0点
     * @param follow_offset          跟随距离，以悬挂链运行方向为正方向，正表示机器人超前跟随
     * @param encoder               拍照时刻的悬挂链编码器值
     * @param is_increas                运动时悬挂链编码器是否是增加的
     * @param invert                表示机器人y正方向和悬挂链运行方向是否相反
     * @return std::vector<float> 
     */ 
    std::vector<float> calChainZeroPoint(Eigen::Vector3d p1,
            float follow_offset, int64_t encoder, bool is_increase, bool invert);
    /**
     * @brief 计算悬挂链0点
     */
    std::vector<float> calChainZeroPoint(TrajParam param, bool invert);

    float pathDist(const Eigen::VectorXd &traj, int ndof);

    void clearEnv();
    Eigen::Vector3d topfarpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize, bool invert);
    Eigen::Vector3d topnearpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize, bool invert);
    Eigen::Vector3d bottomnearpont(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize, bool invert);

   private:
    static std::mutex _mutex;
    
    static TrajectoryGenerator *_instance;
    VwsPlanEnv *pt;
    OpenRAVE::RobotBasePtr robot;
    Eigen::VectorXd planRet;
    /**
     * @brief 编码器数值和实际距离转换系数
     */
    float chainFactor =0.4198727819755431f; //25/600;
    Eigen::Vector3d robotBeltDirection;

    Eigen::Vector3d extraAxisDirection;

    std::vector<OpenRAVE::KinBodyPtr> boxes_;

   private:
    TrajectoryGenerator();
    /**
     * @brief 生成环境信息
     */
    void GenerateEnvirInfo();
};

#endif // TRAJECTORYGENERATOR_H
