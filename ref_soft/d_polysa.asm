 .386P
 .model FLAT
;
; d_polysa.s
; x86 assembly-language polygon model drawing code.
;

include qasm.inc
include d_if.inc

if id386

PUBLIC	_R_PolysetCalcGradients
_BSS	SEGMENT
	ALIGN	4

t1 DD 01H DUP (?)
t0 DD 01H DUP (?)
t1_int DD 01H DUP (?)
t0_int DD 01H DUP (?)
ystepdenominv DD 01H DUP (?)
xstepdenominv DD 01H DUP (?)
p11_minus_p21 DD 01H DUP (?)
p10_minus_p20 DD 01H DUP (?)
p01_minus_p21 DD 01H DUP (?)
p00_minus_p20 DD 01H DUP (?)

_BSS	ENDS
_TEXT	SEGMENT
_skinwidth$ = 8 ; size = 4
_R_PolysetCalcGradients PROC

	push	ebp
	mov	ebp, esp
	push	ebx

;
; p00_minus_p20 = r_p0[0] - r_p2[0];
; p01_minus_p21 = r_p0[1] - r_p2[1];
; p10_minus_p20 = r_p1[0] - r_p2[0];
; p11_minus_p21 = r_p1[1] - r_p2[1];
;

	mov	eax, DWORD PTR [_r_p0+0]
	mov	ebx, DWORD PTR [_r_p0+4]
	sub	eax, DWORD PTR [_r_p2+0]
	sub	ebx, DWORD PTR [_r_p2+4]
	mov	DWORD PTR p00_minus_p20, eax
	mov	DWORD PTR p01_minus_p21, ebx
	fild	DWORD PTR p00_minus_p20
	fild	DWORD PTR p01_minus_p21
	mov	eax, DWORD PTR [_r_p1+0]
	mov	ebx, DWORD PTR [_r_p1+4]
	sub	eax, DWORD PTR [_r_p2+0]
	sub	ebx, DWORD PTR [_r_p2+4]
	fstp	DWORD PTR p01_minus_p21
	fstp	DWORD PTR p00_minus_p20
	mov	DWORD PTR p10_minus_p20, eax
	mov	DWORD PTR p11_minus_p21, ebx
	fild	DWORD PTR p10_minus_p20
	fild	DWORD PTR p11_minus_p21
	fstp	DWORD PTR p11_minus_p21
	fstp	DWORD PTR p10_minus_p20
;
; xstepdenominv = 1.0 / (float)d_xdenom;
;
; ystepdenominv = -xstepdenominv;
;

;
; put FPU in single precision ceil mode
;

	fldcw	WORD PTR _fpu_sp24_ceil_cw

	fild	DWORD PTR _d_xdenom
	fdivr	DWORD PTR float_1
	fst	DWORD PTR xstepdenominv
	fmul	DWORD PTR float_minus_1

; ceil () for light so positive steps are exaggerated, negative steps
; diminished,  pushing us away from underflow toward overflow. Underflow is
; very visible, overflow is very unlikely, because of ambient lighting
;
; t0 = r_p0[4] - r_p2[4];
; t1 = r_p1[4] - r_p2[4];
; r_lstepx = (int)
; 		ceil((t1 * p01_minus_p21 - t0 * p11_minus_p21) * xstepdenominv);
; r_lstepy = (int)
; 		ceil((t1 * p00_minus_p20 - t0 * p10_minus_p20) * ystepdenominv);
;
	mov	eax, DWORD PTR [_r_p0+16]
	mov	ebx, DWORD PTR [_r_p1+16]
	sub	eax, DWORD PTR [_r_p2+16]
	sub	ebx, DWORD PTR [_r_p2+16]

	fstp	DWORD PTR ystepdenominv

	mov	DWORD PTR t0_int, eax
	mov	DWORD PTR t1_int, ebx
	fild	DWORD PTR t0_int
	fild	DWORD PTR t1_int
	fxch	ST(1)
	fstp	DWORD PTR t0
	fst	DWORD PTR t1
	fmul	DWORD PTR p01_minus_p21
	fld	DWORD PTR t0
	fmul	DWORD PTR p11_minus_p21
	fld	DWORD PTR t1
	fmul	DWORD PTR p00_minus_p20
	fld	DWORD PTR t0
	fmul	DWORD PTR p10_minus_p20
	fxch	ST(2)
	fsubp	ST(3), ST(0)
	fsubrp	ST(1), ST(0)
	fxch	ST(1)
	fmul	DWORD PTR xstepdenominv
	fxch	ST(1)
	fmul	DWORD PTR ystepdenominv
	fxch	ST(1)
	fistp	DWORD PTR _r_lstepx
	fistp	DWORD PTR _r_lstepy

;
; put FPU back into extended precision chop mode
;
	fldcw	WORD PTR _fpu_chop_cw

;
; t0 = r_p0[2] - r_p2[2];
; t1 = r_p1[2] - r_p2[2];
; r_sstepx = (int)((t1 * p01_minus_p21 - t0 * p11_minus_p21) *
; 		xstepdenominv);
; r_sstepy = (int)((t1 * p00_minus_p20 - t0* p10_minus_p20) *
; 		ystepdenominv);
;
	mov	eax, DWORD PTR [_r_p0+8]
	mov	ebx, DWORD PTR [_r_p1+8]
	sub	eax, DWORD PTR [_r_p2+8]
	sub	ebx, DWORD PTR [_r_p2+8]
	mov	DWORD PTR t0_int, eax
	mov	DWORD PTR t1_int, ebx
	fild	DWORD PTR t0_int
	fild	DWORD PTR t1_int
	fxch	ST(1)
	fstp	DWORD PTR t0
	fst	DWORD PTR t1

	fmul	DWORD PTR p01_minus_p21
	fld	DWORD PTR t0
	fmul	DWORD PTR p11_minus_p21
	fld	DWORD PTR t1
	fmul	DWORD PTR p00_minus_p20
	fld	DWORD PTR t0
	fmul	DWORD PTR p10_minus_p20
	fxch	ST(2)
	fsubp	ST(3), ST(0)
	fsubrp	ST(1), ST(0)
	fxch	ST(1)
	fmul	DWORD PTR xstepdenominv
	fxch	ST(1)
	fmul	DWORD PTR ystepdenominv
	fxch	ST(1)
	fistp	DWORD PTR [_r_sstepx]
	fistp	DWORD PTR [_r_sstepy]

