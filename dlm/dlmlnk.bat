@echo off
if %1. == . goto usage

echo Building libc.dlm :
echo  - linking ...
ld -r -o _tmpdlm.o *.o -Tc:/usr/djgpp/lib/dlm/dlm.djl
echo  - dlming ...
dlmman _tmpdlm.o -c -q
echo  - cleaning ...
rm _tmpdlm.o
mv _tmpdlm.o %1
echo Done.
goto end

:usage
echo USAGE : dlmlnk output
echo where output is output file name 
echo This bat-file will combine all .o files in current directory
echo into single DLM
echo Example : dlmlink libc.dlm
echo will produce libc.dlm combining all .o files in current directory

:end