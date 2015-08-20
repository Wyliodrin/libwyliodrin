#!/bin/sh

# Raspberry Pi install script
#
# !!! BEFORE RUNNING THIS SCRIPT !!!
# apt-get install raspi-config
# raspi-config
# Select 1 Expand Filesystem.
# Select 8 Advanced Options and then  A7 I2C - Enable/Disable automatic loading.
# A prompt will appear asking "Would you like the ARM I2C interface to be
# enabled?". Select Yes, exit the utility and reboot your raspberry pi.
# add "dtparam=i2c1=on" and "dtparam=i2c_arm=on" in /boot/config.txt.
# add "i2c-dev" in /etc/modules.
# Follow [1] for more details on how to enable I2C on your raspberry pi.
# add "/usr/local/bin/supervisord -c /etc/supervisord.conf" in /etc/rc.local.
#
# [1] https://www.abelectronics.co.uk/i2c-raspbian-wheezy/info.aspx

# Install some stuff
apt-get update
apt-get install -y git gcc g++ gcc-4.7 g++-4.7 make pkg-config libexpat1-dev  \
  libssl-dev libhiredis-dev dh-autoreconf libfuse-dev libcurl4-gnutls-dev     \
  libevent-dev redis-server python-dev libi2c-dev python-pip libjansson-dev   \
  cmake mc mplayer arduino minicom picocom bluez-utils bluez-compat           \
  bluez-hcidump libusb-dev libbluetooth-dev bluetooth joystick python-smbus

# Use gcc and g++ 4.7
update-alternatives --remove-all gcc
update-alternatives --remove-all g++
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 1             \
  --slave /usr/bin/g++ g++ /usr/bin/g++-4.6
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.7 2             \
  --slave /usr/bin/g++ g++ /usr/bin/g++-4.7
update-alternatives --auto gcc

# Install some more stuff
pip install msgpack-python
pip install redis
pip install ino
pip install pyfirmata

# Create sandbox directory
mkdir /etc/sandbox

# Install libstrophe
cd /etc/sandbox
git clone https://github.com/strophe/libstrophe.git
cd libstrophe
./bootstrap.sh
./configure --prefix=/usr
make
make install

# Install node
cd /etc/sandbox
wget https://nodejs.org/dist/v0.10.28/node-v0.10.28.tar.gz
tar -xzf node-v0.10.28.tar.gz
rm node-v0.10.28.tar.gz
cd node-v0.10.28
./configure --prefix=/usr
make
make install

# Install serialport
cd /etc/sandbox
npm install voodootikigod/node-serialport

# Install wiringPi
cd /etc/sandbox
git clone https://github.com/Wyliodrin/wiringPi.git
cd wiringPi
sed 's/sudo//g' build > build2
chmod +x build2
./build2

# Install pcre
cd /etc/sandbox
wget ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.36.tar.gz
tar -xzf pcre-8.36.tar.gz
rm pcre-8.36.tar.gz
cd pcre-8.36
./configure --prefix=/usr
make
make install

# Install swig 3+
cd /etc/sandbox
wget http://prdownloads.sourceforge.net/swig/swig-3.0.2.tar.gz
tar -xzf swig-3.0.2.tar.gz
rm swig-3.0.2.tar.gz
cd swig-3.0.2
./configure --prefix=/usr
make
make install

# Install libwyliodrin
cd /etc/sandbox
git clone https://github.com/Wyliodrin/libwyliodrin.git
cd libwyliodrin
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DRASPBERRYPI=ON ..
make
make install

# Install wyliodrin-server
cd /etc/sandbox
git clone https://github.com/alexandruradovici/wyliodrin-server.git
cd wyliodrin-server
git checkout clean
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
make
make install

# Set boardtype to raspberry
mkdir /etc/wyliodrin
echo -n raspberrypi > /etc/wyliodrin/boardtype

# Create mount and build directories
mkdir /etc/wyliodrin/mnt
mkdir /etc/wyliodrin/build

# Create settings_raspberry.json
printf "{\n\
  \"config_file\": \"/boot/wyliodrin.json\",\n\
  \"mountFile\": \"/etc/wyliodrin/mnt\",\n\
  \"buildFile\": \"/etc/wyliodrin/build\",\n\
  \"board\": \"raspberrypi\"\n\
}\n" > /etc/wyliodrin/settings_raspberrypi.json

# Create running_projects file
touch /etc/wyliodrin/running_projects

# I2C support
cd /etc/sandbox
apt-get install -y python3-dev
wget http://ftp.de.debian.org/debian/pool/main/i/i2c-tools/i2c-tools_3.1.0.orig.tar.bz2
tar xf i2c-tools_3.1.0.orig.tar.bz2
rm i2c-tools_3.1.0.orig.tar.bz2
cd i2c-tools-3.1.0/py-smbus
mv smbusmodule.c smbusmodule.c.orig
wget https://raw.githubusercontent.com/abelectronicsuk/ABElectronics_Python3_Libraries/master/smbusmodule.c
python3 setup.py build
python3 setup.py install

# Startup script
easy_install supervisor
printf "\
[supervisord]\n\
[program:wtalk]\n\
command=/usr/bin/wyliodrind\n"\
> /etc/supervisord.conf
