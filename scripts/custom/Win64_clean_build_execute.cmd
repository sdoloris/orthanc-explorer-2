cd ../../src/backend
rmdir /Q /S build
mkdir build
cd build
call cmake -G "Visual Studio 15 2017 Win64" ..
call cmake --build . --target OE2 --config Release
cd Release
cp OE2.dll C:\Users\merte\Documents\Orthanc
cd C:\Users\merte\Documents\Orthanc
call startOrthanc.bat
cd C:\Users\merte\Documents\orthanc-explorer-2\scripts\custom