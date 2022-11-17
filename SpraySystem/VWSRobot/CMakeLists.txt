cmake_minimum_required(VERSION 3.20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(VWSRobot SHARED ftp.cpp ftp.h VWSRobot.h VWSRobot.cpp tcpClient.h tcpClient.cpp)

#target_link_libraries(YASKAWA PRIVATE pthread)
