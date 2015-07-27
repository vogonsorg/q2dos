#!/bin/sh

. ./cross_defs.dj

make CC=$TARGET-gcc -f makedxe3.frank $*     || exit 1
rm -f game/*.o
make CC=$TARGET-gcc -C ref_soft -f makedxe3 $* || exit 1
rm -f game/*.o
make CC=$TARGET-gcc -C game -f makedxe3 $*   || exit 1
make CC=$TARGET-gcc -C ctf -f makedxe3 $*    || exit 1
make CC=$TARGET-gcc -C xsrc -f makedxe3 $*   || exit 1
make CC=$TARGET-gcc -C rsrc -f makedxe3 $*   || exit 1
make CC=$TARGET-gcc -C 3zb2 -f makedxe3 $*   || exit 1
make CC=$TARGET-gcc -C acebot -f makedxe3 $* || exit 1
make CC=$TARGET-gcc -C action -f makedxe3 $* || exit 1
make CC=$TARGET-gcc -C chaos -f makedxe3 $*  || exit 1
make CC=$TARGET-gcc -C dday -f makedxe3 $*   || exit 1
make CC=$TARGET-gcc -C zaero -f makedxe3 $*  || exit 1
# gamespy shit
make CC=$TARGET-gcc -C Goa -f makedxe3 $*    || exit 1
