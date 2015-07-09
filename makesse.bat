@echo off
call setenv.bat
make -f makedxe3.sse clean
make -f makedxe3.sse
upx -9 q2sse.exe
