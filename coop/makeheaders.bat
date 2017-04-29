@echo off
extractfuncts *.c -o tables\gamefunc_list.h tables\gamefunc_decs.h
extractfuncts *.c -t mmove_t -o tables\gamemmove_list.h tables\gamemmove_decs.h
REM extractfuncts *.c -t field_t -o fields.h levelfields.h clientfields.h

