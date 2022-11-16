cmake_minimum_required(VERSION 3.5)

project(SpraySystem LANGUAGES CXX)
# add_definitions(-DPLAN_FAKE_DATA)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PREFIX_PATH /opt/Qt5.12.8/5.12.8/gcc_64/lib/cmake/)
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)


file(GLOB_RECURSE form Forms/*)
file(GLOB_RECURSE data DataAccess/*)
file(GLOB_RECURSE util Util/*)
file(GLOB_RECURSE device Device/*)
file(GLOB_RECURSE bussiness Bussiness/*)
file(GLOB_RECURSE trajectory Trajectory/*)
file(GLOB_RECURSE vision Vision/*)
file(GLOB_RECURSE libs Libs/*)
file(GLOB_RECURSE struct Data/*)

message(STATUS "**** ${inc}")


set(CATKIN_ENABLE_TESTING OFF)

find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS system python thread program_options REQUIRED)
find_package(OpenRAVE REQUIRED)
find_package(ompl)
find_package(urdfdom)
find_package(Poco REQUIRED COMPONENTS Foundation Net Util XML)
find_package(catkin REQUIRED COMPONENTS
    srdfdom
    urdf
    urdf2or
    )
option(USE_OCCT "use occt " OFF)
if(USE_OCCT)
  find_package(OpenCASCADE REQUIRED)
endif()
# find_package(VTK REQUIRED)
find_package(OpenSceneGraph REQUIRED)
link_directories(Libs)

find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgUtil )
find_package( osgViewer REQUIRED)
find_package(osgGA)

set(OSG_LIBRARIES osg osgViewer OpenThreads osgDB osgGA osgUtil osgText)


include_directories(
  SYSTEM
  ${Boost_INCLUDE_DIRS}
  ${Eigen_INCLUDE_DIRS}
  ${OpenRAVE_INCLUDE_DIRS}
  ${JSON_INCLUDE_DIRS}
  ${OMPL_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
   ${TRAJOPT_ROOT_DIR}/src/
   Include/
   Include/opt/
   Include/json/include/

  ${OpenCASCADE_INCLUDE_DIR}
)
link_directories(VWSCamera/Lib)


if(ANDROID)
  add_library(SpraySystem SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(SpraySystem
    ${form}
    ${data}
    ${util}
    ${device}
    ${bussiness}
    ${trajectory}
    ${vision}
    ${libs}
    ${struct}

    main.cpp
    Resources.qrc

    Include/Core/Config/config.hpp
    Include/Core/Config/config.cpp
    Include/Core/Config/jsonserializer.cpp
    Include/Core/Config/jsonserializer.h
    Include/Core/visibility_control.hpp


#    Bussiness/devicemonitor.h
#    Bussiness/devicemonitor.cpp
  )
endif()


#target_link_directories(SpraySystem PRIVATE )
add_subdirectory(VWSCamera)
add_subdirectory(VWSRobot)
add_subdirectory(VWSVision)
message(STATUS "${OPENSCENEGRAPH_LIBRARIES}")
set( planlibs
trajopt
 ${Boost_SYSTEM_LIBRARY}
 ${Boost_PROGRAM_OPTIONS_LIBRARY}
 ${OpenRAVE_CORE_LIBRARIES}
 ${OpenRAVE_LIBRARIES}
 ${OMPL_LIBRARIES}
 ${urdfdom_LIBRARIES}
 ${urdf_LIBRARIES}
 ${catkin_LIBRARIES}
 ${OPENSCENEGRAPH_LIBRARIES}
 ${OSG_LIBRARIES}
 osgviewer
 json
 sco
 utils
 robot_ik
 robot_plan
 )

target_link_libraries(SpraySystem PRIVATE
    Qt5::Widgets
    Qt5::Network
    VWSCamera
    VWSRobot
    VWSVision
    Poco::Net
    ${planlibs}
    MediaProcess
${OSGVIEWER_LIBRARY_RELEASE}
    )
file(GLOB_RECURSE binfiles Trajectory/Libs/bin/*)
file(GLOB_RECURSE libfiles Libs/* VWSCamera/Lib/*)
message(STATUS "${libfiles}")
add_custom_command(TARGET SpraySystem
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${CMAKE_BINARY_DIR}/urdf
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Trajectory/Libs/urdf ${CMAKE_BINARY_DIR}/urdf/
    COMMAND ${CMAKE_COMMAND} -E
    copy_if_different ${binfiles} ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${CMAKE_COMMAND} -E
    copy_if_different ${libfiles} ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Data/Data/  ${CMAKE_BINARY_DIR}/Data
    )


