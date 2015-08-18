# $Id: Makefile.am,v 1.6.6.1 2003/03/27 20:43:33 brianp Exp $

AUTOMAKE_OPTIONS = gnu no-dependencies

# This is a hack (the -D options here)
INCLUDES = -I$(top_srcdir)/include -I../include -DNDEBUG -DLIBRARYBUILD

noinst_LTLIBRARIES = libutil.la

include libutil_la_SOURCES


include $(top_srcdir)/common_rules.make
