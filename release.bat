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
del Q2DOS_EXE_LATEST.7Z
rd /s /q release

md release
cd release

md baseq2

md ctf

md 3zb2

md ace

md action

md chaos

md dday

md rogue

md xatrix

md zaero
cd ..

copy q2.exe release
copy cwsdpmi.exe release
copy readme.txt release
copy goa\gamespy.dxe release
copy game\gamex86.dxe release\baseq2
copy ctf\gamex86.dxe release\ctf
copy 3zb2\gamex86.dxe release\3zb2
copy 3zb2\3ZB.cfg release\3zb2
copy acebot\gamex86.dxe release\ace
copy action\gamex86.dxe release\action
copy chaos\gamex86.dxe release\chaos
copy dday\gamex86.dxe release\dday
copy rsrc\gamex86.dxe release\rogue
copy xsrc\gamex86.dxe release\xatrix
copy zaero\gamex86.dxe release\zaero

C:
cd\
cd "program files"
cd "7-zip"
REM Uploading this as _LATEST.7Z to bitbucket now
REM 7z.exe a -mx9 -mmt "F:\PROJ\Q2DOS\Q2DOS_EXE_%MONTH%%DAY%%YEAR%.7Z" "F:\PROJ\Q2DOS\RELEASE\*"

7z.exe a -mx9 -mmt "F:\PROJ\Q2DOS\Q2DOS_EXE_LATEST.7Z" "F:\PROJ\Q2DOS\RELEASE\*"

cd F:\PROJ\Q2DOS
rd /s /q F:\PROJ\Q2DOS\RELEASE

F:
cd "F:\PROJ\Q2DOS"
