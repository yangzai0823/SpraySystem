#include "VWSCamera.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"
int _kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	/* set the nonblock */
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0));
	if (ch != EOF)
	{
		ungetc(ch, stdin); /* back the ch to stdin */
		return 1;
	}
	return 0;
}

int saveToFile(std::string fileName,const VWSCamera::ImageData &data){
    std::string ply = fileName+".ply";
    std::ofstream outFile;
    //打开文件
    outFile.open(ply);
    int nPointNum = data.PointCloudImage.nDataLen / (sizeof(float) * 3);
    float* pSrcValue = (float*)(data.PointCloudImage.pData);
    outFile<<"ply"<<std::endl;
    outFile << "format ascii 1.0" << std::endl;
    outFile << "comment author:hik-robot" << std::endl;
    outFile << "element vertex "<< nPointNum << std::endl;
    outFile << "property float x" << std::endl;
    outFile << "property float y" << std::endl;
    outFile << "property float z" << std::endl;
    outFile << "end_header" << std::endl;
    for (int nPntIndex = 0; nPntIndex < nPointNum; ++nPntIndex) {
        outFile<<pSrcValue[nPntIndex * 3 + 0]<<" "<<pSrcValue[nPntIndex * 3 + 1]<<" "<<pSrcValue[nPntIndex * 3 + 2]<<std::endl;
    }
    outFile.close();
    cv::Mat iMat(data.RGB8PlanarImage.nHeight, data.RGB8PlanarImage.nWidth, CV_8UC3);
    int len = data.RGB8PlanarImage.nDataLen;
    for (int i = 0; i < len / 3; i++) {
        iMat.data[i * 3 + 0] = data.RGB8PlanarImage.pData[i * 3 + 2];
        iMat.data[i * 3 + 1] = data.RGB8PlanarImage.pData[i * 3 + 1];
        iMat.data[i * 3 + 2] = data.RGB8PlanarImage.pData[i * 3 + 0];
    }
    cv::imwrite(fileName+".bmp", iMat);
}


void funcss(const VWSCamera::ImageData &data, void* pUser){
    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
    static int i = 0;
    std::cout<<++i<<"  "<<data.RGB8PlanarImage.nTimeStamp<<"      "<<data.RGB8PlanarImage.nTimeStamp - q<<std::endl;
    q = data.RGB8PlanarImage.nTimeStamp;
    saveToFile("/home/joker/testData/"+std::to_string(i),data);
    ((VWSCamera *)pUser)->deleteImage(data);
}

int main(){
    // VWSCamera c;
    // std::cout<<"Init "<<c.Init("192.168.125.99",1)<<std::endl;
    // std::cout<<"connect "<<c.connect()<<std::endl;
    // c.RegisterFrameCallBack(funcss,(void *)(&c));
    VWSCamera c11;
    float time;
    std::cout<<"Init "<<c11.Init("192.168.125.98",1)<<std::endl;
    std::cout<<"connect "<<c11.connect()<<std::endl;
    c11.changeTriggerMode(false);
    c11.startGrab();
    VWSCamera::ImageData data;
    std::cout<<c11.FetchFrame(5000,data)<<std::endl;
    saveToFile("/home/joker/testData/"+std::to_string(1),data);
    c11.deleteImage(data);
    return 1;
    // std::cout<<"save "<<c11.saveAllParam("1111111111")<<std::endl;
    // std::cout<<"load "<<c11.loadAllParam("1111111111")<<std::endl;
    c11.RegisterFrameCallBack(funcss,(void *)(&c11));
    c11.changeTriggerSource(false);
    // c11.setImageMode(VWSCamera::ImageMode::ColorImage);
    //c11.setExposure(40044.00);
    // c11.getExposure(time);
    // std::cout<<"color曝光1  "<<time<<std::endl;
    // c11.setImageMode(VWSCamera::ImageMode::DepthImage);
    // c11.setExposure(40000.00);
    std::cout<<"参数  "<<c11.coordChanged()<<std::endl;
    c11.startGrab();
    
    
 
    // c11.getExposure(time);
    // std::cout<<"dep曝光1  "<<time<<std::endl;
    // std::cout<<"取图片"<<std::endl;
    char w;
    int is = 0;
    while(1)
    {
        scanf("%c",&w);
        if (w == 'q') {
            std::cout << ++is << std::endl;
            c11.softTrigger();
        }

        sleep(1);
    }
    
    // sleep(2);

    // c11.stop();
    // c11.setImageMode(VWSCamera::ImageMode::ColorImage);
    // c11.setExposure(5000.00);
    // c11.getExposure(time);
    // std::cout<<"color曝光2  "<<time<<std::endl;
    // c11.setImageMode(VWSCamera::ImageMode::DepthImage);
    // c11.setExposure(100.00);
    // // c11.setExposure(1000.00);
    // c11.getExposure(time);
    // std::cout<<"dep曝光2  "<<time<<std::endl;
    // c11.start();
    // sleep(2);
    // std::cout<<"取图片"<<std::endl;
    // c11.softTrigger();
    

    
    //c.RegisterFrameCallBack();
    // VWSCamera::ImageData data;
    // VWSCamera::ImageData data2;
   //std::cout<<"getImagesss "<<c.softTrigger()<<std::endl;
    // cv::Mat iMat2(data.RGB8PlanarImage.nHeight, data.RGB8PlanarImage.nWidth, CV_8UC3, data.RGB8PlanarImage.pData);
    // cv::imwrite("/home/joker/ProjectTest/HKCamera/testData/rgb8te2.bmp", iMat2);
    // std::cout<<"getImage "<<c.getImage(data2)<<std::endl;
    // cv::Mat iMat(data2.RGB8PlanarImage.nHeight, data2.RGB8PlanarImage.nWidth, CV_8UC3, data2.RGB8PlanarImage.pData);
    // cv::imwrite("/home/joker/ProjectTest/HKCamera/testData/rgb8te3.bmp", iMat);
    // std::cout<<"c.state()  "<<c.state()<<std::endl;
    // VWSCamera::ImageData data;
    // for (int i = 0; i < 5; ++i) {
    //    std::cout << "getImage " << c.getImage(data) << std::endl;
    //    c.saveToFile("/home/joker/ProjectTest/HKCamera/testData/" + std::to_string(0), data);
    // }
    //sleep(5);
    //std::cout<<"getImagesss "<<c.softTrigger()<<std::endl;
    BOOL bExit_Main = FALSE;
    while (!bExit_Main)
    {
        //按任意键退出
        if (_kbhit())
        {
            bExit_Main = TRUE;
        }
    }
    
    // std::cout<<"c.state()  "<<c.state()<<std::endl;
    // std::cout<<"disConnect "<<c.disConnect()<<std::endl;
    // std::cout<<"c.state()  "<<c.state()<<std::endl;
    
    
    return 0;
}