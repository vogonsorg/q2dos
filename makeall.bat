@echo off
call clean.bat
call makeq2.bat
upx -9 q2.exe
call makeq2ace.bat
upx -9 q2ace.exe
call makeq2rogue.bat
upx -9 q2rogue.exe
call makeq2xatrix.bat
upx -9 q2xatrix.exe
