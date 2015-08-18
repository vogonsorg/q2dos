# $Id: Makefile.am,v 1.9.4.1 2003/03/27 20:43:23 brianp Exp $

AUTOMAKE_OPTIONS = gnu no-dependencies

INCLUDES = -I../../include -I../../../include -I../internals -I../nurbtess -DNDEBUG -DLIBRARYBUILD

noinst_LTLIBRARIES = libNIFac.la

# Choose one of STANDALONE, LIBRARYBUILD, and GLBUILD. They are debugging
# switches. grep LIBRARYBUILD `find .. -name '*.h'` for details.
CXXFLAGS += -DLIBRARYBUILD -DNDEBUG

include libNIFac_la_SOURCES


include $(top_srcdir)/common_rules.make
