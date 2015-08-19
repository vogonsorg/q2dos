@echo off
call setenv.bat
call make3dfx.bat

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
del Q2DOS_3DFX_%MONTH%%DAY%%YEAR%.7Z
del Q2DOS_3DFX_LATEST.7Z
rd /s /q release

md release

copy q2fx.exe release
copy ref_gl\ref_gl.dxe release
copy readme.txt release
xcopy dos\3rdparty\lib_dxe release /E

C:
cd\
cd "program files"
cd "7-zip"
REM Uploading this as _LATEST.7Z to bitbucket now
REM 7z.exe a -mx9 -mmt "F:\PROJ\Q2DOS\Q2DOS_3DFX_%MONTH%%DAY%%YEAR%.7Z" "F:\PROJ\Q2DOS\RELEASE\*"

7z.exe a -mx9 -mmt "F:\PROJ\Q2DOS\Q2DOS_3DFX_LATEST.7Z" "F:\PROJ\Q2DOS\RELEASE\*"

cd F:\PROJ\Q2DOS
rd /s /q F:\PROJ\Q2DOS\RELEASE

F:
cd "F:\PROJ\Q2DOS"
