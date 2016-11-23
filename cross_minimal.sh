#!/bin/sh

. ./cross_defs.dj

if test "$1" = "strip"; then
	echo $TARGET-strip q2.exe
	$TARGET-strip q2.exe
	exit 0
fi

exec make CC=$TARGET-gcc -f Makefile.dj USE_WATT32=0 USE_GAMESPY=0 USE_CURL=0 USE_OGG=0 USE_SNDPCI=0 $*
