//
// r_parta.s
// x86 assembly-language particle drawing code adapted from Win32 MASM
//

// FS: This is entirely unfinished, don't even try!

#include "qasm.h"
#include "../ref_soft/d_ifacea.h"

#if id386

	.data
	.text
	.align 4

.global C(BlendParticle33)
C(BlendParticle33):
	//	return vid.alphamap[pcolor + dstcolor*256];
	movl C(alphamap), %ebp
	xor %ebx, %ebx

	movb (%edi, %ebx), %bl
	shl $8, %ebx

	addl %ebx, %ebp
	addl %ebp, %eax

	movb (%ebp, %ebx), %al

	movb (%edi, %ebx), %al

	ret

.global C(BlendParticle66)
C(BlendParticle66):
	//	return vid.alphamap[pcolor*256 + dstcolor];
	movl C(alphamap), %ebp
	xor %ebx, %ebx

	shl $8, %eax
	movb (%edi, %eax), %bl

	addl %ebx, %ebp
	addl %eax, %ebp

//	__asm mov al,  byte ptr [ebp]

//	__asm mov byte ptr [edi], al

	ret

.global C(BlendParticle100)
C(BlendParticle100):
//	movb (%edi, %
//	__asm mov byte ptr [edi], al
	ret

#define epbsave 12+4

.globl C(R_DrawParticle)
C(D_DrawParticle):
//	static vec3_t	local, transformed;
//	static float	zi;
//	static int      u, v, tmp;
//	static short    izi;
//	static int      ebpsave;

//	static float    particle_z_clip    = PARTICLE_Z_CLIP;
//	static float    one                = 1.0F;
//	static float    point_five         = 0.5F;
//	static float    eight_thousand_hex = 0x8000;

// preserve caller's stack frame
	pushl	%ebp
// preserve register variables
	pushl	%esi
	pushl	%edi

	movl	ebpsave(%esp),%edi

// transform the particle
	// VectorSubtract (pparticle->origin, r_origin, local);
	movl C(partparms.particle), %esi

	fld  dword ptr [esi+0]
	fsub dword ptr [r_origin+0]
	fld  dword ptr [esi+4]
	fsubs C(r_origin)+4
	fld  dword ptr [esi+8]
	fsubs C(r_origin)+8
	fxch %st(2)
	fstp dword ptr [local+0]
	fstp dword ptr [local+4]
	fstp dword ptr [local+8]

	// transformed[0] = DotProduct(local, r_pright);
	// transformed[1] = DotProduct(local, r_pup);
	// transformed[2] = DotProduct(local, r_ppn);
	__asm fld  dword ptr [local+0]
	__asm fmul dword ptr [r_pright+0]
	__asm fld  dword ptr [local+4]
	__asm fmul dword ptr [r_pright+4]
	__asm fld  dword ptr [local+8]
	__asm fmul dword ptr [r_pright+8]
	fxch %st(2)
	__asm faddp st(1), st
	__asm faddp st(1), st
	__asm fstp  dword ptr [transformed+0]

	__asm fld  dword ptr [local+0]
	__asm fmul dword ptr [r_pup+0]
	__asm fld  dword ptr [local+4]
	__asm fmul dword ptr [r_pup+4]
	__asm fld  dword ptr [local+8]
	__asm fmul dword ptr [r_pup+8]
	fxch %st(2)
	__asm faddp st(1), st
	__asm faddp st(1), st
	__asm fstp  dword ptr [transformed+4]

	__asm fld  dword ptr [local+0]
	__asm fmul dword ptr [r_ppn+0]
	__asm fld  dword ptr [local+4]
	__asm fmul dword ptr [r_ppn+4]
	__asm fld  dword ptr [local+8]
	__asm fmul dword ptr [r_ppn+8]
	fxch %st(2)
	__asm faddp st(1), st
	__asm faddp st(1), st
	__asm fstp  dword ptr [transformed+8]

	/*
	** make sure that the transformed particle is not in front of
	** the particle Z clip plane.  We can do the comparison in 
	** integer space since we know the sign of one of the inputs
	** and can figure out the sign of the other easily enough.
	*/
	//	if (transformed[2] < PARTICLE_Z_CLIP)
	//		return;

	__asm mov  eax, dword ptr [transformed+8]
	andl %eax, %eax
	js end
	__asm cmp  eax, particle_z_clip
	jl end

	/*
	** project the point by initiating the 1/z calc
	*/
	//	zi = 1.0 / transformed[2];
	__asm fld   one
	__asm fdiv  dword ptr [transformed+8]

	/*
	** bind the blend function pointer to the appropriate blender
	** while we're dividing
	*/
	//if ( level == PARTICLE_33 )
	//	blendparticle = BlendParticle33;
	//else if ( level == PARTICLE_66 )
	//	blendparticle = BlendParticle66;
	//else 
	//	blendparticle = BlendParticle100;

	__asm cmp partparms.level, PARTICLE_66
	je  blendfunc_66
	jl  blendfunc_33
	__asm lea ebx, BlendParticle100
	jmp done_selecting_blend_func
blendfunc_33:
	__asm lea ebx, BlendParticle33
	jmp done_selecting_blend_func
blendfunc_66:
	__asm lea ebx, BlendParticle66
