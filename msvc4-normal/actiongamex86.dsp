# Microsoft Developer Studio Project File - Name="actiongamex86" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=actiongamex86 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "actiongamex86.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "actiongamex86.mak" CFG="actiongamex86 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "actiongamex86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "actiongamex86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "actiongamex86 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\action\Release"
# PROP Intermediate_Dir ".\action\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "id386=1" /D "NDEBUG" /D "_WINDOWS" /D "WITH_ACEBOT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\action\Release/gamex86.dll"

!ELSEIF  "$(CFG)" == "actiongamex86 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\action\Debug"
# PROP Intermediate_Dir ".\action\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WITH_ACEBOT" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\action\Debug/gamex86.dll"

!ENDIF 

# Begin Target

# Name "actiongamex86 - Win32 Release"
# Name "actiongamex86 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\action\a_cmds.c
# End Source File
# Begin Source File

SOURCE=..\action\a_doorkick.c
# End Source File
# Begin Source File

SOURCE=..\action\a_game.c
# End Source File
# Begin Source File

SOURCE=..\action\a_items.c
# End Source File
# Begin Source File

SOURCE=..\action\a_menu.c
# End Source File
# Begin Source File

SOURCE=..\action\a_radio.c
# End Source File
# Begin Source File

SOURCE=..\action\a_team.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_ai.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_cmds.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_compress.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_items.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_movement.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_nodes.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot_spawn.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\botchat.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\botnav.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\botscan.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\cgf_sfx_fog.c
# End Source File
# Begin Source File

SOURCE=..\action\cgf_sfx_glass.c
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\fogstub.c
# End Source File
# Begin Source File

SOURCE=..\action\g_ai.c
# End Source File
# Begin Source File

SOURCE=..\action\g_chase.c
# End Source File
# Begin Source File

SOURCE=..\action\g_cmds.c
# End Source File
# Begin Source File

SOURCE=..\action\g_combat.c
# End Source File
# Begin Source File

SOURCE=..\action\g_func.c
# End Source File
# Begin Source File

SOURCE=..\action\g_items.c
# End Source File
# Begin Source File

SOURCE=..\action\g_main.c
# End Source File
# Begin Source File

SOURCE=..\action\g_misc.c
# End Source File
# Begin Source File

SOURCE=..\action\g_monster.c
# End Source File
# Begin Source File

SOURCE=..\action\g_phys.c
# End Source File
# Begin Source File

SOURCE=..\action\g_save.c
# End Source File
# Begin Source File

SOURCE=..\action\g_spawn.c
# End Source File
# Begin Source File

SOURCE=..\action\g_svcmds.c
# End Source File
# Begin Source File

SOURCE=..\action\g_target.c
# End Source File
# Begin Source File

SOURCE=..\action\g_trigger.c
# End Source File
# Begin Source File

SOURCE=..\action\g_turret.c
# End Source File
# Begin Source File

SOURCE=..\action\g_utils.c
# End Source File
# Begin Source File

SOURCE=..\action\g_weapon.c
# End Source File
# Begin Source File

SOURCE=..\action\game.def
# End Source File
# Begin Source File

SOURCE=..\action\m_flash.c
# End Source File
# Begin Source File

SOURCE=..\action\m_move.c
# End Source File
# Begin Source File

SOURCE=..\action\m_tank.c
# End Source File
# Begin Source File

SOURCE=..\action\p_client.c
# End Source File
# Begin Source File

SOURCE=..\action\p_hud.c
# End Source File
# Begin Source File

SOURCE=..\action\p_trail.c
# End Source File
# Begin Source File

SOURCE=..\action\p_view.c
# End Source File
# Begin Source File

SOURCE=..\action\p_weapon.c
# End Source File
# Begin Source File

SOURCE=..\action\q_shared.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\action\a_game.h
# End Source File
# Begin Source File

SOURCE=..\action\a_menu.h
# End Source File
# Begin Source File

SOURCE=..\action\a_radio.h
# End Source File
# Begin Source File

SOURCE=..\action\a_team.h
# End Source File
# Begin Source File

SOURCE=..\action\acebot.h
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\acebot.h
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\botchat.h
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\botnav.h
# End Source File
# Begin Source File

SOURCE=..\action\botnav.h
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\botscan.h
# End Source File
# Begin Source File

SOURCE=..\action\acesrc\cgf_sfx_fog.h
# End Source File
# Begin Source File

SOURCE=..\action\cgf_sfx_glass.h
# End Source File
# Begin Source File

SOURCE=..\action\g_local.h
# End Source File
# Begin Source File

SOURCE=..\action\game.h
# End Source File
# Begin Source File

SOURCE=..\action\m_player.h
# End Source File
# Begin Source File

SOURCE=..\action\m_tank.h
# End Source File
# Begin Source File

SOURCE=..\action\q_shared.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
