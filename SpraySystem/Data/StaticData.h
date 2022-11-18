#ifndef STATICDATA_H
#define STATICDATA_H
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace vws {
        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        static int64_t BottomSingalTimeOut = 60 * 1000;
        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        static int64_t TopSingalTimeOut = 60 * 1000;
        /**
         * @brief 箱子理论长度，单位mm
         */
        static int64_t BoxLenght = 500;

        /**
         * @brief 箱子理论高度, 单位mm
         */
       static int64_t BoxHeight = 800;
    
        /*** @brief 上相机+机器人1手眼转换 */
        static Eigen::Isometry3d handEyeMatrix_b_rbt1;
        /*** @brief 下相机+机器人1手眼转换 */
        static Eigen::Isometry3d handEyeMatrix_u_rbt1;

        /*** @brief 上相机+机器人2手眼转换 */
        static Eigen::Isometry3d handEyeMatrix_b_rbt2;
        /*** @brief 下相机+机器人2手眼转换 */
        static Eigen::Isometry3d handEyeMatrix_u_rbt2;

        static int32_t LaserUp = 2047;
        static int32_t LaserBottom = 0;


        /// @brief 喷涂偏差值
        static uint64_t diff = 0; 

        //机器人空闲位置

        /*** @brief 头部运动最远距离 mm*/
        static int64_t HeadMoveMaxLength = 1000;

     class DataInit{
     public:
         static void Init(){
             vws::handEyeMatrix_b_rbt1.affine().block(0,0,3,4)
                     <<  0.047927375882864,
                        0.029266173020005226,
                        0.9984219670295715,
                        469.2415771484375,
                        0.005496096797287464,
                        0.9995478391647339,
                        -0.029563141986727715,
                        2466.051025390625,
                        -0.9988358020782471,
                        0.006904320791363716,
                        0.04774487391114235,
                        -532.5537719726563;
         
                 vws::handEyeMatrix_u_rbt1.affine().block(0,0,3,4)
                     <<  -0.0150250699,
                        -0.118890852,
                        0.992793739,
                        519.98938,
                        0.0130119231,
                        0.992798448,
                        0.11908827,
                        2632.92798,
                        -0.999802351,
                        0.0147074759,
                        -0.0133698536,
                        668.73468;
     }
     };

}
#endif // STATICDATA_H
