# Microsoft Developer Studio Generated NMAKE File, Based on quake2-sv-rg.dsp
!IF "$(CFG)" == ""
CFG=quake2 - Win32 Release
!MESSAGE No configuration specified. Defaulting to quake2 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "quake2 - Win32 Release" && "$(CFG)" != "quake2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "quake2-sv-rg.mak" CFG="quake2 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "quake2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "quake2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "quake2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\quake2-sv-rg.exe"


CLEAN :
	-@erase "$(INTDIR)\cd_win.obj"
	-@erase "$(INTDIR)\cl_cin.obj"
	-@erase "$(INTDIR)\cl_ents.obj"
	-@erase "$(INTDIR)\cl_fx.obj"
	-@erase "$(INTDIR)\cl_http.obj"
	-@erase "$(INTDIR)\cl_input.obj"
	-@erase "$(INTDIR)\cl_inv.obj"
	-@erase "$(INTDIR)\cl_main.obj"
	-@erase "$(INTDIR)\cl_newfx.obj"
	-@erase "$(INTDIR)\cl_parse.obj"
	-@erase "$(INTDIR)\cl_pred.obj"
	-@erase "$(INTDIR)\cl_scrn.obj"
	-@erase "$(INTDIR)\cl_tent.obj"
	-@erase "$(INTDIR)\cl_view.obj"
	-@erase "$(INTDIR)\cmd.obj"
	-@erase "$(INTDIR)\cmodel.obj"
	-@erase "$(INTDIR)\common.obj"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\crc.obj"
	-@erase "$(INTDIR)\cvar.obj"
	-@erase "$(INTDIR)\darray.obj"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\g_ai.obj"
	-@erase "$(INTDIR)\g_chase.obj"
	-@erase "$(INTDIR)\g_cmds.obj"
	-@erase "$(INTDIR)\g_combat.obj"
	-@erase "$(INTDIR)\g_func.obj"
	-@erase "$(INTDIR)\g_items.obj"
	-@erase "$(INTDIR)\g_main.obj"
	-@erase "$(INTDIR)\g_misc.obj"
	-@erase "$(INTDIR)\g_monster.obj"
	-@erase "$(INTDIR)\g_phys.obj"
	-@erase "$(INTDIR)\g_save.obj"
	-@erase "$(INTDIR)\g_spawn.obj"
	-@erase "$(INTDIR)\g_svcmds.obj"
	-@erase "$(INTDIR)\g_target.obj"
	-@erase "$(INTDIR)\g_trigger.obj"
	-@erase "$(INTDIR)\g_turret.obj"
	-@erase "$(INTDIR)\g_utils.obj"
	-@erase "$(INTDIR)\g_weapon.obj"
	-@erase "$(INTDIR)\gl_draw.obj"
	-@erase "$(INTDIR)\gl_image.obj"
	-@erase "$(INTDIR)\gl_light.obj"
	-@erase "$(INTDIR)\gl_mesh.obj"
	-@erase "$(INTDIR)\gl_model.obj"
	-@erase "$(INTDIR)\gl_rmain.obj"
	-@erase "$(INTDIR)\gl_rmisc.obj"
	-@erase "$(INTDIR)\gl_rsurf.obj"
	-@erase "$(INTDIR)\gl_warp.obj"
	-@erase "$(INTDIR)\glw_imp.obj"
	-@erase "$(INTDIR)\gserver.obj"
	-@erase "$(INTDIR)\gserverlist.obj"
	-@erase "$(INTDIR)\gutil.obj"
	-@erase "$(INTDIR)\hashtable.obj"
	-@erase "$(INTDIR)\in_win.obj"
	-@erase "$(INTDIR)\keys.obj"
	-@erase "$(INTDIR)\m_actor.obj"
	-@erase "$(INTDIR)\m_berserk.obj"
	-@erase "$(INTDIR)\m_boss2.obj"
	-@erase "$(INTDIR)\m_boss3.obj"
	-@erase "$(INTDIR)\m_boss31.obj"
	-@erase "$(INTDIR)\m_boss32.obj"
	-@erase "$(INTDIR)\m_brain.obj"
	-@erase "$(INTDIR)\m_chick.obj"
	-@erase "$(INTDIR)\m_flash.obj"
	-@erase "$(INTDIR)\m_flipper.obj"
	-@erase "$(INTDIR)\m_float.obj"
	-@erase "$(INTDIR)\m_flyer.obj"
	-@erase "$(INTDIR)\m_gladiator.obj"
	-@erase "$(INTDIR)\m_gunner.obj"
	-@erase "$(INTDIR)\m_hover.obj"
	-@erase "$(INTDIR)\m_infantry.obj"
	-@erase "$(INTDIR)\m_insane.obj"
	-@erase "$(INTDIR)\m_medic.obj"
	-@erase "$(INTDIR)\m_move.obj"
	-@erase "$(INTDIR)\m_mutant.obj"
	-@erase "$(INTDIR)\m_parasite.obj"
	-@erase "$(INTDIR)\m_soldier.obj"
	-@erase "$(INTDIR)\m_supertank.obj"
	-@erase "$(INTDIR)\m_tank.obj"
	-@erase "$(INTDIR)\md4.obj"
	-@erase "$(INTDIR)\menu.obj"
	-@erase "$(INTDIR)\net_chan.obj"
	-@erase "$(INTDIR)\net_wins.obj"
	-@erase "$(INTDIR)\nonport.obj"
	-@erase "$(INTDIR)\p_client.obj"
	-@erase "$(INTDIR)\p_hud.obj"
	-@erase "$(INTDIR)\p_trail.obj"
	-@erase "$(INTDIR)\p_view.obj"
	-@erase "$(INTDIR)\p_weapon.obj"
	-@erase "$(INTDIR)\pmove.obj"
	-@erase "$(INTDIR)\q2.res"
	-@erase "$(INTDIR)\q_shared.obj"
	-@erase "$(INTDIR)\q_shwin.obj"
	-@erase "$(INTDIR)\qgl_win.obj"
	-@erase "$(INTDIR)\qmenu.obj"
	-@erase "$(INTDIR)\snd_dma.obj"
	-@erase "$(INTDIR)\snd_mem.obj"
	-@erase "$(INTDIR)\snd_mix.obj"
	-@erase "$(INTDIR)\snd_stream.obj"
	-@erase "$(INTDIR)\snd_wavstream.obj"
	-@erase "$(INTDIR)\snd_win.obj"
	-@erase "$(INTDIR)\sv_ccmds.obj"
	-@erase "$(INTDIR)\sv_ents.obj"
	-@erase "$(INTDIR)\sv_game.obj"
	-@erase "$(INTDIR)\sv_init.obj"
	-@erase "$(INTDIR)\sv_main.obj"
	-@erase "$(INTDIR)\sv_send.obj"
	-@erase "$(INTDIR)\sv_user.obj"
	-@erase "$(INTDIR)\sv_world.obj"
	-@erase "$(INTDIR)\sys_win.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vid_dll.obj"
	-@erase "$(INTDIR)\vid_menu.obj"
	-@erase "$(INTDIR)\x86.obj"
	-@erase "$(OUTDIR)\quake2-sv-rg.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "../win32/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D id386=1 /D "REF_HARD_LINKED" /D "GAME_HARD_LINKED" /D "CLIENT_SPLIT_NETFRAME" /D "GAMESPY" /D "GAMESPY_HARD_LINKED" /D "OGG_SUPPORT" /D "USE_CURL" /D "USE_JOYSTICK" /Fp"$(INTDIR)\quake2-sv-rg.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x1009 /fo"$(INTDIR)\q2.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\quake2-sv-rg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /pdb:none /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"$(OUTDIR)\quake2-sv-rg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\darray.obj" \
	"$(INTDIR)\gserver.obj" \
	"$(INTDIR)\gserverlist.obj" \
	"$(INTDIR)\gutil.obj" \
	"$(INTDIR)\hashtable.obj" \
	"$(INTDIR)\nonport.obj" \
	"$(INTDIR)\cl_cin.obj" \
	"$(INTDIR)\cl_ents.obj" \
	"$(INTDIR)\cl_fx.obj" \
	"$(INTDIR)\cl_http.obj" \
	"$(INTDIR)\cl_input.obj" \
	"$(INTDIR)\cl_inv.obj" \
	"$(INTDIR)\cl_main.obj" \
	"$(INTDIR)\cl_newfx.obj" \
	"$(INTDIR)\cl_parse.obj" \
	"$(INTDIR)\cl_pred.obj" \
	"$(INTDIR)\cl_scrn.obj" \
	"$(INTDIR)\cl_tent.obj" \
	"$(INTDIR)\cl_view.obj" \
	"$(INTDIR)\snd_dma.obj" \
	"$(INTDIR)\snd_mem.obj" \
	"$(INTDIR)\snd_mix.obj" \
	"$(INTDIR)\snd_stream.obj" \
	"$(INTDIR)\snd_wavstream.obj" \
	"$(INTDIR)\snd_win.obj" \
	"$(INTDIR)\sv_ccmds.obj" \
	"$(INTDIR)\sv_ents.obj" \
	"$(INTDIR)\sv_game.obj" \
	"$(INTDIR)\sv_init.obj" \
	"$(INTDIR)\sv_main.obj" \
	"$(INTDIR)\sv_send.obj" \
	"$(INTDIR)\sv_user.obj" \
	"$(INTDIR)\sv_world.obj" \
	"$(INTDIR)\cd_win.obj" \
	"$(INTDIR)\in_win.obj" \
	"$(INTDIR)\net_wins.obj" \
	"$(INTDIR)\q_shwin.obj" \
	"$(INTDIR)\sys_win.obj" \
	"$(INTDIR)\vid_dll.obj" \
	"$(INTDIR)\vid_menu.obj" \
	"$(INTDIR)\g_ai.obj" \
	"$(INTDIR)\g_chase.obj" \
	"$(INTDIR)\g_cmds.obj" \
	"$(INTDIR)\g_combat.obj" \
	"$(INTDIR)\g_func.obj" \
	"$(INTDIR)\g_items.obj" \
	"$(INTDIR)\g_main.obj" \
	"$(INTDIR)\g_misc.obj" \
	"$(INTDIR)\g_monster.obj" \
	"$(INTDIR)\g_phys.obj" \
	"$(INTDIR)\g_save.obj" \
	"$(INTDIR)\g_spawn.obj" \
	"$(INTDIR)\g_svcmds.obj" \
	"$(INTDIR)\g_target.obj" \
	"$(INTDIR)\g_trigger.obj" \
	"$(INTDIR)\g_turret.obj" \
	"$(INTDIR)\g_utils.obj" \
	"$(INTDIR)\g_weapon.obj" \
	"$(INTDIR)\m_actor.obj" \
	"$(INTDIR)\m_berserk.obj" \
	"$(INTDIR)\m_boss2.obj" \
	"$(INTDIR)\m_boss3.obj" \
	"$(INTDIR)\m_boss31.obj" \
	"$(INTDIR)\m_boss32.obj" \
	"$(INTDIR)\m_brain.obj" \
	"$(INTDIR)\m_chick.obj" \
	"$(INTDIR)\m_flash.obj" \
	"$(INTDIR)\m_flipper.obj" \
	"$(INTDIR)\m_float.obj" \
	"$(INTDIR)\m_flyer.obj" \
	"$(INTDIR)\m_gladiator.obj" \
	"$(INTDIR)\m_gunner.obj" \
	"$(INTDIR)\m_hover.obj" \
	"$(INTDIR)\m_infantry.obj" \
	"$(INTDIR)\m_insane.obj" \
	"$(INTDIR)\m_medic.obj" \
	"$(INTDIR)\m_move.obj" \
	"$(INTDIR)\m_mutant.obj" \
	"$(INTDIR)\m_parasite.obj" \
	"$(INTDIR)\m_soldier.obj" \
	"$(INTDIR)\m_supertank.obj" \
	"$(INTDIR)\m_tank.obj" \
	"$(INTDIR)\p_client.obj" \
	"$(INTDIR)\p_hud.obj" \
	"$(INTDIR)\p_trail.obj" \
	"$(INTDIR)\p_view.obj" \
	"$(INTDIR)\p_weapon.obj" \
	"$(INTDIR)\cmd.obj" \
	"$(INTDIR)\cmodel.obj" \
	"$(INTDIR)\common.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\crc.obj" \
	"$(INTDIR)\cvar.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\keys.obj" \
	"$(INTDIR)\md4.obj" \
	"$(INTDIR)\menu.obj" \
	"$(INTDIR)\net_chan.obj" \
	"$(INTDIR)\pmove.obj" \
	"$(INTDIR)\q_shared.obj" \
	"$(INTDIR)\qmenu.obj" \
	"$(INTDIR)\x86.obj" \
	"$(INTDIR)\q2.res" \
	"..\win32\libcurl.lib" \
	"..\win32\libogg.lib" \
	"..\win32\libvorbis.lib" \
	"$(INTDIR)\gl_draw.obj" \
	"$(INTDIR)\gl_image.obj" \
	"$(INTDIR)\gl_light.obj" \
	"$(INTDIR)\gl_mesh.obj" \
	"$(INTDIR)\gl_model.obj" \
	"$(INTDIR)\gl_rmain.obj" \
	"$(INTDIR)\gl_rmisc.obj" \
	"$(INTDIR)\gl_rsurf.obj" \
	"$(INTDIR)\gl_warp.obj" \
	"$(INTDIR)\qgl_win.obj" \
	"$(INTDIR)\glw_imp.obj"

