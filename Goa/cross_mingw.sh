#!/bin/sh

. ../cross_defs.mingw

if test "$1" = "strip"; then
	echo $TARGET-strip gamespy.dll
	$TARGET-strip gamespy.dll
	exit 0
fi

exec make CC=$TARGET-gcc -f Makefile.mingw $*
