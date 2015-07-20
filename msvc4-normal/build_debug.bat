@ECHO OFF
taskkill /F /T /IM MSDEV.EXE 
taskkill /F /T /IM MSDEV.COM
call F:\PROJ\Q2DOS\CLEAN.BAT
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "quake2 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "gamespysdk - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "3zb2gamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "acegamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "actiongamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "chaosgamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "coopgamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "ctfgamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "ddaygamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "roguegamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "xatrixgamex86 - Win32 Debug" /BUILD
msdev F:\proj\q2dos\msvc4-normal\quake2.dsw /MAKE "zaerogamex86 - Win32 Debug" /BUILD
