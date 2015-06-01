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


CC     = dmc
CFLAGS = -mx
OBJDIR = ..\digmars\flat


CFLAGS = $(CFLAGS) -r -Jm -I$(DIGMARS)\include -I..\..\inc

all: $(OBJS)

.c.obj:
      $(CC) -c $(CFLAGS) $< -o$(OBJDIR)\$*.obj

clean:
      @del $(OBJDIR)\*.obj

!include ..\digmars\zlib.dep

########################################################################