;
; t0 = r_p0[3] - r_p2[3];
; t1 = r_p1[3] - r_p2[3];
; r_tstepx = (int)((t1 * p01_minus_p21 - t0 * p11_minus_p21) *
; 		xstepdenominv);
; r_tstepy = (int)((t1 * p00_minus_p20 - t0 * p10_minus_p20) *
; 		ystepdenominv);
;
	mov	eax, DWORD PTR [_r_p0+12]
	mov	ebx, DWORD PTR [_r_p1+12]
	sub	eax, DWORD PTR [_r_p2+12]
	sub	ebx, DWORD PTR [_r_p2+12]
	mov	DWORD PTR t0_int, eax
	mov	DWORD PTR t1_int, ebx
	fild	DWORD PTR t0_int
	fild	DWORD PTR t1_int
	fxch	ST(1)
	fstp	DWORD PTR t0
	fst	DWORD PTR t1
	fmul	DWORD PTR p01_minus_p21
	fld	DWORD PTR t0
	fmul	DWORD PTR p11_minus_p21
	fld	DWORD PTR t1
	fmul	DWORD PTR p00_minus_p20
	fld	DWORD PTR t0
	fmul	DWORD PTR p10_minus_p20
	fxch	ST(2)
	fsubp	ST(3), ST(0)
	fsubrp	ST(1), ST(0)
	fxch	ST(1)
	fmul	DWORD PTR xstepdenominv
	fxch	ST(1)
	fmul	DWORD PTR ystepdenominv
	fxch	ST(1)
	fistp	DWORD PTR _r_tstepx
	fistp	DWORD PTR _r_tstepy

;
; t0 = r_p0[5] - r_p2[5];
; t1 = r_p1[5] - r_p2[5];
; r_zistepx = (int)((t1 * p01_minus_p21 - t0 * p11_minus_p21) *
; 		xstepdenominv);
; r_zistepy = (int)((t1 * p00_minus_p20 - t0 * p10_minus_p20) *
; 		ystepdenominv);
;
	mov	eax, DWORD PTR [_r_p0+20]
	mov	ebx, DWORD PTR [_r_p1+20]
	sub	eax, DWORD PTR [_r_p2+20]
	sub	ebx, DWORD PTR [_r_p2+20]

	mov	DWORD PTR t0_int, eax
	mov	DWORD PTR t1_int, ebx
	fild	DWORD PTR t0_int
	fild	DWORD PTR t1_int
	fxch	ST(1)
	fstp	DWORD PTR t0

	fst	DWORD PTR t1
	fmul	DWORD PTR p01_minus_p21
	fld	DWORD PTR t0
	fmul	DWORD PTR p11_minus_p21
	fld	DWORD PTR t1
	fmul	DWORD PTR p00_minus_p20
	fld	DWORD PTR t0
	fmul	DWORD PTR p10_minus_p20
	fxch	ST(2)
	fsubp	ST(3), ST(0)
	fsubrp	ST(1), ST(0)
	fxch	ST(1)
	fmul	DWORD PTR xstepdenominv
	fxch	ST(1)
	fmul	DWORD PTR ystepdenominv
	fxch	ST(1)
	fistp	DWORD PTR [_r_zistepx]
	fistp	DWORD PTR [_r_zistepy]

;
; #if	id386ALIAS
; 	a_sstepxfrac = r_sstepx << 16;
; 	a_tstepxfrac = r_tstepx << 16;
; #else
; 	a_sstepxfrac = r_sstepx & 0xFFFF;
; 	a_tstepxfrac = r_tstepx & 0xFFFF;
; #endif
;
	mov	eax, DWORD PTR _d_pdrawspans
	cmp	eax, OFFSET _R_PolysetDrawSpans8_Opaque
	mov	eax, DWORD PTR _r_sstepx
	mov	ebx, DWORD PTR _r_tstepx
	jne	translucent
; #if id386ALIAS
	shl	eax, 16					; 00000010H
	shl	ebx, 16					; 00000010H
	jmp	done_with_steps
; #else
translucent:
	and	eax, 65535				; 0000ffffH
	and	ebx, 65535				; 0000ffffH
; #endif
done_with_steps:
	mov	DWORD PTR _a_sstepxfrac, eax
	mov	DWORD PTR _a_tstepxfrac, ebx

;
; a_ststepxwhole = skinwidth * (r_tstepx >> 16) + (r_sstepx >> 16);
;
	mov	ebx, DWORD PTR _r_tstepx
	mov	ecx, DWORD PTR _r_sstepx
	sar	ebx, 16					; 00000010H
	mov	eax, DWORD PTR _skinwidth$[ebp]
	mul	ebx
	sar	ecx, 16					; 00000010H
	add	eax, ecx
	mov	DWORD PTR _a_ststepxwhole, eax

	pop	ebx
	pop	ebp
	ret	0
_R_PolysetCalcGradients ENDP
_TEXT	ENDS
endif	;id386
 END
