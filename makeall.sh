#!/bin/sh

. ./cross_defs.dj

make CC=$TARGET-gcc -f Makefile.dj $*     || exit 1
# refresh drivers
rm -f game/*.o
make CC=$TARGET-gcc -C ref_soft -f Makefile.dj $* || exit 1
rm -f game/*.o
make CC=$TARGET-gcc -C ref_gl -f Makefile.dj $* || exit 1
# game code
rm -f game/*.o
make CC=$TARGET-gcc -C game -f Makefile.dj $*   || exit 1
make CC=$TARGET-gcc -C ctf -f Makefile.dj $*    || exit 1
make CC=$TARGET-gcc -C xsrc -f Makefile.dj $*   || exit 1
make CC=$TARGET-gcc -C rsrc -f Makefile.dj $*   || exit 1
make CC=$TARGET-gcc -C 3zb2 -f Makefile.dj $*   || exit 1
make CC=$TARGET-gcc -C acebot -f Makefile.dj $* || exit 1
make CC=$TARGET-gcc -C action -f Makefile.dj $* || exit 1
make CC=$TARGET-gcc -C chaos -f Makefile.dj $*  || exit 1
make CC=$TARGET-gcc -C dday -f Makefile.dj $*   || exit 1
make CC=$TARGET-gcc -C zaero -f Makefile.dj $*  || exit 1
# gamespy shit
make CC=$TARGET-gcc -C Goa -f Makefile.dj $*    || exit 1
