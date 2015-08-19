make -f makedxe3_3dfx.frank clean
make -f makedxe3_3dfx.frank
strip q2fx.exe
upx -9 q2fx.exe

cd ref_3dfx
make -f makedxe3 clean
make -f makedxe3
