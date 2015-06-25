@echo off
call clean.bat
call setenv.bat

make -f makedxe.frank clean
make -f makedxe.frank
upx -9 q2dxe.exe

cd game
make -f makedx3 clean
make -f makedx3
cd..

cd acebot
make -f makedx3 clean
make -f makedx3
cd ..

cd rsrc
make -f makedx3 clean
make -f makedx3
cd ..

cd xsrc
make -f makedx3 clean
make -f makedx3
cd ..

cd zaero
make -f makedx3 clean
make -f makedx3
