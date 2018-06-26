cmake_minimum_required(VERSION 2.4.6)
project(cloverdrone_ros_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  std_msgs
  sensor_msgs
  geometry_msgs
)

add_message_files(FILES
  DroneStatus.msg
  DroneTarget.msg
)

generate_messages(DEPENDENCIES std_msgs sensor_msgs geometry_msgs)

catkin_package(
  CATKIN_DEPENDS message_runtime std_msgs geometry_msgs
)
