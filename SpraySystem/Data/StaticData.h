#ifndef STATICDATA_H
#define STATICDATA_H
#include <iostream>

namespace vws {
        /**
         * @brief PLC出发信号超时时间，单位毫秒
         */
        int64_t SingalTimeOut = 60 * 1000;

        /**
         * @brief 箱子理论长度
         */
        int64_t BoxLenght = 1.2;

        /**
         * @brief 箱子理论高度
         */
        int64_t BoxHeight = 0.8;

}
#endif // STATICDATA_H
