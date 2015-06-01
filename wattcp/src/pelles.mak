#
# NB! THIS MAKEFILE WAS AUTOMATICALLY GENERATED FROM MAKEFILE.ALL.
#     DO NOT EDIT.
#
# Makefile for Waterloo TCP/IP kernel
#

ASM_SOURCE = asmpkt.asm chksum0.asm cpumodel.asm

CORE_SOURCE = bsdname.c  btree.c    chksum.c   country.c  crc.c      \
              echo.c     fortify.c  getopt.c   gettod.c   highc.c    \
              ip4_frag.c ip4_in.c   ip4_out.c  ip6_in.c   ip6_out.c  \
              language.c lookup.c   loopback.c misc.c     netback.c  \
              oldstuff.c pc_cbrk.c  pcarp.c    pcbootp.c  powerpak.c \
              pcbuf.c    pcconfig.c pcdbug.c   pcdhcp.c   pcicmp.c   \
              pcicmp6.c  pcintr.c   pcmulti.c  pcping.c   pcpkt.c    \
              pcpkt32.c  pcqueue.c  pcrarp.c   pcrecv.c   pcsed.c    \
              pcstat.c   pctcp.c    ports.c    ppp.c      pppoe.c    \
              qmsg.c     rs232.c    settod.c   sock_dbu.c sock_in.c  \
              sock_ini.c sock_io.c  sock_prn.c sock_scn.c sock_sel.c \
              split.c    strings.c  tcp_fsm.c  tftp.c     timer.c    \
              udp_dom.c  udp_rev.c  version.c  wdpmi.c    x32vm.c    \
              pcsarp.c   idna.c     punycode.c tcp_md5.c  dynip.c    \
              winpcap.c  winmisc.c  packet32.c

BSD_SOURCE = accept.c   adr2asc.c  asc2adr.c  bind.c     bsddbug.c  \
             close.c    connect.c  fcntl.c    fsext.c    get_ai.c   \
             get_ni.c   get_ip.c   geteth.c   gethost.c  gethost6.c \
             getname.c  getnet.c   getprot.c  getput.c   getserv.c  \
             get_xbyr.c ioctl.c    linkaddr.c listen.c   netaddr.c  \
             neterr.c   nettime.c  nsapaddr.c poll.c     presaddr.c \
             printk.c   receive.c  select.c   shutdown.c signal.c   \
             socket.c   sockopt.c  stream.c   syslog.c   syslog2.c  \
             transmit.c

BIND_SOURCE = res_comp.c res_data.c res_debu.c res_init.c res_loc.c \
              res_mkqu.c res_quer.c res_send.c

C_SOURCE = $(CORE_SOURCE) $(BSD_SOURCE) $(BIND_SOURCE)


