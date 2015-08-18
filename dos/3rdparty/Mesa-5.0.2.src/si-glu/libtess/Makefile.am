# $Id: Makefile.am,v 1.7.6.1 2003/03/27 20:43:32 brianp Exp $

AUTOMAKE_OPTIONS = gnu no-dependencies

# This is a hack (the -D options here)
INCLUDES = -I../include -I../../include -DNDEBUG -DLIBRARYBUILD

noinst_LTLIBRARIES = libtess.la

include libtess_la_SOURCES


include $(top_srcdir)/common_rules.make
