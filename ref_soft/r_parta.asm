 .386P
 .model FLAT
;
; r_parta.s
; x86 assembly-language particle code
;

include qasm.inc
include d_if.inc

if	id386

_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT
_TEXT ENDS
endif	;id386
 END
