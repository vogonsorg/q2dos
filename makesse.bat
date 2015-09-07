@echo off
call setenv.bat
make -f makedxe3.frank clean
make -f makedxe3.frank USE_SSE=1
upx -9 q2sse.exe
