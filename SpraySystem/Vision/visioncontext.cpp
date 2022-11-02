#include "visioncontext.h"

VisionContext::VisionContext()
{

}

void imageConvert(MV3D_RGBD_IMAGE_DATA points)
{
//    // object3d
//      int width = points.nWidth;
//      int height = points.nHeight;
//      int count = width * height;
//      std::vector<float> px(count), py(count), pz(count);
//      for (int i = 0; i < height; i++) {
//        for (int j = 0; j < width; j++) {
//          px[i * width + j] = points.pData(j, i).x;
//          py[i * width + j] = points(j, i).y;
//          pz[i * width + j] = points(j, i).z;
//        }
//      }

//      HImage imgx("real", width, height, (void*)px.data());
//      HImage imgy("real", width, height, (void*)py.data());
//      HImage imgz("real", width, height, (void*)pz.data());
}

void VisionContext::getPoseAndHeight(ImageData image, VisionContext::VisionData &visionData)
{


    //销毁图像

}
