#!/bin/bash
cd ../src/frontend/explorer2
#npm install
#ng build 
cd ../../backend
mkdir build
cd build
cmake ..
make

cp libOE2.dylib ../../../binaries