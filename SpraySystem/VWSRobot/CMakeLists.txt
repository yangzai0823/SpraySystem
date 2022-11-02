cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 14)
#project(test LANGUAGES CXX)
enable_language(C )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Poco 1.4.3 REQUIRED COMPONENTS Foundation Net Util XML)
file(GLOB_RECURSE srcs ${CMAKE_CURRENT_SOURCE_DIR}/Lib/*)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/Lib/libabb_librws.so)
add_library(VWSRobot SHARED VWSRobot.h VWSRobot.cpp  ${srcs})
target_include_directories(VWSRobot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Lib)
target_link_libraries(VWSRobot PRIVATE
  ${Poco_LIBRARIES} pthread
)
