cmake_minimum_required(VERSION 2.4.6)
project(cloverdrone_ros)

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
   add_compile_options(-std=c++11)
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  roslib
  std_msgs
  cloverdrone_ros_msgs
)

# Refer http://wiki.ros.org/catkin/CMakeLists.txt
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp rospy std_msgs roslib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/../../../cloverdrone/include
  ${PROJECT_SOURCE_DIR}/../../../cloverdrone/include/HAL
  ${PROJECT_SOURCE_DIR}/../../../RTIMULib/RTIMULib
  ${PROJECT_SOURCE_DIR}/../../../RTIMULib/RTIMULib/IMUDrivers
  /usr/lib/
)

set(LIBS
  ${PROJECT_SOURCE_DIR}/../../../RTIMULib/RTIMULib/build/libRTIMULib.so
  ${PROJECT_SOURCE_DIR}/../../../cloverdrone/lib/libcloverdrone.so
  ${catkin_LIBRARIES}
  pthread
)

add_executable(${PROJECT_NAME}_node src/main.cpp src/interfacesROS.cpp src/ros_interface.cpp)
target_link_libraries(${PROJECT_NAME}_node ${LIBS})
add_dependencies(${PROJECT_NAME}_node ${catkin_EXPORTED_TARGETS})
