@echo off
if not defined Q2DOSDIR ( goto failed )

SET SEVENZIPPATH=E:\Program Files\7-zip

Echo Building Distribution Package: Q2DOS for Win32
Echo.
Echo Q2DOS directory set %Q2DOSDIR%

ERRTIME -y > NUL

FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %%A0 SET ERR10=%%A
FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %ERR10%%%A SET ERR1=%%A
SET YEAR=20%ERR10%%ERR1%
REM Don't plan on living past 2099.  If someone reads this in 2100, update SET YEAR.

ERRTIME -m > NUL

FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %%A0 SET ERR10=%%A
FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %ERR10%%%A SET ERR1=%%A
SET MONTH=%ERR10%%ERR1%

ERRTIME -d > NUL

FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %%A0 SET ERR10=%%A
FOR %%A IN (0 1 2 3 4 5 6 7 8 9) DO IF ERRORLEVEL %ERR10%%%A SET ERR1=%%A
SET DAY=%ERR10%%ERR1%

del "%Q2DOSDIR%\q2dos_%YEAR%%MONTH%%DAY%_win32.7z"
del "%Q2DOSDIR%\q2dos_latest_win32.7z"

rd /s /q "%Q2DOSDIR%\RELEASE"
md "%Q2DOSDIR%\RELEASE"
md "%Q2DOSDIR%\RELEASE\baseq2"
md "%Q2DOSDIR%\RELEASE\3zb2"
md "%Q2DOSDIR%\RELEASE\ace"
md "%Q2DOSDIR%\RELEASE\action"
md "%Q2DOSDIR%\RELEASE\chaos"
md "%Q2DOSDIR%\RELEASE\coop"
md "%Q2DOSDIR%\RELEASE\ctf"
md "%Q2DOSDIR%\RELEASE\dday"
md "%Q2DOSDIR%\RELEASE\rogue"
md "%Q2DOSDIR%\RELEASE\xatrix"
md "%Q2DOSDIR%\RELEASE\zaero"

copy "%Q2DOSDIR%\gamespy.dll" "%Q2DOSDIR%\RELEASE"
copy "%Q2DOSDIR%\q2.exe" "%Q2DOSDIR%\RELEASE"
copy "%Q2DOSDIR%\ref_gl.dll" "%Q2DOSDIR%\RELEASE"
copy "%Q2DOSDIR%\ref_soft.dll" "%Q2DOSDIR%\RELEASE"

REM Game DLLs
copy "%Q2DOSDIR%\baseq2\gamex86.dll" "%Q2DOSDIR%\RELEASE\baseq2"
copy "%Q2DOSDIR%\3zb2\gamex86.dll" "%Q2DOSDIR%\RELEASE\3zb2"
copy "%Q2DOSDIR%\ace\gamex86.dll" "%Q2DOSDIR%\RELEASE\ace"
copy "%Q2DOSDIR%\action\gamex86.dll" "%Q2DOSDIR%\RELEASE\action"
copy "%Q2DOSDIR%\chaos\gamex86.dll" "%Q2DOSDIR%\RELEASE\chaos"
copy "%Q2DOSDIR%\coop\gamex86.dll" "%Q2DOSDIR%\RELEASE\coop"
copy "%Q2DOSDIR%\ctf\gamex86.dll" "%Q2DOSDIR%\RELEASE\ctf"
copy "%Q2DOSDIR%\dday\gamex86.dll" "%Q2DOSDIR%\RELEASE\dday"
copy "%Q2DOSDIR%\rogue\gamex86.dll" "%Q2DOSDIR%\RELEASE\rogue"
copy "%Q2DOSDIR%\xatrix\gamex86.dll" "%Q2DOSDIR%\RELEASE\xatrix"
copy "%Q2DOSDIR%\zaero\gamex86.dll" "%Q2DOSDIR%\RELEASE\zaero"

"%SEVENZIPPATH%\7z.exe" a -mx9 -mmt "%Q2DOSDIR%\q2dos_%YEAR%%MONTH%%DAY%_win32.7z" "%Q2DOSDIR%\RELEASE\*"

rd /s /q "%Q2DOSDIR%\RELEASE"

CALL upload_binary.bat "%Q2DOSDIR%\q2dos_%YEAR%%MONTH%%DAY%_win32.7z"

goto end

:failed
echo Q2DOSDIR not defined!
pause
goto end

:end
