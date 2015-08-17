# Microsoft Developer Studio Project File - Name="ref_sqb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ref_sqb - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ref_sqb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ref_sqb.mak" CFG="ref_sqb - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ref_sqb - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ref_sqb - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ref_sqb"
# PROP BASE Intermediate_Dir ".\ref_sqb"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D id386=1 /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\ref_sqb"
# PROP BASE Intermediate_Dir ".\ref_sqb"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib /nologo /subsystem:windows /dll /incremental:no /map /debug /machine:I386 /nodefaultlib:"libc"
# SUBTRACT LINK32 /profile /nodefaultlib

!ENDIF 

# Begin Target

# Name "ref_sqb - Win32 Release"
# Name "ref_sqb - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ref_soft_qb\r_draw16.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_draw16.asm
InputName=r_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_drawa.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_drawa.asm
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_edgea.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_edgea.asm
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_polysa.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_polysa.asm
InputName=r_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_spr8.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_spr8.asm
InputName=r_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_surf8.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_surf8.asm
InputName=r_surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_varsa.asm

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\ref_soft_qb\r_varsa.asm
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\game\q_shared.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\win32\q_shwin.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_aclip.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_alias.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_bsp.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_draw.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_edge.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_image.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_light.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_main.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_misc.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_model.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_part.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_poly.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_polyse.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_rast.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_scan.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_sprite.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_surf.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\ref_sqb.def
# End Source File
# Begin Source File

SOURCE=..\win32\rw_ddraw.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\win32\rw_dib.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\win32\rw_imp.c

!IF  "$(CFG)" == "ref_sqb - Win32 Release"

!ELSEIF  "$(CFG)" == "ref_sqb - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\ref_soft_qb\adivtab.h
# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\anorms.h
# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\asm_draw.h
# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_dither.h
# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_local.h
# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\r_model.h
# End Source File
# Begin Source File

SOURCE=..\ref_soft_qb\rand1k.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