OBJS = \
       $(OBJDIR)\chksum0.obj  $(OBJDIR)\cpumodel.obj  \
       $(OBJDIR)\accept.obj   $(OBJDIR)\adr2asc.obj   \
       $(OBJDIR)\asc2adr.obj  $(OBJDIR)\bind.obj      \
       $(OBJDIR)\bsddbug.obj  $(OBJDIR)\bsdname.obj   \
       $(OBJDIR)\btree.obj    $(OBJDIR)\chksum.obj    \
       $(OBJDIR)\close.obj    $(OBJDIR)\connect.obj   \
       $(OBJDIR)\country.obj  $(OBJDIR)\crc.obj       \
       $(OBJDIR)\echo.obj     $(OBJDIR)\fcntl.obj     \
       $(OBJDIR)\fortify.obj  $(OBJDIR)\get_ai.obj    \
       $(OBJDIR)\get_ni.obj   $(OBJDIR)\geteth.obj    \
       $(OBJDIR)\gethost.obj  $(OBJDIR)\gethost6.obj  \
       $(OBJDIR)\getname.obj  $(OBJDIR)\getnet.obj    \
       $(OBJDIR)\getopt.obj   $(OBJDIR)\getprot.obj   \
       $(OBJDIR)\getput.obj   $(OBJDIR)\getserv.obj   \
       $(OBJDIR)\gettod.obj   $(OBJDIR)\ioctl.obj     \
       $(OBJDIR)\ip4_frag.obj $(OBJDIR)\ip4_in.obj    \
       $(OBJDIR)\ip4_out.obj  $(OBJDIR)\ip6_in.obj    \
       $(OBJDIR)\ip6_out.obj  $(OBJDIR)\language.obj  \
       $(OBJDIR)\linkaddr.obj $(OBJDIR)\listen.obj    \
       $(OBJDIR)\lookup.obj   $(OBJDIR)\loopback.obj  \
       $(OBJDIR)\misc.obj     $(OBJDIR)\netaddr.obj   \
       $(OBJDIR)\netback.obj  $(OBJDIR)\neterr.obj    \
       $(OBJDIR)\nettime.obj  $(OBJDIR)\nsapaddr.obj  \
       $(OBJDIR)\oldstuff.obj $(OBJDIR)\pc_cbrk.obj   \
       $(OBJDIR)\pcarp.obj    $(OBJDIR)\pcbootp.obj   \
       $(OBJDIR)\powerpak.obj $(OBJDIR)\pcbuf.obj     \
       $(OBJDIR)\pcconfig.obj $(OBJDIR)\pcdbug.obj    \
       $(OBJDIR)\pcdhcp.obj   $(OBJDIR)\pcicmp.obj    \
       $(OBJDIR)\pcicmp6.obj  $(OBJDIR)\pcintr.obj    \
       $(OBJDIR)\pcmulti.obj  $(OBJDIR)\pcping.obj    \
       $(OBJDIR)\pcpkt.obj    $(OBJDIR)\pcpkt32.obj   \
       $(OBJDIR)\pcqueue.obj  $(OBJDIR)\pcrarp.obj    \
       $(OBJDIR)\pcrecv.obj   $(OBJDIR)\pcsed.obj     \
       $(OBJDIR)\pcstat.obj   $(OBJDIR)\pctcp.obj     \
       $(OBJDIR)\poll.obj     $(OBJDIR)\ports.obj     \
       $(OBJDIR)\ppp.obj      $(OBJDIR)\pppoe.obj     \
       $(OBJDIR)\presaddr.obj $(OBJDIR)\printk.obj    \
       $(OBJDIR)\qmsg.obj     $(OBJDIR)\receive.obj   \
       $(OBJDIR)\res_comp.obj $(OBJDIR)\res_data.obj  \
       $(OBJDIR)\res_debu.obj $(OBJDIR)\res_init.obj  \
       $(OBJDIR)\res_loc.obj  $(OBJDIR)\res_mkqu.obj  \
       $(OBJDIR)\res_quer.obj $(OBJDIR)\res_send.obj  \
       $(OBJDIR)\select.obj   $(OBJDIR)\settod.obj    \
       $(OBJDIR)\shutdown.obj $(OBJDIR)\signal.obj    \
       $(OBJDIR)\sock_dbu.obj $(OBJDIR)\sock_in.obj   \
       $(OBJDIR)\sock_ini.obj $(OBJDIR)\sock_io.obj   \
       $(OBJDIR)\sock_prn.obj $(OBJDIR)\sock_scn.obj  \
       $(OBJDIR)\sock_sel.obj $(OBJDIR)\socket.obj    \
       $(OBJDIR)\sockopt.obj  $(OBJDIR)\split.obj     \
       $(OBJDIR)\stream.obj   $(OBJDIR)\strings.obj   \
       $(OBJDIR)\syslog.obj   $(OBJDIR)\syslog2.obj   \
       $(OBJDIR)\tcp_fsm.obj  $(OBJDIR)\get_xbyr.obj  \
       $(OBJDIR)\tftp.obj     $(OBJDIR)\timer.obj     \
       $(OBJDIR)\transmit.obj $(OBJDIR)\udp_dom.obj   \
       $(OBJDIR)\udp_rev.obj  $(OBJDIR)\version.obj   \
       $(OBJDIR)\fsext.obj    $(OBJDIR)\wdpmi.obj     \
       $(OBJDIR)\x32vm.obj    $(OBJDIR)\rs232.obj     \
       $(OBJDIR)\get_ip.obj   $(OBJDIR)\pcsarp.obj    \
       $(OBJDIR)\idna.obj     $(OBJDIR)\punycode.obj  \
       $(OBJDIR)\tcp_md5.obj  $(OBJDIR)\dynip.obj     \
       $(OBJDIR)\winpcap.obj  $(OBJDIR)\winmisc.obj   \
       $(OBJDIR)\packet32.obj


