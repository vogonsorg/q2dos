make -f makedxe3.frank clean
make -f makedxe3.frank EXE=q2fx.exe
strip q2fx.exe
upx -9 q2fx.exe

cd ref_gl
make -f makedxe3 clean
make -f makedxe3
