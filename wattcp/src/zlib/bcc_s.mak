#
# NB! THIS MAKEFILE WAS AUTOMATICALLY GENERATED FROM ZLIB\MAKEFILE.ALL.
#     DO NOT EDIT.
#
# Makefile for zlib in Watt-32
#

SOURCE = adler32.c compress.c crc32.c gzio.c uncompr.c deflate.c \
         trees.c zutil.c inflate.c infback.c inftrees.c inffast.c

OBJS = $(OBJDIR)\adler32.obj  $(OBJDIR)\compress.obj \
       $(OBJDIR)\crc32.obj    $(OBJDIR)\gzio.obj     \
       $(OBJDIR)\uncompr.obj  $(OBJDIR)\deflate.obj  \
       $(OBJDIR)\trees.obj    $(OBJDIR)\zutil.obj    \
       $(OBJDIR)\inflate.obj  $(OBJDIR)\infback.obj  \
       $(OBJDIR)\inftrees.obj $(OBJDIR)\inffast.obj

########################################################################


CC     = bcc
CFLAGS = -ms -H -H=$(TEMP)\bcc_s.pch
OBJDIR = ..\borland\small


CFLAGS = -I.. -I..\..\inc -v -w -w+pro -n$(OBJDIR) $(CFLAGS)

.SWAP

all: $(OBJS)

.c.obj:
          $(CC) -c $(CFLAGS) $*.c

clean:
      @del $(OBJDIR)\*.obj

!include ..\borland\zlib.dep

########################################################################


