#!/bin/sh

find . -name "*.o"|xargs rm -f
find . -name "*.dxe"|xargs rm -f
rm -f q2.exe
rm -rf _build
