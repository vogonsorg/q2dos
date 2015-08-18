make -f makedxe3_v1.frank clean
make -f makedxe3_v1.frank
strip q2fx_v1.exe
upx -9 q2fx_v1.exe

make -f makedxe3_v2.frank
strip q2fx_v2.exe
upx -9 q2fx_v2.exe

make -f makedxe3_v3.frank
strip q2fx_v3.exe
upx -9 q2fx_v3.exe

make -f makedxe3_v5.frank
strip q2fx_v5.exe
upx -9 q2fx_v5.exe