ZLIB_OBJS = $(OBJDIR)\adler32.obj  $(OBJDIR)\compress.obj \
            $(OBJDIR)\crc32.obj    $(OBJDIR)\gzio.obj     \
            $(OBJDIR)\uncompr.obj  $(OBJDIR)\deflate.obj  \
            $(OBJDIR)\trees.obj    $(OBJDIR)\zutil.obj    \
            $(OBJDIR)\inflate.obj  $(OBJDIR)\infback.obj  \
            $(OBJDIR)\inftrees.obj $(OBJDIR)\inffast.obj

O = obj


########################################################################


########################################################################



OBJDIR  = pellesc
TARGETS = ..\lib\wattcppo.lib ..\lib\wattcppo_imp.lib ..\bin\watt-32.dll

CC      = pocc
CFLAGS  = -DWATT32_BUILD -MD -Ox -Ze -Zl -Zi -W1 -Go -Tx86-coff \
          -I$(PELLESC)\include -I$(PELLESC)\include\win -I..\inc
AS      = ml
AFLAGS  = -c -nologo -DDOSX -Zi -Fl -coff
LFLAGS  = -dll -map -libpath:$(PELLESC)\lib -libpath:$(PELLESC)\lib\Win \
          -nodefaultlib -machine:ix86 -subsystem:console -debug -verbose
EX_LIBS = user32.lib kernel32.lib advapi32.lib


all: $(TARGETS)

$(ZLIB_OBJS):
      @cd zlib
      $(MAKE) -f pellesc.mak
      @cd ..

..\lib\wattcppo.lib: $(OBJS) $(ZLIB_OBJS)
      polib -out:$@ $**

..\bin\watt-32.dll ..\lib\wattcppo_imp.lib: $(OBJS) $(ZLIB_OBJS)
      polink $(LFLAGS) -out:..\bin\watt-32.dll -implib:..\lib\wattcppo_imp.lib @<<
           $** $(EX_LIBS)
<<

clean:
      - @del $(TARGETS)
      - @del $(OBJDIR)\*.obj
      - @del *.lst
      @echo Cleaning done

$(OBJDIR)\chksum0.obj:  chksum0.asm
$(OBJDIR)\cpumodel.obj: cpumodel.asm

.c{$(OBJDIR)}.obj:
      $(CC) -c $(CFLAGS) -Fo$*.obj $<

.asm{$(OBJDIR)}.obj:
       $(AS) $(AFLAGS) -Fo $*.obj $<

language.c: language.l
       flex -8 -o$*.c $*.l

!include pellesc\watt32.dep


########################################################################

doxygen:
	doxygen doxyfile



visualc\release\watt-32.res: watt-32.rc
	rc -DDEBUG=0 -Fo visualc\release\watt-32.res watt-32.rc

visualc\debug\watt-32.res: watt-32.rc
	rc -DDEBUG=1 -Fo visualc\debug\watt-32.res watt-32.rc

watcom\win32\watt-32.res: watt-32.rc
	wrc -dDEBUG=0 -q -r -zm -fo=watcom\win32\watt-32.res watt-32.rc

digmars\win32\watt-32.res: watt-32.rc
	rcc -DDEBUG=0 -32 -r -odigmars\win32\watt-32.res watt-32.rc

lcc\watt-32.res: watt-32.rc
	lrc -DDEBUG=0 -i..\inc -olcc\watt-32.res watt-32.rc

pellesc\watt-32.res: watt-32.rc
	porc -DDEBUG=0 -I..\inc -Fopellesc\watt-32.res watt-32.rc

MingW32/watt-32.res: watt-32.rc
	windres -DDEBUG=0 -O coff -o $@ -i watt-32.rc

