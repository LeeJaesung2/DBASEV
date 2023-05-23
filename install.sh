# Update package lists
apt-get update -y

# Upgrade all installed packages
apt-get upgrade -y

#install python2.7
apt-get install -y python2.7-dev

#change pip and python version

#dronekit here

#for message queue

# #WiringPi
# cd lib
# git clone https://github.com/WiringPi/WiringPi
# cd WiringPi
# ./build
# cd ../..

#ws2811(neopixel)
cd lib
git clone https://github.com/jgarff/rpi_ws281x
cd rpi_ws281x
mkdir build
cd build
cmake -D BUILD_SHARED=OFF -D BUILD_TEST=ON ..
cmake --build .
make install
cd ../../..