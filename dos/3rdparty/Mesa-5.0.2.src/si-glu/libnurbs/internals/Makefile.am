# $Id: Makefile.am,v 1.8.4.2 2003/11/07 14:43:30 brianp Exp $

AUTOMAKE_OPTIONS = gnu no-dependencies

INCLUDES = -I../../include -I../nurbtess -DNDEBUG -DLIBRARYBUILD

noinst_LTLIBRARIES = libNInt.la

# Choose one of STANDALONE, LIBRARYBUILD, and GLBUILD. They are debugging
# switches. grep LIBRARYBUILD `find .. -name '*.h'` for details.
AM_CXXFLAGS = -DLIBRARYBUILD -DNDEBUG

include libNInt_la_SOURCES


include $(top_srcdir)/common_rules.make