"$(OUTDIR)\quake2-sv-rg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\quake2-sv-rg.exe" "$(OUTDIR)\quake2-sv-rg.bsc"


CLEAN :
	-@erase "$(INTDIR)\cd_win.obj"
	-@erase "$(INTDIR)\cd_win.sbr"
	-@erase "$(INTDIR)\cl_cin.obj"
	-@erase "$(INTDIR)\cl_cin.sbr"
	-@erase "$(INTDIR)\cl_ents.obj"
	-@erase "$(INTDIR)\cl_ents.sbr"
	-@erase "$(INTDIR)\cl_fx.obj"
	-@erase "$(INTDIR)\cl_fx.sbr"
	-@erase "$(INTDIR)\cl_http.obj"
	-@erase "$(INTDIR)\cl_http.sbr"
	-@erase "$(INTDIR)\cl_input.obj"
	-@erase "$(INTDIR)\cl_input.sbr"
	-@erase "$(INTDIR)\cl_inv.obj"
	-@erase "$(INTDIR)\cl_inv.sbr"
	-@erase "$(INTDIR)\cl_main.obj"
	-@erase "$(INTDIR)\cl_main.sbr"
	-@erase "$(INTDIR)\cl_newfx.obj"
	-@erase "$(INTDIR)\cl_newfx.sbr"
	-@erase "$(INTDIR)\cl_parse.obj"
	-@erase "$(INTDIR)\cl_parse.sbr"
	-@erase "$(INTDIR)\cl_pred.obj"
	-@erase "$(INTDIR)\cl_pred.sbr"
	-@erase "$(INTDIR)\cl_scrn.obj"
	-@erase "$(INTDIR)\cl_scrn.sbr"
	-@erase "$(INTDIR)\cl_tent.obj"
	-@erase "$(INTDIR)\cl_tent.sbr"
	-@erase "$(INTDIR)\cl_view.obj"
	-@erase "$(INTDIR)\cl_view.sbr"
	-@erase "$(INTDIR)\cmd.obj"
	-@erase "$(INTDIR)\cmd.sbr"
	-@erase "$(INTDIR)\cmodel.obj"
	-@erase "$(INTDIR)\cmodel.sbr"
	-@erase "$(INTDIR)\common.obj"
	-@erase "$(INTDIR)\common.sbr"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\console.sbr"
	-@erase "$(INTDIR)\crc.obj"
	-@erase "$(INTDIR)\crc.sbr"
	-@erase "$(INTDIR)\cvar.obj"
	-@erase "$(INTDIR)\cvar.sbr"
	-@erase "$(INTDIR)\darray.obj"
	-@erase "$(INTDIR)\darray.sbr"
	-@erase "$(INTDIR)\dxe.obj"
	-@erase "$(INTDIR)\dxe.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\g_ai.obj"
	-@erase "$(INTDIR)\g_ai.sbr"
	-@erase "$(INTDIR)\g_chase.obj"
	-@erase "$(INTDIR)\g_chase.sbr"
	-@erase "$(INTDIR)\g_cmds.obj"
	-@erase "$(INTDIR)\g_cmds.sbr"
	-@erase "$(INTDIR)\g_combat.obj"
	-@erase "$(INTDIR)\g_combat.sbr"
	-@erase "$(INTDIR)\g_func.obj"
	-@erase "$(INTDIR)\g_func.sbr"
	-@erase "$(INTDIR)\g_items.obj"
	-@erase "$(INTDIR)\g_items.sbr"
	-@erase "$(INTDIR)\g_main.obj"
	-@erase "$(INTDIR)\g_main.sbr"
	-@erase "$(INTDIR)\g_misc.obj"
	-@erase "$(INTDIR)\g_misc.sbr"
	-@erase "$(INTDIR)\g_monster.obj"
	-@erase "$(INTDIR)\g_monster.sbr"
	-@erase "$(INTDIR)\g_phys.obj"
	-@erase "$(INTDIR)\g_phys.sbr"
	-@erase "$(INTDIR)\g_save.obj"
	-@erase "$(INTDIR)\g_save.sbr"
	-@erase "$(INTDIR)\g_spawn.obj"
	-@erase "$(INTDIR)\g_spawn.sbr"
	-@erase "$(INTDIR)\g_svcmds.obj"
	-@erase "$(INTDIR)\g_svcmds.sbr"
	-@erase "$(INTDIR)\g_target.obj"
	-@erase "$(INTDIR)\g_target.sbr"
	-@erase "$(INTDIR)\g_trigger.obj"
	-@erase "$(INTDIR)\g_trigger.sbr"
	-@erase "$(INTDIR)\g_turret.obj"
	-@erase "$(INTDIR)\g_turret.sbr"
	-@erase "$(INTDIR)\g_utils.obj"
	-@erase "$(INTDIR)\g_utils.sbr"
	-@erase "$(INTDIR)\g_weapon.obj"
	-@erase "$(INTDIR)\g_weapon.sbr"
	-@erase "$(INTDIR)\gl_draw.obj"
	-@erase "$(INTDIR)\gl_draw.sbr"
	-@erase "$(INTDIR)\gl_image.obj"
	-@erase "$(INTDIR)\gl_image.sbr"
	-@erase "$(INTDIR)\gl_light.obj"
	-@erase "$(INTDIR)\gl_light.sbr"
	-@erase "$(INTDIR)\gl_mesh.obj"
	-@erase "$(INTDIR)\gl_mesh.sbr"
	-@erase "$(INTDIR)\gl_model.obj"
	-@erase "$(INTDIR)\gl_model.sbr"
	-@erase "$(INTDIR)\gl_rmain.obj"
	-@erase "$(INTDIR)\gl_rmain.sbr"
	-@erase "$(INTDIR)\gl_rmisc.obj"
	-@erase "$(INTDIR)\gl_rmisc.sbr"
	-@erase "$(INTDIR)\gl_rsurf.obj"
	-@erase "$(INTDIR)\gl_rsurf.sbr"
	-@erase "$(INTDIR)\gl_warp.obj"
	-@erase "$(INTDIR)\gl_warp.sbr"
	-@erase "$(INTDIR)\glw_imp.obj"
	-@erase "$(INTDIR)\glw_imp.sbr"
	-@erase "$(INTDIR)\gserver.obj"
	-@erase "$(INTDIR)\gserver.sbr"
	-@erase "$(INTDIR)\gserverlist.obj"
	-@erase "$(INTDIR)\gserverlist.sbr"
	-@erase "$(INTDIR)\gutil.obj"
	-@erase "$(INTDIR)\gutil.sbr"
	-@erase "$(INTDIR)\hashtable.obj"
	-@erase "$(INTDIR)\hashtable.sbr"
	-@erase "$(INTDIR)\in_win.obj"
	-@erase "$(INTDIR)\in_win.sbr"
	-@erase "$(INTDIR)\keys.obj"
	-@erase "$(INTDIR)\keys.sbr"
	-@erase "$(INTDIR)\m_actor.obj"
	-@erase "$(INTDIR)\m_actor.sbr"
	-@erase "$(INTDIR)\m_berserk.obj"
	-@erase "$(INTDIR)\m_berserk.sbr"
	-@erase "$(INTDIR)\m_boss2.obj"
	-@erase "$(INTDIR)\m_boss2.sbr"
	-@erase "$(INTDIR)\m_boss3.obj"
	-@erase "$(INTDIR)\m_boss3.sbr"
	-@erase "$(INTDIR)\m_boss31.obj"
	-@erase "$(INTDIR)\m_boss31.sbr"
	-@erase "$(INTDIR)\m_boss32.obj"
	-@erase "$(INTDIR)\m_boss32.sbr"
	-@erase "$(INTDIR)\m_brain.obj"
	-@erase "$(INTDIR)\m_brain.sbr"
	-@erase "$(INTDIR)\m_chick.obj"
	-@erase "$(INTDIR)\m_chick.sbr"
	-@erase "$(INTDIR)\m_flash.obj"
	-@erase "$(INTDIR)\m_flash.sbr"
	-@erase "$(INTDIR)\m_flipper.obj"
	-@erase "$(INTDIR)\m_flipper.sbr"
	-@erase "$(INTDIR)\m_float.obj"
	-@erase "$(INTDIR)\m_float.sbr"
	-@erase "$(INTDIR)\m_flyer.obj"
	-@erase "$(INTDIR)\m_flyer.sbr"
	-@erase "$(INTDIR)\m_gladiator.obj"
	-@erase "$(INTDIR)\m_gladiator.sbr"
	-@erase "$(INTDIR)\m_gunner.obj"
	-@erase "$(INTDIR)\m_gunner.sbr"
	-@erase "$(INTDIR)\m_hover.obj"
	-@erase "$(INTDIR)\m_hover.sbr"
	-@erase "$(INTDIR)\m_infantry.obj"
	-@erase "$(INTDIR)\m_infantry.sbr"
	-@erase "$(INTDIR)\m_insane.obj"
	-@erase "$(INTDIR)\m_insane.sbr"
	-@erase "$(INTDIR)\m_medic.obj"
	-@erase "$(INTDIR)\m_medic.sbr"
	-@erase "$(INTDIR)\m_move.obj"
	-@erase "$(INTDIR)\m_move.sbr"
	-@erase "$(INTDIR)\m_mutant.obj"
	-@erase "$(INTDIR)\m_mutant.sbr"
	-@erase "$(INTDIR)\m_parasite.obj"
	-@erase "$(INTDIR)\m_parasite.sbr"
	-@erase "$(INTDIR)\m_soldier.obj"
	-@erase "$(INTDIR)\m_soldier.sbr"
	-@erase "$(INTDIR)\m_supertank.obj"
	-@erase "$(INTDIR)\m_supertank.sbr"
	-@erase "$(INTDIR)\m_tank.obj"
	-@erase "$(INTDIR)\m_tank.sbr"
	-@erase "$(INTDIR)\md4.obj"
	-@erase "$(INTDIR)\md4.sbr"
	-@erase "$(INTDIR)\menu.obj"
	-@erase "$(INTDIR)\menu.sbr"
	-@erase "$(INTDIR)\net_chan.obj"
	-@erase "$(INTDIR)\net_chan.sbr"
	-@erase "$(INTDIR)\net_wins.obj"
	-@erase "$(INTDIR)\net_wins.sbr"
	-@erase "$(INTDIR)\nonport.obj"
	-@erase "$(INTDIR)\nonport.sbr"
	-@erase "$(INTDIR)\p_client.obj"
	-@erase "$(INTDIR)\p_client.sbr"
	-@erase "$(INTDIR)\p_hud.obj"
	-@erase "$(INTDIR)\p_hud.sbr"
	-@erase "$(INTDIR)\p_trail.obj"
	-@erase "$(INTDIR)\p_trail.sbr"
	-@erase "$(INTDIR)\p_view.obj"
	-@erase "$(INTDIR)\p_view.sbr"
	-@erase "$(INTDIR)\p_weapon.obj"
	-@erase "$(INTDIR)\p_weapon.sbr"
	-@erase "$(INTDIR)\pmove.obj"
	-@erase "$(INTDIR)\pmove.sbr"
	-@erase "$(INTDIR)\q2.res"
	-@erase "$(INTDIR)\q_shared.obj"
	-@erase "$(INTDIR)\q_shared.sbr"
	-@erase "$(INTDIR)\q_shwin.obj"
	-@erase "$(INTDIR)\q_shwin.sbr"
	-@erase "$(INTDIR)\qgl_win.obj"
	-@erase "$(INTDIR)\qgl_win.sbr"
	-@erase "$(INTDIR)\qmenu.obj"
	-@erase "$(INTDIR)\qmenu.sbr"
	-@erase "$(INTDIR)\r_aclip.obj"
	-@erase "$(INTDIR)\r_aclip.sbr"
	-@erase "$(INTDIR)\r_alias.obj"
	-@erase "$(INTDIR)\r_alias.sbr"
	-@erase "$(INTDIR)\r_bsp.obj"
	-@erase "$(INTDIR)\r_bsp.sbr"
	-@erase "$(INTDIR)\r_draw.obj"
	-@erase "$(INTDIR)\r_draw.sbr"
	-@erase "$(INTDIR)\r_edge.obj"
	-@erase "$(INTDIR)\r_edge.sbr"
	-@erase "$(INTDIR)\r_image.obj"
	-@erase "$(INTDIR)\r_image.sbr"
	-@erase "$(INTDIR)\r_light.obj"
	-@erase "$(INTDIR)\r_light.sbr"
	-@erase "$(INTDIR)\r_main.obj"
	-@erase "$(INTDIR)\r_main.sbr"
	-@erase "$(INTDIR)\r_misc.obj"
	-@erase "$(INTDIR)\r_misc.sbr"
	-@erase "$(INTDIR)\r_model.obj"
	-@erase "$(INTDIR)\r_model.sbr"
	-@erase "$(INTDIR)\r_part.obj"
	-@erase "$(INTDIR)\r_part.sbr"
	-@erase "$(INTDIR)\r_poly.obj"
	-@erase "$(INTDIR)\r_poly.sbr"
	-@erase "$(INTDIR)\r_polyse.obj"
	-@erase "$(INTDIR)\r_polyse.sbr"
	-@erase "$(INTDIR)\r_rast.obj"
	-@erase "$(INTDIR)\r_rast.sbr"
	-@erase "$(INTDIR)\r_scan.obj"
	-@erase "$(INTDIR)\r_scan.sbr"
	-@erase "$(INTDIR)\r_sprite.obj"
	-@erase "$(INTDIR)\r_sprite.sbr"
	-@erase "$(INTDIR)\r_surf.obj"
	-@erase "$(INTDIR)\r_surf.sbr"
	-@erase "$(INTDIR)\rw_ddraw.obj"
	-@erase "$(INTDIR)\rw_ddraw.sbr"
	-@erase "$(INTDIR)\rw_dib.obj"
	-@erase "$(INTDIR)\rw_dib.sbr"
	-@erase "$(INTDIR)\rw_imp.obj"
	-@erase "$(INTDIR)\rw_imp.sbr"
	-@erase "$(INTDIR)\snd_dma.obj"
	-@erase "$(INTDIR)\snd_dma.sbr"
	-@erase "$(INTDIR)\snd_mem.obj"
	-@erase "$(INTDIR)\snd_mem.sbr"
	-@erase "$(INTDIR)\snd_mix.obj"
	-@erase "$(INTDIR)\snd_mix.sbr"
	-@erase "$(INTDIR)\snd_stream.obj"
	-@erase "$(INTDIR)\snd_stream.sbr"
	-@erase "$(INTDIR)\snd_wavstream.obj"
	-@erase "$(INTDIR)\snd_wavstream.sbr"
	-@erase "$(INTDIR)\snd_win.obj"
	-@erase "$(INTDIR)\snd_win.sbr"
	-@erase "$(INTDIR)\sv_ccmds.obj"
	-@erase "$(INTDIR)\sv_ccmds.sbr"
	-@erase "$(INTDIR)\sv_ents.obj"
	-@erase "$(INTDIR)\sv_ents.sbr"
	-@erase "$(INTDIR)\sv_game.obj"
	-@erase "$(INTDIR)\sv_game.sbr"
	-@erase "$(INTDIR)\sv_init.obj"
	-@erase "$(INTDIR)\sv_init.sbr"
	-@erase "$(INTDIR)\sv_main.obj"
	-@erase "$(INTDIR)\sv_main.sbr"
	-@erase "$(INTDIR)\sv_send.obj"
	-@erase "$(INTDIR)\sv_send.sbr"
	-@erase "$(INTDIR)\sv_user.obj"
	-@erase "$(INTDIR)\sv_user.sbr"
	-@erase "$(INTDIR)\sv_world.obj"
	-@erase "$(INTDIR)\sv_world.sbr"
	-@erase "$(INTDIR)\sys_win.obj"
	-@erase "$(INTDIR)\sys_win.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vid_dll.obj"
	-@erase "$(INTDIR)\vid_dll.sbr"
	-@erase "$(INTDIR)\vid_menu.obj"
	-@erase "$(INTDIR)\vid_menu.sbr"
	-@erase "$(INTDIR)\x86.obj"
	-@erase "$(INTDIR)\x86.sbr"
	-@erase "$(OUTDIR)\quake2-sv-rg.bsc"
	-@erase "$(OUTDIR)\quake2-sv-rg.exe"
	-@erase "$(OUTDIR)\quake2-sv-rg.ilk"
	-@erase "$(OUTDIR)\quake2-sv-rg.map"
	-@erase "$(OUTDIR)\quake2-sv-rg.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "../win32/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D id386=1 /D "REF_HARD_LINKED" /D "GAME_HARD_LINKED" /D "CLIENT_SPLIT_NETFRAME" /D "GAMESPY" /D "GAMESPY_HARD_LINKED" /D "OGG_SUPPORT" /D "USE_CURL" /D "USE_JOYSTICK" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\quake2-sv-rg.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x1009 /fo"$(INTDIR)\q2.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\quake2-sv-rg.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\darray.sbr" \
	"$(INTDIR)\gserver.sbr" \
	"$(INTDIR)\gserverlist.sbr" \
	"$(INTDIR)\gutil.sbr" \
	"$(INTDIR)\hashtable.sbr" \
	"$(INTDIR)\nonport.sbr" \
	"$(INTDIR)\cl_cin.sbr" \
	"$(INTDIR)\cl_ents.sbr" \
	"$(INTDIR)\cl_fx.sbr" \
	"$(INTDIR)\cl_http.sbr" \
	"$(INTDIR)\cl_input.sbr" \
	"$(INTDIR)\cl_inv.sbr" \
	"$(INTDIR)\cl_main.sbr" \
	"$(INTDIR)\cl_newfx.sbr" \
	"$(INTDIR)\cl_parse.sbr" \
	"$(INTDIR)\cl_pred.sbr" \
	"$(INTDIR)\cl_scrn.sbr" \
	"$(INTDIR)\cl_tent.sbr" \
	"$(INTDIR)\cl_view.sbr" \
	"$(INTDIR)\snd_dma.sbr" \
	"$(INTDIR)\snd_mem.sbr" \
	"$(INTDIR)\snd_mix.sbr" \
	"$(INTDIR)\snd_stream.sbr" \
	"$(INTDIR)\snd_wavstream.sbr" \
	"$(INTDIR)\snd_win.sbr" \
	"$(INTDIR)\sv_ccmds.sbr" \
	"$(INTDIR)\sv_ents.sbr" \
	"$(INTDIR)\sv_game.sbr" \
	"$(INTDIR)\sv_init.sbr" \
	"$(INTDIR)\sv_main.sbr" \
	"$(INTDIR)\sv_send.sbr" \
	"$(INTDIR)\sv_user.sbr" \
	"$(INTDIR)\sv_world.sbr" \
	"$(INTDIR)\cd_win.sbr" \
	"$(INTDIR)\in_win.sbr" \
	"$(INTDIR)\net_wins.sbr" \
	"$(INTDIR)\q_shwin.sbr" \
	"$(INTDIR)\rw_ddraw.sbr" \
	"$(INTDIR)\rw_dib.sbr" \
	"$(INTDIR)\rw_imp.sbr" \
	"$(INTDIR)\sys_win.sbr" \
	"$(INTDIR)\vid_dll.sbr" \
	"$(INTDIR)\vid_menu.sbr" \
	"$(INTDIR)\g_ai.sbr" \
	"$(INTDIR)\g_chase.sbr" \
	"$(INTDIR)\g_cmds.sbr" \
	"$(INTDIR)\g_combat.sbr" \
	"$(INTDIR)\g_func.sbr" \
	"$(INTDIR)\g_items.sbr" \
	"$(INTDIR)\g_main.sbr" \
	"$(INTDIR)\g_misc.sbr" \
	"$(INTDIR)\g_monster.sbr" \
	"$(INTDIR)\g_phys.sbr" \
	"$(INTDIR)\g_save.sbr" \
	"$(INTDIR)\g_spawn.sbr" \
	"$(INTDIR)\g_svcmds.sbr" \
	"$(INTDIR)\g_target.sbr" \
	"$(INTDIR)\g_trigger.sbr" \
	"$(INTDIR)\g_turret.sbr" \
	"$(INTDIR)\g_utils.sbr" \
	"$(INTDIR)\g_weapon.sbr" \
	"$(INTDIR)\m_actor.sbr" \
	"$(INTDIR)\m_berserk.sbr" \
	"$(INTDIR)\m_boss2.sbr" \
	"$(INTDIR)\m_boss3.sbr" \
	"$(INTDIR)\m_boss31.sbr" \
	"$(INTDIR)\m_boss32.sbr" \
	"$(INTDIR)\m_brain.sbr" \
	"$(INTDIR)\m_chick.sbr" \
	"$(INTDIR)\m_flash.sbr" \
	"$(INTDIR)\m_flipper.sbr" \
	"$(INTDIR)\m_float.sbr" \
	"$(INTDIR)\m_flyer.sbr" \
	"$(INTDIR)\m_gladiator.sbr" \
	"$(INTDIR)\m_gunner.sbr" \
	"$(INTDIR)\m_hover.sbr" \
	"$(INTDIR)\m_infantry.sbr" \
	"$(INTDIR)\m_insane.sbr" \
	"$(INTDIR)\m_medic.sbr" \
	"$(INTDIR)\m_move.sbr" \
	"$(INTDIR)\m_mutant.sbr" \
	"$(INTDIR)\m_parasite.sbr" \
	"$(INTDIR)\m_soldier.sbr" \
	"$(INTDIR)\m_supertank.sbr" \
	"$(INTDIR)\m_tank.sbr" \
	"$(INTDIR)\p_client.sbr" \
	"$(INTDIR)\p_hud.sbr" \
	"$(INTDIR)\p_trail.sbr" \
	"$(INTDIR)\p_view.sbr" \
	"$(INTDIR)\p_weapon.sbr" \
	"$(INTDIR)\r_aclip.sbr" \
	"$(INTDIR)\r_alias.sbr" \
	"$(INTDIR)\r_bsp.sbr" \
	"$(INTDIR)\r_draw.sbr" \
	"$(INTDIR)\r_edge.sbr" \
	"$(INTDIR)\r_image.sbr" \
	"$(INTDIR)\r_light.sbr" \
	"$(INTDIR)\r_main.sbr" \
	"$(INTDIR)\r_misc.sbr" \
	"$(INTDIR)\r_model.sbr" \
	"$(INTDIR)\r_part.sbr" \
	"$(INTDIR)\r_poly.sbr" \
	"$(INTDIR)\r_polyse.sbr" \
	"$(INTDIR)\r_rast.sbr" \
	"$(INTDIR)\r_scan.sbr" \
	"$(INTDIR)\r_sprite.sbr" \
	"$(INTDIR)\r_surf.sbr" \
	"$(INTDIR)\dxe.sbr" \
	"$(INTDIR)\cmd.sbr" \
	"$(INTDIR)\cmodel.sbr" \
	"$(INTDIR)\common.sbr" \
	"$(INTDIR)\console.sbr" \
	"$(INTDIR)\crc.sbr" \
	"$(INTDIR)\cvar.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\keys.sbr" \
	"$(INTDIR)\md4.sbr" \
	"$(INTDIR)\menu.sbr" \
	"$(INTDIR)\net_chan.sbr" \
	"$(INTDIR)\pmove.sbr" \
	"$(INTDIR)\q_shared.sbr" \
	"$(INTDIR)\qmenu.sbr" \
	"$(INTDIR)\x86.sbr" \
	"$(INTDIR)\gl_draw.sbr" \
	"$(INTDIR)\gl_image.sbr" \
	"$(INTDIR)\gl_light.sbr" \
	"$(INTDIR)\gl_mesh.sbr" \
	"$(INTDIR)\gl_model.sbr" \
	"$(INTDIR)\gl_rmain.sbr" \
	"$(INTDIR)\gl_rmisc.sbr" \
	"$(INTDIR)\gl_rsurf.sbr" \
	"$(INTDIR)\gl_warp.sbr" \
	"$(INTDIR)\qgl_win.sbr" \
	"$(INTDIR)\glw_imp.sbr"

