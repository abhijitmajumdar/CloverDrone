cmake_minimum_required (VERSION 2.8)
project (cloverdrone C CXX)

message(STATUS "Compiling cloverdrone")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_DOUBLE")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/HAL
  ${PROJECT_SOURCE_DIR}/../RTIMULib/RTIMULib
  ${PROJECT_SOURCE_DIR}/../RTIMULib/RTIMULib/IMUDrivers
  /usr/lib/
)

set( CD_SOURCES
  src/sensors.cpp
  src/actuators.cpp
  src/controller.cpp
  src/configuration.cpp
  src/stability_controller.cpp
  src/interfaces.cpp
  src/utils.cpp
  src/HAL/hardware_communication.cpp
  src/HAL/rc_interface.cpp
  src/HAL/pca9685.cpp
  src/HAL/gpio_access.cpp
)

set( CD_LIBS
  ${PROJECT_SOURCE_DIR}/../RTIMULib/RTIMULib/build/libRTIMULib.so
  pthread
)

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/ )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/ )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )

add_library(${PROJECT_NAME} STATIC ${CD_SOURCES})
target_link_libraries(${PROJECT_NAME} ${CD_LIBS} )
add_executable(${PROJECT_NAME}_flight  src/main.cpp)
target_link_libraries(${PROJECT_NAME}_flight ${PROJECT_NAME} )
set_target_properties(${PROJECT_NAME}_flight
                      PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
                      PREFIX "")
