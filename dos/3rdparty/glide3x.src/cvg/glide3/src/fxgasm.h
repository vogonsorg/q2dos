
;----------------------------------------------------------------------
; Assembler offsets for SSTREGS struct
;----------------------------------------------------------------------



;----------------------------------------------------------------------
; Assembler offsets for GC struct
;----------------------------------------------------------------------

base_ptr		equ 00000000h
reg_ptr			equ 00000004h
lfb_ptr			equ 0000000ch
tex_ptr			equ 00000008h
cull_mode		equ 000000d0h
tsuDataList		equ 00000014h
triPacketHdr	equ 00000358h
cullStripHdr	equ 0000035ch
paramMask	equ 00000360h
triSetupProc	equ 00000344h
drawTrianglesProc	equ 00000348h
drawVertexList	equ 0000034ch
fifoStart	equ 00000364h
fifoEnd	equ 00000368h
fifoOffset	equ 0000036ch
fifoSize	equ 00000370h
fifoJmpHdr	equ 00000374h
fifoPtr	equ 00000378h
fifoRead	equ 0000037ch
fifoRoom	equ 00000380h
roomToReadPtr	equ 00000384h
roomToEnd	equ 00000388h
fifoLfbP	equ 0000038ch
lfbLockCount	equ 00000390h
vertexSize	equ 00000278h
vertexStride	equ 00000274h
invalid	equ 00000280h
CoordinateSpace	equ 0000033ch
paramIndex	equ 000000d4h
vp_hwidth	equ 00000208h
vp_hheight	equ 0000020ch
vp_hdepth	equ 00000210h
vp_ox	equ 000001fch
vp_oy	equ 00000200h
vp_oz	equ 00000204h
colorType	equ 0000027ch
wInfo_offset	equ 00000228h
qInfo_mode	equ 0000025ch
qInfo_offset	equ 00000260h
q0Info_offset	equ 00000268h
q1Info_offset	equ 00000270h
q0Info_mode	equ 00000264h
q1Info_mode	equ 0000026ch
tmu0_s_scale	equ 00000144h
tmu0_t_scale	equ 00000148h
tmu1_s_scale	equ 00000184h
tmu1_t_scale	equ 00000188h
SIZEOF_GrState		equ 00000274h
SIZEOF_GC		equ 00000438h


;----------------------------------------------------------------------
; Assembler offsets for GlideRoot struct
;----------------------------------------------------------------------

p6Fencer		equ 00000000h
current_sst		equ 00000004h
CPUType			equ 00000008h
curGC			equ 0000002ch
curTriSize		equ 00000038h
stats_linesDrawn		equ 00000098h
stats_otherTrisDrawn		equ 000000a4h
trisProcessed		equ 0000009ch
trisDrawn		equ 000000a0h
pool_f0	equ 00000044h
pool_fHalf	equ 00000048h
pool_f1	equ 0000004ch
pool_f255	equ 00000050h
pool_fTemp1	equ 00000054h
pool_fTemp2	equ 00000058h
SIZEOF_GlideRoot	equ 0000126ch

