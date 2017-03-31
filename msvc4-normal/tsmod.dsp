# Microsoft Developer Studio Project File - Name="tsmodgamex86" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tsmodgamex86 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tsmod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tsmod.mak" CFG="tsmodgamex86 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tsmodgamex86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tsmodgamex86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tsmodgamex86 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\tsmod\Release"
# PROP Intermediate_Dir ".\tsmod\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D id386=1 /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\tsmod\Release/gamex86.dll"

!ELSEIF  "$(CFG)" == "tsmodgamex86 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\tsmod\Debug"
# PROP Intermediate_Dir ".\tsmod\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D id386=1 /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\tsmod\Debug/gamex86.dll"

!ENDIF 

# Begin Target

# Name "tsmodgamex86 - Win32 Release"
# Name "tsmodgamex86 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\coop\tsmod\g_main.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\game.def
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\md4.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\regex.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_ban.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_checkvar.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_clib.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_cmd.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_disable.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_flood.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_init.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_log.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_lrcon.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_msgqueue.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_spawn.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_util.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_vote.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_zbot.c
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\zb_zbotcheck.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\coop\tsmod\g_local.h
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\game.h
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\coop\tsmod\regex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
