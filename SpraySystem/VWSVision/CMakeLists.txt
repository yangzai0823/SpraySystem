cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 14)
# ${PROJECT_NAME}：本CMakeLists.txt的project名称
project(VWSVision  LANGUAGES CXX)
enable_language(C )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb3")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# find_package(Poco 1.4.3 REQUIRED COMPONENTS Foundation Net Util XML)
# file(GLOB_RECURSE srcs Lib/*)
#配置halcon，include、lib目录，具体的lib在target_link_libraries里(注意不加.lib后缀)

#系统环境变量$ENV，也可以换成自己的绝对路径
# 5. 添加头文件路径
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/heightandpose)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/eigen3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/head/heightandpose)
# message(STATUS "----------------------------      ${CMAKE_CURRENT_SOURCE_DIR}/head/heightandpose")
# 6. 添加源文件
FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# link_directories("/opt/halcon/lib/x64-linux/libhalcon.so"
# "/opt/halcon/lib/x64-linux/libhalconcpp.so")

# 规定.so/.a库文件路径
link_libraries("/opt/halcon/lib/x64-linux/libhalcon.so"
"/opt/halcon/lib/x64-linux/libhalconcpp.so")

# 将.cpp/.c/.cc文件生成可执行文件
# add_executable(可执行文件名称 文件)
# add_executable(PCLMeasure HeightAndPose.cpp ) # 生成静态库/动态库时需要注销该行
# # 9. 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，
# #但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(ALL_SRCS ${SOURCE_FILES})
message(${ALL_SRCS})

# 生成so包
ADD_LIBRARY(${PROJECT_NAME} SHARED ${ALL_SRCS})
#add_library(Test2_  PointCloudMeasure.cpp ) ## 生成静态库
# add_library(${PROJECT_NAME}   SHARED  HeightAndPose.cpp )  # 生成动态库
# add_executable(PCLMeasure  main.cpp ) 

# 对add_library或add_executable生成的文件进行链接操作
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
# target_link_libraries(Test3_  libPCLMeasure)
# target_link_libraries(VWSPcl PUBLIC
#   ${Poco_LIBRARIES}
# )

# cmake_minimum_required(VERSION 3.5)

# project(VWSVision)


# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb3")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# # 3. opencv
# #link_directories("/usr/local/lib")
# #include_directories("/usr/local/include/opencv4")

# # 5. 添加头文件路径
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/utils)
# # 6. 添加源文件
# FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# message(${CMAKE_CURRENT_SOURCE_DIR})
# message(${SOURCE_FILES})
# # 7. 添加链接库
# #LINK_LIBRARIES(opencv_core opencv_highgui)

# # 9. 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，
# #但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
# SET(ALL_SRCS ${SOURCE_FILES})
# message(${ALL_SRCS})

# # 生成so包
# ADD_LIBRARY(${PROJECT_NAME} SHARED ${ALL_SRCS})

# 10.add executable file，添加要编译的可执行文件
#ADD_EXECUTABLE(${PROJECT_NAME} ${ALL_SRCS})