"$(OUTDIR)\quake2-sv-rg.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\quake2-sv-rg.pdb" /map:"$(INTDIR)\quake2-sv-rg.map" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"$(OUTDIR)\quake2-sv-rg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\darray.obj" \
	"$(INTDIR)\gserver.obj" \
	"$(INTDIR)\gserverlist.obj" \
	"$(INTDIR)\gutil.obj" \
	"$(INTDIR)\hashtable.obj" \
	"$(INTDIR)\nonport.obj" \
	"$(INTDIR)\cl_cin.obj" \
	"$(INTDIR)\cl_ents.obj" \
	"$(INTDIR)\cl_fx.obj" \
	"$(INTDIR)\cl_http.obj" \
	"$(INTDIR)\cl_input.obj" \
	"$(INTDIR)\cl_inv.obj" \
	"$(INTDIR)\cl_main.obj" \
	"$(INTDIR)\cl_newfx.obj" \
	"$(INTDIR)\cl_parse.obj" \
	"$(INTDIR)\cl_pred.obj" \
	"$(INTDIR)\cl_scrn.obj" \
	"$(INTDIR)\cl_tent.obj" \
	"$(INTDIR)\cl_view.obj" \
	"$(INTDIR)\snd_dma.obj" \
	"$(INTDIR)\snd_mem.obj" \
	"$(INTDIR)\snd_mix.obj" \
	"$(INTDIR)\snd_stream.obj" \
	"$(INTDIR)\snd_wavstream.obj" \
	"$(INTDIR)\snd_win.obj" \
	"$(INTDIR)\sv_ccmds.obj" \
	"$(INTDIR)\sv_ents.obj" \
	"$(INTDIR)\sv_game.obj" \
	"$(INTDIR)\sv_init.obj" \
	"$(INTDIR)\sv_main.obj" \
	"$(INTDIR)\sv_send.obj" \
	"$(INTDIR)\sv_user.obj" \
	"$(INTDIR)\sv_world.obj" \
	"$(INTDIR)\cd_win.obj" \
	"$(INTDIR)\in_win.obj" \
	"$(INTDIR)\net_wins.obj" \
	"$(INTDIR)\q_shwin.obj" \
	"$(INTDIR)\rw_ddraw.obj" \
	"$(INTDIR)\rw_dib.obj" \
	"$(INTDIR)\rw_imp.obj" \
	"$(INTDIR)\sys_win.obj" \
	"$(INTDIR)\vid_dll.obj" \
	"$(INTDIR)\vid_menu.obj" \
	"$(INTDIR)\g_ai.obj" \
	"$(INTDIR)\g_chase.obj" \
	"$(INTDIR)\g_cmds.obj" \
	"$(INTDIR)\g_combat.obj" \
	"$(INTDIR)\g_func.obj" \
	"$(INTDIR)\g_items.obj" \
	"$(INTDIR)\g_main.obj" \
	"$(INTDIR)\g_misc.obj" \
	"$(INTDIR)\g_monster.obj" \
	"$(INTDIR)\g_phys.obj" \
	"$(INTDIR)\g_save.obj" \
	"$(INTDIR)\g_spawn.obj" \
	"$(INTDIR)\g_svcmds.obj" \
	"$(INTDIR)\g_target.obj" \
	"$(INTDIR)\g_trigger.obj" \
	"$(INTDIR)\g_turret.obj" \
	"$(INTDIR)\g_utils.obj" \
	"$(INTDIR)\g_weapon.obj" \
	"$(INTDIR)\m_actor.obj" \
	"$(INTDIR)\m_berserk.obj" \
	"$(INTDIR)\m_boss2.obj" \
	"$(INTDIR)\m_boss3.obj" \
	"$(INTDIR)\m_boss31.obj" \
	"$(INTDIR)\m_boss32.obj" \
	"$(INTDIR)\m_brain.obj" \
	"$(INTDIR)\m_chick.obj" \
	"$(INTDIR)\m_flash.obj" \
	"$(INTDIR)\m_flipper.obj" \
	"$(INTDIR)\m_float.obj" \
	"$(INTDIR)\m_flyer.obj" \
	"$(INTDIR)\m_gladiator.obj" \
	"$(INTDIR)\m_gunner.obj" \
	"$(INTDIR)\m_hover.obj" \
	"$(INTDIR)\m_infantry.obj" \
	"$(INTDIR)\m_insane.obj" \
	"$(INTDIR)\m_medic.obj" \
	"$(INTDIR)\m_move.obj" \
	"$(INTDIR)\m_mutant.obj" \
	"$(INTDIR)\m_parasite.obj" \
	"$(INTDIR)\m_soldier.obj" \
	"$(INTDIR)\m_supertank.obj" \
	"$(INTDIR)\m_tank.obj" \
	"$(INTDIR)\p_client.obj" \
	"$(INTDIR)\p_hud.obj" \
	"$(INTDIR)\p_trail.obj" \
	"$(INTDIR)\p_view.obj" \
	"$(INTDIR)\p_weapon.obj" \
	"$(INTDIR)\r_aclip.obj" \
	"$(INTDIR)\r_alias.obj" \
	"$(INTDIR)\r_bsp.obj" \
	"$(INTDIR)\r_draw.obj" \
	"$(INTDIR)\r_edge.obj" \
	"$(INTDIR)\r_image.obj" \
	"$(INTDIR)\r_light.obj" \
	"$(INTDIR)\r_main.obj" \
	"$(INTDIR)\r_misc.obj" \
	"$(INTDIR)\r_model.obj" \
	"$(INTDIR)\r_part.obj" \
	"$(INTDIR)\r_poly.obj" \
	"$(INTDIR)\r_polyse.obj" \
	"$(INTDIR)\r_rast.obj" \
	"$(INTDIR)\r_scan.obj" \
	"$(INTDIR)\r_sprite.obj" \
	"$(INTDIR)\r_surf.obj" \
	"$(INTDIR)\dxe.obj" \
	"$(INTDIR)\cmd.obj" \
	"$(INTDIR)\cmodel.obj" \
	"$(INTDIR)\common.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\crc.obj" \
	"$(INTDIR)\cvar.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\keys.obj" \
	"$(INTDIR)\md4.obj" \
	"$(INTDIR)\menu.obj" \
	"$(INTDIR)\net_chan.obj" \
	"$(INTDIR)\pmove.obj" \
	"$(INTDIR)\q_shared.obj" \
	"$(INTDIR)\qmenu.obj" \
	"$(INTDIR)\x86.obj" \
	"$(INTDIR)\q2.res" \
	"..\win32\libcurl.lib" \
	"..\win32\libogg.lib" \
	"..\win32\libvorbis.lib" \
	"$(INTDIR)\r_aclipa.obj" \
	"$(INTDIR)\r_draw16.obj" \
	"$(INTDIR)\r_drawa.obj" \
	"$(INTDIR)\r_edgea.obj" \
	"$(INTDIR)\r_polysa.obj" \
	"$(INTDIR)\r_scana.obj" \
	"$(INTDIR)\r_spr8.obj" \
	"$(INTDIR)\r_surf8.obj" \
	"$(INTDIR)\r_varsa.obj" \
	"$(INTDIR)\gl_draw.obj" \
	"$(INTDIR)\gl_image.obj" \
	"$(INTDIR)\gl_light.obj" \
	"$(INTDIR)\gl_mesh.obj" \
	"$(INTDIR)\gl_model.obj" \
	"$(INTDIR)\gl_rmain.obj" \
	"$(INTDIR)\gl_rmisc.obj" \
	"$(INTDIR)\gl_rsurf.obj" \
	"$(INTDIR)\gl_warp.obj" \
	"$(INTDIR)\qgl_win.obj" \
	"$(INTDIR)\glw_imp.obj"

