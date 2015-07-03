#!/bin/sh

. ../../cross_defs.dj

exec make CC=$TARGET-gcc AR=$TARGET-ar -f makefile.dj $*
