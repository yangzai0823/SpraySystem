#ifndef STATICDATA_H
#define STATICDATA_H
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace vws {
        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        int64_t BottomSingalTimeOut = 60 * 1000;
        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        int64_t TopSingalTimeOut = 60 * 1000;
        /**
         * @brief 箱子理论长度
         */
        int64_t BoxLenght = 1.2;

        /**
         * @brief 箱子理论高度
         */
        int64_t BoxHeight = 0.8;

        Eigen::Isometry3d handEyeMatrix;

        int32_t LaserUp = 2047;
        int32_t LaserBottom = 0;
     class DataInit{
     public:
         static void Init(){
             vws::handEyeMatrix.affine().block(0,0,3,4)
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
         }
     };

}
#endif // STATICDATA_H
