@echo off
call setenv.bat
make -f makedxe3.sse clean
make -f makedxe3.sse
upx -9 q2sse.exe
cd ref_soft
make -f makedxe3 clean
make -f makedxe3 USE_SSE=1