"$(OUTDIR)\quake2-sv-rg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("quake2-sv-rg.dep")
!INCLUDE "quake2-sv-rg.dep"
!ELSE 
!MESSAGE Warning: cannot find "quake2-sv-rg.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "quake2 - Win32 Release" || "$(CFG)" == "quake2 - Win32 Debug"
SOURCE=..\Goa\CEngine\darray.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\darray.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\darray.obj"	"$(INTDIR)\darray.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Goa\CEngine\gserver.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gserver.obj"	"$(INTDIR)\gserver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Goa\CEngine\gserverlist.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gserverlist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gserverlist.obj"	"$(INTDIR)\gserverlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Goa\CEngine\gutil.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gutil.obj"	"$(INTDIR)\gutil.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Goa\CEngine\hashtable.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\hashtable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\hashtable.obj"	"$(INTDIR)\hashtable.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Goa\nonport.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\nonport.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\nonport.obj"	"$(INTDIR)\nonport.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_cin.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_cin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_cin.obj"	"$(INTDIR)\cl_cin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_ents.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_ents.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_ents.obj"	"$(INTDIR)\cl_ents.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_fx.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_fx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_fx.obj"	"$(INTDIR)\cl_fx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\client\cl_http.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_http.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_http.obj"	"$(INTDIR)\cl_http.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_input.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_input.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_input.obj"	"$(INTDIR)\cl_input.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_inv.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_inv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_inv.obj"	"$(INTDIR)\cl_inv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_main.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_main.obj"	"$(INTDIR)\cl_main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_newfx.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_newfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_newfx.obj"	"$(INTDIR)\cl_newfx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_parse.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_parse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_parse.obj"	"$(INTDIR)\cl_parse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_pred.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_pred.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_pred.obj"	"$(INTDIR)\cl_pred.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_scrn.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_scrn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_scrn.obj"	"$(INTDIR)\cl_scrn.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_tent.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_tent.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_tent.obj"	"$(INTDIR)\cl_tent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\cl_view.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cl_view.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cl_view.obj"	"$(INTDIR)\cl_view.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\snd_dma.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\snd_dma.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\snd_dma.obj"	"$(INTDIR)\snd_dma.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\snd_mem.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\snd_mem.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\snd_mem.obj"	"$(INTDIR)\snd_mem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\snd_mix.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\snd_mix.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\snd_mix.obj"	"$(INTDIR)\snd_mix.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\client\snd_stream.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\snd_stream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\snd_stream.obj"	"$(INTDIR)\snd_stream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\client\snd_wavstream.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\snd_wavstream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\snd_wavstream.obj"	"$(INTDIR)\snd_wavstream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\snd_win.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\snd_win.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\snd_win.obj"	"$(INTDIR)\snd_win.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_ccmds.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_ccmds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_ccmds.obj"	"$(INTDIR)\sv_ccmds.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_ents.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_ents.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_ents.obj"	"$(INTDIR)\sv_ents.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_game.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_game.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_game.obj"	"$(INTDIR)\sv_game.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_init.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_init.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_init.obj"	"$(INTDIR)\sv_init.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_main.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_main.obj"	"$(INTDIR)\sv_main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_send.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_send.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_send.obj"	"$(INTDIR)\sv_send.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_user.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_user.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_user.obj"	"$(INTDIR)\sv_user.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\SERVER\sv_world.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sv_world.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sv_world.obj"	"$(INTDIR)\sv_world.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\cd_win.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cd_win.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cd_win.obj"	"$(INTDIR)\cd_win.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\win32\glw_imp.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\glw_imp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\glw_imp.obj"	"$(INTDIR)\glw_imp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\in_win.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\in_win.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\in_win.obj"	"$(INTDIR)\in_win.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\win32\net_wins.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\net_wins.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\net_wins.obj"	"$(INTDIR)\net_wins.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\q_shwin.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\q_shwin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\q_shwin.obj"	"$(INTDIR)\q_shwin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\win32\qgl_win.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\qgl_win.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\qgl_win.obj"	"$(INTDIR)\qgl_win.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\rw_ddraw.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\rw_ddraw.obj"	"$(INTDIR)\rw_ddraw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\rw_dib.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\rw_dib.obj"	"$(INTDIR)\rw_dib.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\rw_imp.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\rw_imp.obj"	"$(INTDIR)\rw_imp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\sys_win.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\sys_win.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\sys_win.obj"	"$(INTDIR)\sys_win.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\vid_dll.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\vid_dll.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\vid_dll.obj"	"$(INTDIR)\vid_dll.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\WIN32\vid_menu.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\vid_menu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\vid_menu.obj"	"$(INTDIR)\vid_menu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_ai.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_ai.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_ai.obj"	"$(INTDIR)\g_ai.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_chase.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_chase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_chase.obj"	"$(INTDIR)\g_chase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_cmds.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_cmds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_cmds.obj"	"$(INTDIR)\g_cmds.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_combat.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_combat.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_combat.obj"	"$(INTDIR)\g_combat.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_func.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_func.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_func.obj"	"$(INTDIR)\g_func.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_items.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_items.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_items.obj"	"$(INTDIR)\g_items.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_main.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_main.obj"	"$(INTDIR)\g_main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_misc.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_misc.obj"	"$(INTDIR)\g_misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_monster.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_monster.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_monster.obj"	"$(INTDIR)\g_monster.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_phys.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_phys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_phys.obj"	"$(INTDIR)\g_phys.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_save.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_save.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_save.obj"	"$(INTDIR)\g_save.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_spawn.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_spawn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_spawn.obj"	"$(INTDIR)\g_spawn.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_svcmds.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_svcmds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_svcmds.obj"	"$(INTDIR)\g_svcmds.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_target.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_target.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_target.obj"	"$(INTDIR)\g_target.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_trigger.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_trigger.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_trigger.obj"	"$(INTDIR)\g_trigger.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_turret.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_turret.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_turret.obj"	"$(INTDIR)\g_turret.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_utils.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_utils.obj"	"$(INTDIR)\g_utils.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\g_weapon.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\g_weapon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\g_weapon.obj"	"$(INTDIR)\g_weapon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_actor.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_actor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_actor.obj"	"$(INTDIR)\m_actor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_berserk.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_berserk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_berserk.obj"	"$(INTDIR)\m_berserk.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_boss2.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_boss2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_boss2.obj"	"$(INTDIR)\m_boss2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_boss3.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_boss3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_boss3.obj"	"$(INTDIR)\m_boss3.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_boss31.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_boss31.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_boss31.obj"	"$(INTDIR)\m_boss31.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_boss32.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_boss32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_boss32.obj"	"$(INTDIR)\m_boss32.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_brain.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_brain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_brain.obj"	"$(INTDIR)\m_brain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_chick.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_chick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_chick.obj"	"$(INTDIR)\m_chick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_flash.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_flash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_flash.obj"	"$(INTDIR)\m_flash.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_flipper.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_flipper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_flipper.obj"	"$(INTDIR)\m_flipper.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_float.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_float.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_float.obj"	"$(INTDIR)\m_float.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_flyer.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_flyer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_flyer.obj"	"$(INTDIR)\m_flyer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_gladiator.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_gladiator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_gladiator.obj"	"$(INTDIR)\m_gladiator.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_gunner.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_gunner.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_gunner.obj"	"$(INTDIR)\m_gunner.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_hover.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_hover.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_hover.obj"	"$(INTDIR)\m_hover.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_infantry.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_infantry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_infantry.obj"	"$(INTDIR)\m_infantry.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_insane.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_insane.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_insane.obj"	"$(INTDIR)\m_insane.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_medic.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_medic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_medic.obj"	"$(INTDIR)\m_medic.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_move.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_move.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_move.obj"	"$(INTDIR)\m_move.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_mutant.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_mutant.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_mutant.obj"	"$(INTDIR)\m_mutant.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_parasite.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_parasite.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_parasite.obj"	"$(INTDIR)\m_parasite.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_soldier.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_soldier.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_soldier.obj"	"$(INTDIR)\m_soldier.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_supertank.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_supertank.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_supertank.obj"	"$(INTDIR)\m_supertank.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\m_tank.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\m_tank.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\m_tank.obj"	"$(INTDIR)\m_tank.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\p_client.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\p_client.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\p_client.obj"	"$(INTDIR)\p_client.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\p_hud.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\p_hud.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\p_hud.obj"	"$(INTDIR)\p_hud.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\p_trail.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\p_trail.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\p_trail.obj"	"$(INTDIR)\p_trail.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\p_view.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\p_view.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\p_view.obj"	"$(INTDIR)\p_view.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\GAME\p_weapon.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\p_weapon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\p_weapon.obj"	"$(INTDIR)\p_weapon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_aclip.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_aclip.obj"	"$(INTDIR)\r_aclip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_soft\r_aclipa.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_aclipa.asm
InputName=r_aclipa

