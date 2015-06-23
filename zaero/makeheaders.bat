@echo off
extractfuncts *.c -o gamefunc_list.h gamefunc_decs.h
extractfuncts *.c -t mmove_t -o gamemmove_list.h gamemmove_decs.h
extractfuncts *.c -t field_t -o fields.h levelfields.h clientfields.h

