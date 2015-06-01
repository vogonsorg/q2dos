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


CC     = gcc
CFLAGS = -O2 -g -Wall -W -I../../inc
OBJDIR = ../mingw32

OBJS := $(subst \,/,$(OBJS))
OBJS := $(OBJS:.obj=.o)

all:  $(OBJS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

-include ../mingw32/zlib.dep