"$(INTDIR)\r_aclipa.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\REF_SOFT\r_alias.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_alias.obj"	"$(INTDIR)\r_alias.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_bsp.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_bsp.obj"	"$(INTDIR)\r_bsp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_draw.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_draw.obj"	"$(INTDIR)\r_draw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_soft\r_draw16.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_draw16.asm
InputName=r_draw16

"$(INTDIR)\r_draw16.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\ref_soft\r_drawa.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_drawa.asm
InputName=r_drawa

"$(INTDIR)\r_drawa.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\REF_SOFT\r_edge.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_edge.obj"	"$(INTDIR)\r_edge.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_soft\r_edgea.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_edgea.asm
InputName=r_edgea

"$(INTDIR)\r_edgea.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\REF_SOFT\r_image.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_image.obj"	"$(INTDIR)\r_image.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_light.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_light.obj"	"$(INTDIR)\r_light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_main.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_main.obj"	"$(INTDIR)\r_main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_misc.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_misc.obj"	"$(INTDIR)\r_misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_model.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_model.obj"	"$(INTDIR)\r_model.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_part.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_part.obj"	"$(INTDIR)\r_part.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_poly.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_poly.obj"	"$(INTDIR)\r_poly.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_soft\r_polysa.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_polysa.asm
InputName=r_polysa

