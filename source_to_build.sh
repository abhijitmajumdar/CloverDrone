#### CloverDrone Build ####
# You need to source this file to make sure that the catkin setup configures the paths properly
# To build from scratch use as: $ source source_to_build.sh
# To build specific package (to which changes have been made): $ source source_to_build.sh <imu/cloverdrone/ros>
# To clean build specific package use as: $ source source_to_build.sh <imu/cloverdrone/ros> rebuild
# To clean build all packages use as: $ source source_to_build.sh clean

WORKDIR=`pwd`
cd $(dirname "$BASH_SOURCE")
BASEDIR=`pwd`

# Configure package path to be used by program to access configuration files
export CLOVERDRONEDIR=${BASEDIR}


OPTION_TO_BUILD="all"
if [[ $1 ]]; then
  OPTION_TO_BUILD="${1}"
fi

case ${OPTION_TO_BUILD} in
  "clean")
    echo "Cleaning all builds"
    if [ -d ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build ]; then
      rm -rf ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build
    fi
    if [ -d ${CLOVERDRONEDIR}/cloverdrone/build ]; then
      rm -rf ${CLOVERDRONEDIR}/cloverdrone/build
    fi
    if [[ (-d ${CLOVERDRONEDIR}/cloverdrone_ros_ws/build) || (-d ${CLOVERDRONEDIR}/cloverdrone_ros_ws/devel) || (-e ${CLOVERDRONEDIR}/cloverdrone_ros_ws/src/CMakeLists.txt) ]]; then
  		rm -rf ${CLOVERDRONEDIR}/cloverdrone_ros_ws/build ${CLOVERDRONEDIR}/cloverdrone_ros_ws/devel ${CLOVERDRONEDIR}/cloverdrone_ros_ws/src/CMakeLists.txt
  	fi
    ;&
  "all")
    ;&
  "imu")
    echo "(Re)building RTIMULib"
    if [[ ($2) && ($2 == "rebuild") && (-d ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build) ]]; then
      rm -rf ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build
    fi
    if [ ! -d ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build ]; then
      mkdir ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build
      cd ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build
      cmake ..
    else
      cd ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build
    fi
    make -j
    cd ${CLOVERDRONEDIR}/cloverdrone
    if [ -d build ]; then
      rm -rf build
    fi
    ;&
  "cloverdrone")
    echo "(Re)building CloverDrone"
    if [[ ($2) && ($2 == "rebuild") && (-d ${CLOVERDRONEDIR}/cloverdrone/build) ]]; then
      rm -rf ${CLOVERDRONEDIR}/cloverdrone/build
    fi
    if [ ! -d ${CLOVERDRONEDIR}/cloverdrone/build ]; then
      mkdir ${CLOVERDRONEDIR}/cloverdrone/build
      cd ${CLOVERDRONEDIR}/cloverdrone/build
      cmake ..
    else
      cd ${CLOVERDRONEDIR}/cloverdrone/build
    fi
    make
    cd ${CLOVERDRONEDIR}/cloverdrone_ros_ws
    if [[ (-d build) || (-d devel) || (-e src/CMakeLists.txt) ]]; then
      rm -rf build devel src/CMakeLists.txt
    fi
    ;&
  "ros")
    echo "(Re)building cloverdrone_ros package and messages"
    if [[ ($2) && ($2 == "rebuild") ]]; then
      if [[ (-d ${CLOVERDRONEDIR}/cloverdrone_ros_ws/build) || (-d ${CLOVERDRONEDIR}/cloverdrone_ros_ws/devel) || (-e ${CLOVERDRONEDIR}/cloverdrone_ros_ws/src/CMakeLists.txt) ]]; then
        rm -rf ${CLOVERDRONEDIR}/cloverdrone_ros_ws/build ${CLOVERDRONEDIR}/cloverdrone_ros_ws/devel ${CLOVERDRONEDIR}/cloverdrone_ros_ws/src/CMakeLists.txt
      fi
    fi
    cd ${CLOVERDRONEDIR}/cloverdrone_ros_ws
    catkin_make
    source devel/setup.sh
    ;;
  *)
    echo "Invalid command!"
    ;;
esac

cd ${WORKDIR}
