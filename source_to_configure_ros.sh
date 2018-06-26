echo "Configuring variables"

# Finding my hostname and mac address
EthernetAdapterName=`ls /sys/class/net | awk '/en/ || /eth/'`
WifiAdapterName=`ls /sys/class/net | awk '/wlan/ || /wl/'`
if [ ! -z "${EthernetAdapterName}" ]; then
  mymaceth=`cat /sys/class/net/${EthernetAdapterName}/address`
	ipeth=`ifconfig | grep ${EthernetAdapterName} -A1 | grep "inet addr" | awk -F" " {'print $2'} | awk -F: {'print $2'}`
fi
if [ ! -z "${WifiAdapterName}" ]; then
  mymacwlan=`cat /sys/class/net/${WifiAdapterName}/address`
	ipwlan=`ifconfig | grep ${WifiAdapterName} -A1 | grep "inet addr" | awk -F" " {'print $2'} | awk -F: {'print $2'}`
fi

# Prefer ethernet for mac address
if [ ! -z ${mymaceth} ]; then
	mymac="${mymaceth}"
elif [ ! -z ${mymacwlan} ]; then
	mymac="${mymacwlan}"
else
	mymac=""
fi
mymac_simplestring="${mymac[@]//:}"

# Use IP from whichever network is connected, prefer ethernet
ip='127.0.0.1'
if [ ! -z "${ipwlan}" ]; then
    ip=${ipwlan}
fi
if [ ! -z "${ipeth}" ]; then
    ip=${ipeth}
fi

myhostname=`cat /etc/hostname`
# If no hostname is configured, use mac addr
if [ -z "$myhostname" ]; then
    echo 'No hostname configured, using MAC address'
    myhostname="${mymac_simplestring}"
fi

echo "Hostname = $myhostname"
echo "MAC = $mymac"
echo "IP = $ip"

# Configuring System ROS variables
rosmaster="http://localhost:11311/"
#rosmaster="http://${ip}:11311/"
#rosmaster="http://192.168.50.129:11311/"
export ROS_NAMESPACE="${mymac_simplestring}"
export ROS_IP=${ip}
export ROS_MASTER_URI=${rosmaster}
echo "ROS_NAMESPACE = $ROS_NAMESPACE"
echo "ROS_IP = $ROS_IP"
echo "ROS_MASTER_URI = $ROS_MASTER_URI"
