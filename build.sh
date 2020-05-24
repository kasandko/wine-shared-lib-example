#!/bin/env bash

echo Clean...
rm -rf ./bin
rm ./executable/executable.exe
rm ./sharedlib/main.o
rm ./sharedlib/Makefile
rm ./sharedlib/sharedlib.dll.so

echo Build PE executable...
cd ./executable
x86_64-w64-mingw32-g++ ./main.cpp -o ./executable.exe
cd ..

echo Build shared Wine lib...
cd ./sharedlib
winemaker --dll --nosource-fix .
make LDFLAGS="$LDFLAGS --fPIC"
cd ..

echo Prepare files...
mkdir ./bin
cp /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll ./bin/libgcc_s_seh-1.dll
cp /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll ./bin/libstdc++-6.dll
cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll ./bin/libwinpthread-1.dll
cp ./executable/executable.exe ./bin/executable.exe
cp ./sharedlib/sharedlib.dll.so ./bin/sharedlib.dll

echo Run...
cd ./bin
wine ./executable
