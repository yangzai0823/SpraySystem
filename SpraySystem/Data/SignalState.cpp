 #include "SignalState.h"
 #include <iostream>

SignalState::State SignalState::NextState(SignalState::State current, SignalState::Trigger trigger)
  {
        SignalState::State ret = SignalState::State::IDLE;

      if(current == SignalState::State::IDLE && trigger == SignalState::Trigger::CameraSignalOn){
            std::cout<<"空闲+头部相机拍照=等待箱体感应"<<std::endl;
            ret = SignalState::State::WaitLaserSignalOn;
      }
      else if(current == SignalState::State::WaitLaserSignalOn && SignalState::Trigger::LaserSignalOnWithImg){
            std::cout<<"等待箱体感应+感应（有图像）= 头部图像处理"<<std::endl;
            ret = SignalState::State::ProcessHeadImg;
      }
      else if(current == SignalState::State::WaitCameraSignalOn && SignalState::Trigger::LaserSignalOnWithOutImg){
            std::cout<<"等待箱体感应+感应（无图像）= 等待头部图像"<<std::endl;
            ret = SignalState::State::WaitHeadImg;
      }
      else if(current == SignalState::State::WaitHeadImg && SignalState::Trigger::HeadImgReady){
            std::cout<<"等待头部图像+t头部图像 = 头部图像处理"<<std::endl;
            ret = SignalState::State::ProcessHeadImg;
      }
      else if(current == SignalState::State::ProcessHeadImg && SignalState::Trigger::HeadImgProcessDone){
            std::cout<<"头部图像处理+头部处理完成 = 头部处理完成"<<std::endl;
            ret = SignalState::State::HeadImgDone;
      }
      else if(current == SignalState::State::HeadImgDone && SignalState::Trigger::CameraSignalOffWithImg){
            std::cout<<"头部处理完成+尾部相机拍照（有图像） = 尾部处理"<<std::endl;
            ret = SignalState::State::ProcessTrailImg;
      }
       else if(current == SignalState::State::HeadImgDone && SignalState::Trigger::CameraSignalOffWithOutImg){
            std::cout<<"头部处理完成+尾部相机拍照（无图像） = 等待尾部图像"<<std::endl;
            ret = SignalState::State::WaitTrailImg;
      }
      else if(current == SignalState::State::WaitTrailImg && SignalState::Trigger::TrailImgReady){
            std::cout<<"等待尾部图像+尾部图像 = 尾部处理"<<std::endl;
             ret = SignalState::State::ProcessTrailImg;
      }
      else if(current == SignalState::SignalState::ProcessTrailImg && SignalState::Trigger::TrailImgProcessDone){
            std::cout<<"尾部处理+尾部完成 = 尾部处理完成"<<std::endl;

            ret = SignalState::State::IDLE;
      }
      else{
            std::cout<<"状态异常, Current State: "<<current<<", Trigger: "<<trigger<<std::endl;
            ret = current;
      }
      return ret;
  }