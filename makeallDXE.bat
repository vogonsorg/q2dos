@echo off
call clean.bat
call setenv.bat

make -f Makefile.dj clean
make -f Makefile.dj
strip q2.exe
upx -9 q2.exe

cd ref_soft
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd ref_gl
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd Goa
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd game
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd ctf
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd 3zb2
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd acebot
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd action
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd chaos
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd dday
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd rsrc
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd xsrc
make -f Makefile.dj clean
make -f Makefile.dj
cd ..

cd zaero
make -f Makefile.dj clean
make -f Makefile.dj
