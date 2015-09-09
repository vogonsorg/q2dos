@echo off
call setenv.bat
make -f Makefile.dj clean
make -f Makefile.dj USE_SSE=1
upx -9 q2sse.exe
