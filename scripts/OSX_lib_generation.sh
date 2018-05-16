#!/bin/bash
cd ../src/frontend
rm -rf staticRessource
mkdir staticRessource
cd explorer2
npm install
npm install -g @angular/cli
ng build 
cd ../../backend
rm -rf build
mkdir build
cd build
cmake ..
make

cp libOE2.dylib ../../../binaries