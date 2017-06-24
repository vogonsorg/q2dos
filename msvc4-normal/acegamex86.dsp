# Microsoft Developer Studio Project File - Name="acegamex86" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=acegamex86 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "acegamex86.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "acegamex86.mak" CFG="acegamex86 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "acegamex86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "acegamex86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "acegamex86 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ace\Release"
# PROP Intermediate_Dir ".\ace\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D id386=1 /D "NDEBUG" /D "_WINDOWS" /D "WITH_ACEBOT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\ace\Release/gamex86.dll"

!ELSEIF  "$(CFG)" == "acegamex86 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\ace\Debug"
# PROP Intermediate_Dir ".\ace\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D id386=1 /D "_DEBUG" /D "_WINDOWS" /D "WITH_ACEBOT" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\ace\Debug/gamex86.dll"

!ENDIF 

# Begin Target

# Name "acegamex86 - Win32 Release"
# Name "acegamex86 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\acebot\acebot_ai.c
# End Source File
# Begin Source File

SOURCE=..\acebot\acebot_cmds.c
# End Source File
# Begin Source File

SOURCE=..\acebot\acebot_compress.c
# End Source File
# Begin Source File

SOURCE=..\acebot\acebot_items.c
# End Source File
# Begin Source File

SOURCE=..\acebot\acebot_movement.c
# End Source File
# Begin Source File

SOURCE=..\acebot\acebot_nodes.c
# End Source File
# Begin Source File

SOURCE=..\acebot\acebot_spawn.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_ai.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_chase.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_cmds.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_combat.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_ctf.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_func.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_items.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_main.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_misc.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_monster.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_phys.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_save.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_spawn.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_svcmds.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_target.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_trigger.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_turret.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_utils.c
# End Source File
# Begin Source File

SOURCE=..\acebot\g_weapon.c
# End Source File
# Begin Source File

SOURCE=..\acebot\game.def
# End Source File
# Begin Source File

SOURCE=..\acebot\m_actor.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_berserk.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss2.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss3.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss31.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss32.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_brain.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_chick.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_flash.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_flipper.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_float.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_flyer.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_gladiator.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_gunner.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_hover.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_infantry.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_insane.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_medic.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_move.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_mutant.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_parasite.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_soldier.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_supertank.c
# End Source File
# Begin Source File

SOURCE=..\acebot\m_tank.c
# End Source File
# Begin Source File

SOURCE=..\acebot\p_client.c
# End Source File
# Begin Source File

SOURCE=..\acebot\p_hud.c
# End Source File
# Begin Source File

SOURCE=..\acebot\p_menu.c
# End Source File
# Begin Source File

SOURCE=..\acebot\p_trail.c
# End Source File
# Begin Source File

SOURCE=..\acebot\p_view.c
# End Source File
# Begin Source File

SOURCE=..\acebot\p_weapon.c
# End Source File
# Begin Source File

SOURCE=..\acebot\q_shared.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\acebot\acebot.h
# End Source File
# Begin Source File

SOURCE=..\acebot\g_ctf.h
# End Source File
# Begin Source File

SOURCE=..\acebot\g_local.h
# End Source File
# Begin Source File

SOURCE=..\acebot\game.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_actor.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_berserk.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss2.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss31.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_boss32.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_brain.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_chick.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_flipper.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_float.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_flyer.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_gladiator.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_gunner.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_hover.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_infantry.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_insane.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_medic.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_mutant.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_parasite.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_player.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_rider.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_soldier.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_supertank.h
# End Source File
# Begin Source File

SOURCE=..\acebot\m_tank.h
# End Source File
# Begin Source File

SOURCE=..\acebot\p_menu.h
# End Source File
# Begin Source File

SOURCE=..\acebot\q_shared.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