"$(INTDIR)\r_polysa.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\REF_SOFT\r_polyse.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_polyse.obj"	"$(INTDIR)\r_polyse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_rast.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_rast.obj"	"$(INTDIR)\r_rast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_scan.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_scan.obj"	"$(INTDIR)\r_scan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_soft\r_scana.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_scana.asm
InputName=r_scana

"$(INTDIR)\r_scana.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\ref_soft\r_spr8.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_spr8.asm
InputName=r_spr8

"$(INTDIR)\r_spr8.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\REF_SOFT\r_sprite.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_sprite.obj"	"$(INTDIR)\r_sprite.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\REF_SOFT\r_surf.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\r_surf.obj"	"$(INTDIR)\r_surf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_soft\r_surf8.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_surf8.asm
InputName=r_surf8

"$(INTDIR)\r_surf8.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\ref_soft\r_varsa.asm

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"

OutDir=.\Debug
InputPath=..\ref_soft\r_varsa.asm
InputName=r_varsa

"$(INTDIR)\r_varsa.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi $(InputPath)
<< 
	

!ENDIF 

SOURCE=..\dos\cd_audio.c
SOURCE=..\dos\cd_dos.c
SOURCE=..\dos\dos_v2.c
SOURCE=..\dos\dxe.c

!IF  "$(CFG)" == "quake2 - Win32 Release"

