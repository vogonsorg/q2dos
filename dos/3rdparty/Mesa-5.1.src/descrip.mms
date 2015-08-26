# Makefile for Mesa for VMS
# contributed by Jouk Jansen  joukj@hrem.stm.tudelft.nl

macro : 
        @ macro=""
.ifdef NOSHARE
.else
	@ if f$getsyi("HW_MODEL") .ge. 1024 then macro= "/MACRO=(SHARE=1)"
.endif
	$(MMS)$(MMSQUALIFIERS)'macro' all

all :
	if f$search("lib.dir") .eqs. "" then create/directory [.lib]
	set default [.src]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.progs.demos]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.xdemos]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.tests]
	$(MMS)$(MMSQUALIFIERS)
