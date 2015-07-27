@echo off
call setenv.bat
make -f makedxe3.sse clean
make -f makedxe3.sse
cd ref_soft
make -f makedxe3 clean
make -f makedxe3 USE_SSE=1
upx -9 q2sse.exe
