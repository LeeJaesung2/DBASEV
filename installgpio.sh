mkdir lib

#WiringPi
cd lib
git clone https://github.com/WiringPi/WiringPi
cd WiringPi
./build
cd ../..

#ws2811(neopixel)
cd lib
git clone https://github.com/jgarff/rpi_ws281x
cd rpi_ws281x
apt-get install scons
scons
mkdir build
cd build
apt install cmake -y
cmake -D BUILD_SHARED=OFF -D BUILD_TEST=ON ..
cmake --build .
make install
cd ../../..