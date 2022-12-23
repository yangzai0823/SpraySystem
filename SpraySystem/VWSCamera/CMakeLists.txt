cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 14)
#project(test LANGUAGES CXX)
enable_language(C )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBS FormatConversion MediaProcess)


file(GLOB_RECURSE inc ${CMAKE_CURRENT_SOURCE_DIR}/Include/*)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/Lib)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Lib)

set(LIBS libFormatConversion.so        libMediaProcess.so
libGCBase_gcc421_v3_0.so      libMv3dRgbd.so
libGenApi_gcc421_v3_0.so      libMvCameraControl.so
libglfw.so                    libMVGigEVisionSDK.so
                  libMVRender.so
                libMvUsb3vTL.so
liblog4cpp_gcc421_v3_0.so     libNodeMapData_gcc421_v3_0.so
libLog_gcc421_v3_0.so         libXmlParser_gcc421_v3_0.so
libMathParser_gcc421_v3_0.so
)
#find_package(OpenCV REQUIRED opencv_highgui)
add_library(VWSCamera SHARED VWSCamera.h VWSCamera.cpp ${inc})
target_include_directories(VWSCamera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_link_libraries(VWSCamera PRIVATE ${LIBS} GL GLU dl)


