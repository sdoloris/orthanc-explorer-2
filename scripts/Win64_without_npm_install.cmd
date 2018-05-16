cd ../src/frontend/explorer2
call ng build 
cd ../../backend
mkdir build
cd build
call cmake -G "Visual Studio 15 2017 Win64" ..
call cmake --build . --target OE2 --config Release
cd Release
cp OE2.dll ../../../../binaries
cd ../../../../scripts