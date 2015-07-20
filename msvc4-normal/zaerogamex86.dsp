# Microsoft Developer Studio Project File - Name="zaerogamex86" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=zaerogamex86 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zaerogamex86.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zaerogamex86.mak" CFG="zaerogamex86 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zaerogamex86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zaerogamex86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zaerogamex86 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Zaero\Release"
# PROP Intermediate_Dir ".\Zaero\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "id386" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\Zaero\Release/gamex86.dll"

!ELSEIF  "$(CFG)" == "zaerogamex86 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Zaero\Debug"
# PROP Intermediate_Dir ".\Zaero\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Zaero\Debug/gamex86.dll"

!ENDIF 

# Begin Target

# Name "zaerogamex86 - Win32 Release"
# Name "zaerogamex86 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\zaero\g_ai.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_chase.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_cmds.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_combat.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_func.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_items.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_main.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_misc.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_monster.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_phys.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_save.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_spawn.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_svcmds.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_target.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_trigger.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_turret.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_utils.c
# End Source File
# Begin Source File

SOURCE=..\zaero\g_weapon.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_actor.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_berserk.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss2.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss3.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss31.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss32.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_brain.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_chick.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_flash.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_flipper.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_float.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_flyer.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_gladiator.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_gunner.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_hover.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_infantry.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_insane.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_medic.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_move.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_mutant.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_parasite.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_soldier.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_supertank.c
# End Source File
# Begin Source File

SOURCE=..\zaero\m_tank.c
# End Source File
# Begin Source File

SOURCE=..\zaero\p_client.c
# End Source File
# Begin Source File

SOURCE=..\zaero\p_hud.c
# End Source File
# Begin Source File

SOURCE=..\zaero\p_trail.c
# End Source File
# Begin Source File

SOURCE=..\zaero\p_view.c
# End Source File
# Begin Source File

SOURCE=..\zaero\p_weapon.c
# End Source File
# Begin Source File

SOURCE=..\zaero\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_acannon.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_ai.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_anim.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_boss.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_camera.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_debug.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_frames.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_handler.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_hound.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_item.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_list.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_mtest.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_sentien.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_spawn.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_trigger.c
# End Source File
# Begin Source File

SOURCE=..\zaero\z_weapon.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\zaero\tables\clientfields.h
# End Source File
# Begin Source File

SOURCE=..\zaero\tables\fields.h
# End Source File
# Begin Source File

SOURCE=..\zaero\g_local.h
# End Source File
# Begin Source File

SOURCE=..\zaero\game.h
# End Source File
# Begin Source File

SOURCE=..\zaero\tables\gamefunc_decs.h
# End Source File
# Begin Source File

SOURCE=..\zaero\tables\gamefunc_list.h
# End Source File
# Begin Source File

SOURCE=..\zaero\tables\gamemmove_decs.h
# End Source File
# Begin Source File

SOURCE=..\zaero\tables\gamemmove_list.h
# End Source File
# Begin Source File

SOURCE=..\zaero\tables\levelfields.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_actor.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_berserk.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss2.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss31.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_boss32.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_brain.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_chick.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_flipper.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_float.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_flyer.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_gladiator.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_gunner.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_hover.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_infantry.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_insane.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_medic.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_mutant.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_parasite.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_player.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_rider.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_soldier.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_supertank.h
# End Source File
# Begin Source File

SOURCE=..\zaero\m_tank.h
# End Source File
# Begin Source File

SOURCE=..\zaero\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_anim.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_boss.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_debug.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_frames.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_handler.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_hound.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_list.h
# End Source File
# Begin Source File

SOURCE=..\zaero\z_sentien.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
