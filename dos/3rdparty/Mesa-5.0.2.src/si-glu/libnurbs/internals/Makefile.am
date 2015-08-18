# $Id: Makefile.am,v 1.8.4.1 2003/03/27 20:43:30 brianp Exp $

AUTOMAKE_OPTIONS = gnu no-dependencies

INCLUDES = -I../../include -I../nurbtess -DNDEBUG -DLIBRARYBUILD

noinst_LTLIBRARIES = libNInt.la

# Choose one of STANDALONE, LIBRARYBUILD, and GLBUILD. They are debugging
# switches. grep LIBRARYBUILD `find .. -name '*.h'` for details.
CXXFLAGS += -DLIBRARYBUILD -DNDEBUG

include libNInt_la_SOURCES


include $(top_srcdir)/common_rules.make
