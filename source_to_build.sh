#### CloverDrone Build ####
# You need to source this file to make sure that the catkin setup configures the paths properly
# Usage: source source_to_build.sh <BUILD COMPONENT> <CONFIGURATIONS>
# BUILD COMPONENT = [all(default) | clean | cloverdrone | ros]
# CONFIGURATIONS = [--rebuild] [--noros]

# To build from scratch use as: $ source source_to_build.sh
# To build specific package (to which changes have been made): $ source source_to_build.sh <imu/cloverdrone/ros>
# To clean build specific package use as: $ source source_to_build.sh <imu/cloverdrone/ros> --rebuild
# To clean build all packages use as: $ source source_to_build.sh clean
# To build specific package (to which changes have been made) without ROS: $ source source_to_build.sh <imu/cloverdrone/ros> --noros

WORKDIR=`pwd`
cd $(dirname "$BASH_SOURCE")
BASEDIR=`pwd`

# Configure package path to be used by program to access configuration files
export CLOVERDRONEDIR=${BASEDIR}

# Default build configurations
OPTION_TO_BUILD="all"
unset ROSEXISTS
unset NOROS
unset REBUILD

# Check if build component defined
if [[ $1 ]]; then
  if [[ $1 =~ ^(clean|all|imu|cloverdrone|ros)$ ]]; then
    OPTION_TO_BUILD="${1}"
  fi
fi

# Check if ROS is installed on system
if [ -x "$(command -v rosversion)" ]; then
  ROSEXISTS=1
fi

# Check other arguments for build configurations
for arg in $@
do
  #echo $arg
  if [ $arg == "--noros" ]; then
    NOROS=1
  elif [ $arg == "--rebuild" ]; then
    REBUILD=1
  fi
done

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
    if [[ ($REBUILD) && (-d ${CLOVERDRONEDIR}/RTIMULib/RTIMULib/build) ]]; then
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
    if [[ ($REBUILD) && (-d ${CLOVERDRONEDIR}/cloverdrone/build) ]]; then
      rm -rf ${CLOVERDRONEDIR}/cloverdrone/build
    fi
    if [ ! -d ${CLOVERDRONEDIR}/cloverdrone/build ]; then
      mkdir ${CLOVERDRONEDIR}/cloverdrone/build
      cd ${CLOVERDRONEDIR}/cloverdrone/build
      cmake ..
    else
      cd ${CLOVERDRONEDIR}/cloverdrone/build
    fi
    make -j
    cd ${CLOVERDRONEDIR}/cloverdrone_ros_ws
    if [[ (-d build) || (-d devel) || (-e src/CMakeLists.txt) ]]; then
      rm -rf build devel src/CMakeLists.txt
    fi
    ;&
  "ros")
    if [[ ($ROSEXISTS) && ( -z $NOROS ) ]]; then
      echo "(Re)building cloverdrone_ros package and messages"
      if [[ ($REBUILD) ]]; then
        if [[ (-d ${CLOVERDRONEDIR}/cloverdrone_ros_ws/build) || (-d ${CLOVERDRONEDIR}/cloverdrone_ros_ws/devel) || (-e ${CLOVERDRONEDIR}/cloverdrone_ros_ws/src/CMakeLists.txt) ]]; then
          rm -rf ${CLOVERDRONEDIR}/cloverdrone_ros_ws/build ${CLOVERDRONEDIR}/cloverdrone_ros_ws/devel ${CLOVERDRONEDIR}/cloverdrone_ros_ws/src/CMakeLists.txt
        fi
      fi
      cd ${CLOVERDRONEDIR}/cloverdrone_ros_ws
      catkin_make
      source devel/setup.sh
    fi
    ;;
  *)
    echo "Invalid command!"
    echo "Usage: source source_to_build.sh <BUILD COMPONENT> <CONFIGURATIONS>"
    echo "BUILD COMPONENT = [all(default) | clean | cloverdrone | ros]"
    echo "CONFIGURATIONS = [--rebuild] [--noros]"
    ;;
esac

cd ${WORKDIR}
