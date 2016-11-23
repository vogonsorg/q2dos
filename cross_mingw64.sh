#!/bin/sh

. ./cross_defs.mingw64

if test "$1" = "strip"; then
	echo $TARGET-strip q2.exe
	$TARGET-strip q2.exe
	exit 0
fi

exec make CC=$TARGET-gcc RC=$TARGET-windres -f Makefile.mingw $*
