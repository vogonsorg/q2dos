SST_CHIP_MASK equ 03c00h

;----------------------------------------------------------------------
; Assembler offsets for SSTREGS struct
;----------------------------------------------------------------------

FVAX			equ 00000088h
FVAY			equ 0000008ch
FVBX			equ 00000090h
FVBY			equ 00000094h
FVCX			equ 00000098h
FVCY			equ 0000009ch
FTRIANGLECMD		equ 00000100h

FDPDX_OFFSET		equ 00000020h
FDPDY_OFFSET		equ 00000040h

;----------------------------------------------------------------------
; Assembler offsets for GC struct
;----------------------------------------------------------------------

base_ptr		equ 00000000h
reg_ptr			equ 00000004h
lfb_ptr			equ 0000000ch
cull_mode		equ 000003c4h
dataList		equ 00000018h
invalid		equ 0000055ch
coord_space		equ 0000060ch
color_type		equ 00000558h
fifoFree		equ 000003c8h
SIZEOF_GrState		equ 0000024ch
SIZEOF_GrHwConfiguration	equ 00000094h
SIZEOF_GC		equ 00000750h


;----------------------------------------------------------------------
; Assembler offsets for GlideRoot struct
;----------------------------------------------------------------------

p6Fencer		equ 00000000h
current_sst		equ 00000004h
curGC			equ 00000008h
curTriSize		equ 0000000ch
trisProcessed		equ 0000007ch
trisDrawn		equ 00000080h
SIZEOF_GlideRoot	equ 00001e68h


;----------------------------------------------------------------------
; Assembler offsets for dataList struct
;----------------------------------------------------------------------

dl_i			equ 00000000h
dl_addr			equ 00000004h
dl_bddr			equ 00000008h
SIZEOF_dataList		equ 00000014h

