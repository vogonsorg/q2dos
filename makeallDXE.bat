@echo off
call clean.bat
call setenv.bat

make -f makedxe.frank clean
make -f makedxe.frank
strip q2.exe
upx -9 q2.exe

cd game
make -f makedx3 clean
make -f makedx3
cd..

cd ctf
make -f makedx3 clean
make -f makedx3
cd..

cd 3zb2
make -f makedx3 clean
make -f makedx3
cd..

cd acebot
make -f makedx3 clean
make -f makedx3
cd ..

cd action
make -f makedx3 clean
make -f makedx3
cd ..

cd chaos
make -f makedx3 clean
make -f makedx3
cd ..

cd dday
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
