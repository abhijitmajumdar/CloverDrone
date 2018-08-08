# Most of the configurations here are to modify hardware resource access
# This on most systems will require superuser access
# Ex. use $ sudo ./source_to_access_resources.sh

WORKDIR=`pwd`
cd $(dirname "$BASH_SOURCE")
BASEDIR=`pwd`

if [ -z ${SUDO_USER} ]; then
  echo "Please run with sudo(as root)"
  exit
fi

echo 'Configuring access to hardware peripherals (USB/I2C/SPI/GPIO)'
# Enabling i2c,spi,gpio on RPi
if [ -z `grep -Fx "i2c-dev" /etc/modules` ]; then
  sudo echo "i2c-dev" >> /etc/modules
fi
if [ -z `grep -Fx "i2c-bcm2708" /etc/modules` ]; then
  sudo echo "i2c-bcm2708" >> /etc/modules
fi
if [ -z `grep -Fx "bcm2835-gpiomem" /etc/modules` ]; then
  sudo echo "bcm2835-gpiomem" >> /etc/modules
fi
grep -v -e "blacklist i2c-bcm2708" -e "blacklist spi-bcm2708" /etc/modprobe.d/raspi-blacklist.conf > temp
sudo mv temp /etc/modprobe.d/raspi-blacklist.conf

# Non-sudo access to harware peripherals
sed 's@{USER}@'"${SUDO_USER}"'@g' ${BASEDIR}/hardware_files/999-perif-i2c-spi-serial-gpio.rules.copy > ${BASEDIR}/hardware_files/999-perif-i2c-spi-serial-gpio.rules
sudo cp ${BASEDIR}/hardware_files/*.rules /etc/udev/rules.d/

# Configure stuff
BOOTCONFIGFILE="/boot/config.txt"
grep -v -e "dtparam=i2c" -e "dtparam=spi" ${BOOTCONFIGFILE} > temp
echo "dtparam=i2c1_arm=on" >> temp
echo "dtparam=i2c1_baudrate=400000" >> temp
echo "dtparam=spi=on" >> temp
sudo mv temp ${BOOTCONFIGFILE}


echo 'Configuring core isolation'
# Backup "dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait isolcpus=3"
# Isolate the 4th core (in 0-3)
CHECK_CORE_ISOLATION=`cat /boot/cmdline.txt | grep isolcpus`
if [ -z "${CHECK_CORE_ISOLATION}" ]; then
  sudo echo "`cat /boot/cmdline.txt` isolcpus=3" > /boot/cmdline.txt
fi
# For further isolation refer this https://www.kernel.org/doc/Documentation/IRQ-affinity.txt
# where we set the affinity for ISRs to the other cores

cd ${WORKDIR}
