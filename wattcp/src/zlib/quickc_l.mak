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


CC     = cl
CFLAGS = -AL
OBJDIR = ..\quickc\large


CFLAGS = $(CFLAGS) -nologo -O -W2 -Gs -Gr -G1 -Zi -Oa -Os -I..\..\inc

all: $(OBJS)

.c{$(OBJDIR)}.obj:
      $(CC) -c $(CFLAGS) -Fo$*.obj $<

clean:
      @del $(OBJDIR)\*.obj

!include ..\quickc\zlib.dep

########################################################################


