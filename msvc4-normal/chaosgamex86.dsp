# Microsoft Developer Studio Project File - Name="chaosgamex86" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=chaosgamex86 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "chaosgamex86.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "chaosgamex86.mak" CFG="chaosgamex86 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "chaosgamex86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "chaosgamex86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "chaosgamex86 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\chaos\Release"
# PROP Intermediate_Dir ".\chaos\Release"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\chaos\Release/gamex86.dll"

!ELSEIF  "$(CFG)" == "chaosgamex86 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\chaos\Debug"
# PROP Intermediate_Dir ".\chaos\Debug"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\chaos\Debug/gamex86.dll"

!ENDIF 

# Begin Target

# Name "chaosgamex86 - Win32 Release"
# Name "chaosgamex86 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\chaos\c_base.c
# End Source File
# Begin Source File

SOURCE=..\chaos\c_botai.c
# End Source File
# Begin Source File

SOURCE=..\chaos\c_botmisc.c
# End Source File
# Begin Source File

SOURCE=..\chaos\c_botnav.c
# End Source File
# Begin Source File

SOURCE=..\chaos\c_cam.c
# End Source File
# Begin Source File

SOURCE=..\chaos\c_item.c
# End Source File
# Begin Source File

SOURCE=..\chaos\c_weapon.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_cmds.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_combat.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_ctf.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_func.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_items.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_main.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_misc.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_phys.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_save.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_spawn.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_svcmds.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_target.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_trigger.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_utils.c
# End Source File
# Begin Source File

SOURCE=..\chaos\g_weapon.c
# End Source File
# Begin Source File

SOURCE=..\chaos\game.def
# End Source File
# Begin Source File

SOURCE=..\chaos\gslog.c
# End Source File
# Begin Source File

SOURCE=..\chaos\m_move.c
# End Source File
# Begin Source File

SOURCE=..\chaos\p_client.c
# End Source File
# Begin Source File

SOURCE=..\chaos\p_hud.c
# End Source File
# Begin Source File

SOURCE=..\chaos\p_menu.c
# End Source File
# Begin Source File

SOURCE=..\chaos\p_view.c
# End Source File
# Begin Source File

SOURCE=..\chaos\p_weapon.c
# End Source File
# Begin Source File

SOURCE=..\chaos\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\chaos\stdlog.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\chaos\c_base.h
# End Source File
# Begin Source File

SOURCE=..\chaos\c_botai.h
# End Source File
# Begin Source File

SOURCE=..\chaos\c_botnav.h
# End Source File
# Begin Source File

SOURCE=..\chaos\c_cam.h
# End Source File
# Begin Source File

SOURCE=..\chaos\c_item.h
# End Source File
# Begin Source File

SOURCE=..\chaos\c_weapon.h
# End Source File
# Begin Source File

SOURCE=..\chaos\classes.h
# End Source File
# Begin Source File

SOURCE=..\chaos\effects.h
# End Source File
# Begin Source File

SOURCE=..\chaos\g_ctf.h
# End Source File
# Begin Source File

SOURCE=..\chaos\g_local.h
# End Source File
# Begin Source File

SOURCE=..\chaos\game.h
# End Source File
# Begin Source File

SOURCE=..\chaos\gslog.h
# End Source File
# Begin Source File

SOURCE=..\chaos\m_player.h
# End Source File
# Begin Source File

SOURCE=..\chaos\p_menu.h
# End Source File
# Begin Source File

SOURCE=..\chaos\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\chaos\stdlog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
