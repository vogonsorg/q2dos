@echo off
call clean.bat
call setenv.bat

make -f makedxe3.frank clean
make -f makedxe3.frank
strip q2.exe
upx -9 q2.exe

cd ref_soft
make -f makedxe3 clean
make -f makedxe3
cd..

cd ref_gl
make -f makedxe3 clean
make -f makedxe3
cd..

cd Goa
make -f makedxe3 clean
make -f makedxe3
cd..

cd game
make -f makedxe3 clean
make -f makedxe3
cd..

cd ctf
make -f makedxe3 clean
make -f makedxe3
cd..

cd 3zb2
make -f makedxe3 clean
make -f makedxe3
cd..

cd acebot
make -f makedxe3 clean
make -f makedxe3
cd ..

cd action
make -f makedxe3 clean
make -f makedxe3
cd ..

cd chaos
make -f makedxe3 clean
make -f makedxe3
cd ..

cd dday
make -f makedxe3 clean
make -f makedxe3
cd ..

cd rsrc
make -f makedxe3 clean
make -f makedxe3
cd ..

cd xsrc
make -f makedxe3 clean
make -f makedxe3
cd ..

cd zaero
make -f makedxe3 clean
make -f makedxe3