done_selecting_blend_func:
	__asm mov blendfunc, ebx

	// prefetch the next particle
	__asm mov ebp, s_prefetch_address
	__asm mov ebp, [ebp]

	// finish the above divide
	__asm fstp  zi

	// u = (int)(xcenter + zi * transformed[0] + 0.5);
	// v = (int)(ycenter - zi * transformed[1] + 0.5);
	__asm fld   zi
	__asm fmul  dword ptr [transformed+0]
	__asm fld   zi
	__asm fmul  dword ptr [transformed+4]
	fxch  %st(1)
	__asm fadd  xcenter
	fxch  %st(1)
	__asm fld   ycenter
	__asm fsubrp st(1), st(0)
  	fxch  %st(1)
  	__asm fadd  point_five
  	fxch  %st(1)
  	__asm fadd  point_five
  	fxch  %st(1)
  	__asm fistp dword ptr [u]
  	__asm fistp dword ptr [v]

	/*
	** clip out the particle
	*/

	//	if ((v > d_vrectbottom_particle) || 
	//		(u > d_vrectright_particle) ||
	//		(v < d_vrecty) ||
	//		(u < d_vrectx))
	//	{
	//		return;
	//	}

	__asm mov ebx, u
	__asm mov ecx, v
	cmpl C(d_vrectbottom_particle), %ecx
	jg end
	cmpl C(d_vrecty), %ecx
	jl end
	cmpl C(d_vrectright_particle), %ebx
	jg end
	cmpl C(d_vrectx), %ebx
	jl end

	/*
	** compute addresses of zbuffer, framebuffer, and 
	** compute the Z-buffer reference value.
	**
	** EBX      = U
	** ECX      = V
	**
	** Outputs:
	** ESI = Z-buffer address
	** EDI = framebuffer address
	*/
	// ESI = d_pzbuffer + (d_zwidth * v) + u;
	movl C(d_pzbuffer), %esi
	movl C(d_zwidth), %eax
	__asm mul ecx                         ; eax = d_zwidth*v
	addl %ebx, %eax
	shll $1, %eax
	addl %eax, %esi

	// initiate
	// izi = (int)(zi * 0x8000);
	__asm fld  zi
	__asm fmul eight_thousand_hex

	// EDI = pdest = d_viewbuffer + d_scantable[v] + u;
	__asm lea edi, [d_scantable+ecx*4]
	__asm mov edi, [edi]
	addl C(d_viewbuffer), %edi
	addl %ebx, %edi

	// complete
	// izi = (int)(zi * 0x8000);
	__asm fistp tmp
	__asm mov   eax, tmp
	__asm mov   izi, ax

	/*
	** determine the screen area covered by the particle,
	** which also means clamping to a min and max
	*/
	//	pix = izi >> d_pix_shift;
	xor %edx, %edx
	__asm mov dx, izi
	movl C(d_pix_shift), %ecx
	__asm shr dx, cl

	//	if (pix < d_pix_min)
	//		pix = d_pix_min;
	cmpl C(d_pix_min), %edx
	jge check_pix_max
	movl C(d_pix_min), %edx
	jmp skip_pix_clamp

	//	else if (pix > d_pix_max)
	//		pix = d_pix_max;
check_pix_max:
	cmpl C(d_pix_max), %edx
	jle skip_ipx_clamp
	movl C(d_pix_max), %edx

skip_pix_clamp:

	/*
	** render the appropriate pixels
	**
	** ECX = count (used for inner loop)
	** EDX = count (used for outer loop)
	** ESI = zbuffer
	** EDI = framebuffer
	*/
	movl %edx, %ecx

	cmpl $1, %ecx
	ja over
over:

	/*
	** at this point:
	**
	** ECX = count
	*/
	pushl %ecx
	pushl %edi
	pushl %esi

top_of_pix_vert_loop:

top_of_pix_horiz_loop:

	//	for ( ; count ; count--, pz += d_zwidth, pdest += screenwidth)
	//	{
	//		for (i=0 ; i<pix ; i++)
	//		{
	//			if (pz[i] <= izi)
	//			{
	//				pdest[i] = blendparticle( color, pdest[i] );
	//			}
	//		}
	//	}
	xor %eax, %eax

	__asm mov   ax, word ptr [esi]

	__asm cmp   ax, izi
	jg end_of_horiz_loop

#if ENABLE_ZWRITES_FOR_PARTICLES
  	__asm mov   bp, izi
  	__asm mov   word ptr [esi], bp
#endif

	movl C(partparms.color), %eax

	call blendfunc

	addl $1, %edi
	addl $2, %esi

end_of_horiz_loop:

	decl %ecx
	jnz top_of_pix_horiz_loop

	popl %esi
	popl %edi

	movl C(d_zwidth), %ebp
	shll $1, %ebp

	addl %ebp, %esi
	addl C(r_screenwidth), %edi

	popl %ecx

	pushl %ecx

	pushl %edi
	pushl %esi

	decl %edx
	jnz top_of_pix_vert_loop

	popl %ecx
	popl %ecx
	popl %ecx

end:
	popl %edi
	popl %esi
	movl epbsave, %ebp
	ret

#endif /* id386 */
