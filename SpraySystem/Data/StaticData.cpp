
#include <QString>
#include <Eigen/Eigen>
#include "StaticData.h"
namespace vws {
    QString Camera_top ="相机上";
    QString Camera_bottom = "相机下";
    QString Robot1= "机器人1";
    QString Robot2 = "机器人2";
    QString PLC ="PLC";
    QString MC1 = "MC";
    QString MC2 = "MC2";

    /**
     * @brief 箱子理论长度，单位mm
     */
    int64_t BoxLenght = 500;

    /**
     * @brief 箱子理论高度, 单位mm
     */
    int64_t BoxHeight = 800;

    int64_t BoxMaxLength = 1000;
    int64_t BoxMinHeight = 100;
    int64_t BoxMaxWidth = 1000;
    int64_t BoxMinWidth = 100;
    int64_t BoxMaxHeight = 800;

    float rangeMin = 0;
    float rangeMax = 2000;

    /**
     * @brief PLC出发信号超时时间，单位毫秒
     */
    int64_t BottomSingalTimeOut = 30 * 1000;
    /**
     * @brief PLC出发信号超时时间，单位毫秒
     */
    int64_t TopSingalTimeOut = 30 * 1000;

    int32_t LaserUp = 2047;
    int32_t LaserBottom = 0;


    /// @brief 喷涂偏差值
    uint64_t diff = 0; 

    //机器人空闲位置

    /*** @brief 头部运动最远距离 mm*/
    int64_t HeadMoveMaxLength = 1500;

    double rbtspeed = 50; //20 * 20 * 3;

    /** @brief 编码器数值与距离转换关系  encoder/mm*/
    float chainFactor = 0.4198727819755431f;
    /****************标定参数********************/

    /*** @brief 下相机+机器人1手眼转换 */
      std::array<double,12>  handEyeMatrix_b_rbt1;
    /*** @brief 上相机+机器人1手眼转换 */
      std::array<double,12>  handEyeMatrix_u_rbt1;

    /*** @brief 上相机+机器人2手眼转换 */
      std::array<double,12>  handEyeMatrix_b_rbt2;
    /*** @brief 下相机+机器人2手眼转换 */
      std::array<double,12>  handEyeMatrix_u_rbt2;

    /** @brief 传感器相对距离*/
    double senorDistance_b  = 2929;  //mm

    /** @brief 工装孔距*/
    double holeDistance = 100.0; //mm

    /** @brief 编码器旋转矩阵*/
    std::array<double,9> senorRotationMatrix_b;

    std::array<double,3> robotBeltDirection;
    /************************************/

    void DataInit::Init(){
        vws::handEyeMatrix_b_rbt1 = std::array<double,12>({
             0.047927375882864,
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
            -532.5537719726563
        });

            vws::handEyeMatrix_u_rbt1 = std::array<double,12>({
              0.00505609391,
              -0.0653268024,
              0.997851133,
              534.693359,
              0.00418723887,
              0.997856498,
              0.0653059781,
              2759.16211,
              -0.999978483,
              0.00384803256,
              0.00531880092,
              583.703369
            });

        vws::senorRotationMatrix_b = std::array<double,9>({
             -0.059050992131233218,
            0.9958091974258424,
            0.06983548402786255,
            -0.996536672115326,
            -0.05470200255513191,
            -0.06262891739606857,
            -0.05854630842804909,
            -0.07329192757606507,
            0.9955905675888062
        });
      

      vws::robotBeltDirection = std::array<double,3>({
        0.0007763591129332781,
        0.4166736602783203,
        -0.0002799863286782056
      });
    }

    bool DataVerify::RangingVerify(float num1,float num2){
        return (num1>rangeMin && num1<rangeMax) && (num2>rangeMin && num2<rangeMax);
    }

    bool DataVerify::BoxHeightVerify(float height){
      return height<BoxMaxHeight && height>BoxMinHeight;
    }
    bool DataVerify::BoxWidthVerify(float width){
      return width<BoxMaxWidth && width>BoxMinWidth;
    }
}

