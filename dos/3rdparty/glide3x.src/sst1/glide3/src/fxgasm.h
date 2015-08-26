SST_CHIP_MASK equ 03c00h

;----------------------------------------------------------------------
; Assembler offsets for SSTREGS struct
;----------------------------------------------------------------------


FDPDX_OFFSET		equ 00000004h
FDPDY_OFFSET		equ 00000008h

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
writesSinceFence	equ 000006b8h
paramMask		equ 000006c0h
gwCommand		equ 000006c4h
gwHeaders		equ 000006c8h
fifoPtr			equ 000006ach
fifoSize		equ 000006b0h
SIZEOF_GrState		equ 0000024ch
SIZEOF_GrHwConfiguration	equ 00000094h
SIZEOF_GC		equ 00000754h


;----------------------------------------------------------------------
; Assembler offsets for GlideRoot struct
;----------------------------------------------------------------------

p6Fencer		equ 00000000h
current_sst		equ 00000004h
CPUType			equ 00000008h
curGC			equ 0000000ch
curTriSize		equ 00000010h
trisProcessed		equ 00000080h
trisDrawn		equ 00000084h
SIZEOF_GlideRoot	equ 00001e7ch


;----------------------------------------------------------------------
; Assembler offsets for dataList struct
;----------------------------------------------------------------------

dl_i			equ 00000000h
dl_addr			equ 00000004h
dl_bddr			equ 00000008h
SIZEOF_dataList		equ 00000014h

