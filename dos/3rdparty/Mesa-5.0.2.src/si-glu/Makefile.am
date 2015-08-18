# $Id: Makefile.am,v 1.8.6.1 2003/03/27 18:20:24 brianp Exp $
# Makefile for SGI SI GLU

AUTOMAKE_OPTIONS =

SUBDIRS = include libnurbs libtess libutil

lib_LTLIBRARIES = libGLU.la

libGLU_la_SOURCES = dummy.cc

# XXX adding -lstdc++ to this line is a total hack, but appears to be
# the only solution to solving the problem:  libGLU.so must be linked with
# g++, not gcc since it contains C++ code and needs the C++ runtime library.
# Without this, linking any program with libGLU results in undefined symbols.
# I think there's a bug in libtool, but who knows where.
libGLU_la_LDFLAGS = -version-info @SI_GLU_VERSION_INFO@ -L../src -lstdc++
libGLU_la_LIBADD = \
	-lGL \
	libnurbs/interface/libNIFac.la \
	libnurbs/internals/libNInt.la \
	libnurbs/nurbtess/libNTess.la \
	libtess/libtess.la \
	libutil/libutil.la


include $(top_srcdir)/common_rules.make

EXTRA_DIST = descrip.mms mesaglu.opt
