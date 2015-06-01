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


CC     = cc386
CFLAGS = +v -O+a -O+i -C+N -C+S -I..\..\inc
OBJDIR = ..\ladsoft

.SWAP

all: $(OBJS)

.c.obj:
          $(CC) -c $(CFLAGS) $*.c -o$(OBJDIR)\$*.asm
          nasm -s -f obj -l $(OBJDIR)\$*.lst -o $(OBJDIR)\$*.obj $(OBJDIR)\$*.asm

clean:
      @del $(OBJDIR)\*.obj
      @del $(OBJDIR)\*.lst
      @del $(OBJDIR)\*.asm

!include ..\ladsoft\zlib.dep

########################################################################


