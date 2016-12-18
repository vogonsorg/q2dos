@echo off

if not defined Q2DOSDEVBASE ( goto failed )

taskkill /F /T /IM MSDEV.EXE 
taskkill /F /T /IM MSDEV.COM

call "%Q2DOSDEVBASE%\clean.bat"

cd /D "%Q2DOSDEVBASE%\msvc4-normal"

:startcopy
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "gamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "ref_soft - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "ref_gl - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "quake2 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "gamespysdk - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "3zb2gamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "acegamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "actiongamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "chaosgamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "coopgamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "ctfgamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "ddaygamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "roguegamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "xatrixgamex86 - Win32 Debug" /BUILD
msdev "%Q2DOSDEVBASE%\msvc4-normal\quake2.dsw" /MAKE "zaerogamex86 - Win32 Debug" /BUILD

CALL mvd.bat
goto end

:failed
echo Q2DOSDEVBASE not defined!
pause
goto end

:end
