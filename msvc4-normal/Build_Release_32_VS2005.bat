@echo off
set Q2DOSBINDIR=Release

if not defined Q2DOSDEVBASE ( goto failed )

call "%Q2DOSDEVBASE%\clean.bat"
call vcvars32.bat
call "%VS80COMNTOOLS%vsvars32.bat"

cd /D "%Q2DOSDEVBASE%\msvc4-normal"

:startcopy
devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "q2ded"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "gamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "ref_soft"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "ref_gl"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "quake2"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "gamespysdk"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "3zb2gamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "acegamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "actiongamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "chaosgamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "coopgamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "ctfgamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "ddaygamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "roguegamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "xatrixgamex86"

devenv "%Q2DOSDEVBASE%\msvc4-normal\quake2.sln" /Build "%Q2DOSBINDIR%" /Project "zaerogamex86"

CALL mvf.bat
goto end

:failed
echo Q2DOSDEVBASE not defined!
pause
goto end

:end
set Q2DOSBINDIR=
