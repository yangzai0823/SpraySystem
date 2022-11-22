#ifndef STATICDATA_H
#define STATICDATA_H
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace vws {
        extern QString Camera_top;
        extern QString Camera_bottom;
        extern QString Robot1;
        extern QString Robot2;

        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        extern int64_t BottomSingalTimeOut;
        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        extern int64_t TopSingalTimeOut;
        /**
         * @brief 箱子理论长度，单位mm
         */
        extern int64_t BoxLenght;

        /**
         * @brief 箱子理论高度, 单位mm
         */
       extern int64_t BoxHeight;

        extern int32_t LaserUp;
        extern int32_t LaserBottom;


        /// @brief 喷涂偏差值
        extern uint64_t diff; 

        //机器人空闲位置

        /*** @brief 头部运动最远距离 mm*/
        extern int64_t HeadMoveMaxLength;

        extern double rbtspeed;

        /** @brief 编码器数值与距离转换关系  encoder/mm*/
        extern float chainFactor;
        /****************标定参数********************/

        /*** @brief 下相机+机器人1手眼转换 */
        extern std::array<double,12>  handEyeMatrix_b_rbt1;
        /*** @brief 上相机+机器人1手眼转换 */
        extern std::array<double,12>  handEyeMatrix_u_rbt1;

        /*** @brief 上相机+机器人2手眼转换 */
        extern std::array<double,12>  handEyeMatrix_b_rbt2;
        /*** @brief 下相机+机器人2手眼转换 */
        extern std::array<double,12>  handEyeMatrix_u_rbt2;

        /** @brief 传感器相对距离*/
        extern double senorDistance_b;  //mm

        /** @brief 工装孔距*/
        extern double holeDistance; //mm

        /** @brief 编码器旋转矩阵*/
        extern std::array<double,9> senorRotationMatrix_b;
        /************************************/

     class DataInit{
     public:
         static void Init();
     };

}
#endif // STATICDATA_H
