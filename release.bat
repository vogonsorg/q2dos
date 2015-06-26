@echo off
call setenv.bat
call makeallDXE.bat

ERRTIME -y > NUL

FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %%A0 SET ERR10=%%A
FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %ERR10%%%A SET ERR1=%%A
SET YEAR=%ERR10%%ERR1%

ERRTIME -m > NUL

FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %%A0 SET ERR10=%%A
FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %ERR10%%%A SET ERR1=%%A
SET MONTH=%ERR10%%ERR1%

ERRTIME -d > NUL

FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %%A0 SET ERR10=%%A
FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %ERR10%%%A SET ERR1=%%A
SET DAY=%ERR10%%ERR1%

F:
cd "F:\PROJ\Q2DOS"
del Q2DOS_EXE_%MONTH%%DAY%%YEAR%.7Z
rd /s /q release

md release
cd release

md baseq2

md ctf

md ace

md rogue

md xatrix

md zaero
cd ..

copy q2.exe release
copy cwsdpmi.exe release
copy readme.txt release
copy game\gamex86.dx3 release\baseq2
copy ctf\gamex86.dx3 release\ctf
copy acebot\gamex86.dx3 release\ace
copy rsrc\gamex86.dx3 release\rogue
copy xsrc\gamex86.dx3 release\xatrix
copy zaero\gamex86.dx3 release\zaero

C:
cd\
cd "program files"
cd "7-zip"
7z.exe a -mx9 -mmt "F:\PROJ\Q2DOS\Q2DOS_EXE_%MONTH%%DAY%%YEAR%.7Z" "F:\PROJ\Q2DOS\RELEASE\*"
cd F:\PROJ\Q2DOS
rd /s /q F:\PROJ\Q2DOS\RELEASE

F:
cd "F:\PROJ\Q2DOS"