!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\dxe.obj"	"$(INTDIR)\dxe.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\dos\glob.c
SOURCE=..\dos\in_dos.c
SOURCE=..\dos\q_shdos.c
SOURCE=..\dos\snd_gus.c
SOURCE=..\dos\snddma_dos.c
SOURCE=..\dos\swimp_dos.c
SOURCE=..\dos\sys_dos.c
SOURCE=..\dos\sys_dosq1.c
SOURCE=..\dos\vid_dos.c
SOURCE=..\dos\vid_ext.c
SOURCE=..\ref_gl\gl_draw.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_draw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_draw.obj"	"$(INTDIR)\gl_draw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_image.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_image.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_image.obj"	"$(INTDIR)\gl_image.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_light.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_light.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_light.obj"	"$(INTDIR)\gl_light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_mesh.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_mesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_mesh.obj"	"$(INTDIR)\gl_mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_model.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_model.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_model.obj"	"$(INTDIR)\gl_model.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_rmain.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_rmain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_rmain.obj"	"$(INTDIR)\gl_rmain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_rmisc.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_rmisc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_rmisc.obj"	"$(INTDIR)\gl_rmisc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_rsurf.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_rsurf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_rsurf.obj"	"$(INTDIR)\gl_rsurf.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ref_gl\gl_warp.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\gl_warp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\gl_warp.obj"	"$(INTDIR)\gl_warp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\cmd.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cmd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cmd.obj"	"$(INTDIR)\cmd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\cmodel.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cmodel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cmodel.obj"	"$(INTDIR)\cmodel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\common.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\common.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\common.obj"	"$(INTDIR)\common.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\console.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\console.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\console.obj"	"$(INTDIR)\console.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\crc.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\crc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\crc.obj"	"$(INTDIR)\crc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\cvar.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\cvar.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\cvar.obj"	"$(INTDIR)\cvar.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\files.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\files.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\files.obj"	"$(INTDIR)\files.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\keys.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\keys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\keys.obj"	"$(INTDIR)\keys.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\md4.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\md4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\md4.obj"	"$(INTDIR)\md4.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\menu.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\menu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\menu.obj"	"$(INTDIR)\menu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\net_chan.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\net_chan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\net_chan.obj"	"$(INTDIR)\net_chan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\QCOMMON\pmove.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\pmove.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\pmove.obj"	"$(INTDIR)\pmove.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\win32\q2.rc

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\q2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\q2.res" /i "\PROJ\q2dos\win32" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\q2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\q2.res" /i "\PROJ\q2dos\win32" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\GAME\q_shared.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\q_shared.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\q_shared.obj"	"$(INTDIR)\q_shared.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\qmenu.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\qmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\qmenu.obj"	"$(INTDIR)\qmenu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\CLIENT\x86.c

!IF  "$(CFG)" == "quake2 - Win32 Release"


"$(INTDIR)\x86.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "quake2 - Win32 Debug"


"$(INTDIR)\x86.obj"	"$(INTDIR)\x86.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

