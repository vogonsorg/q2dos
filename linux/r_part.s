//
// r_part.s
// x86 assembly-language particle drawing code adapted from Win32 MASM
//

/ * FS: This is entirely unfinished, don't even try! */

#include "qasm.h"
#include "../ref_soft/d_ifacea.h"

#if id386

	.data
	.text
	.align 4

.global C(BlendParticle33)
C(BlendParticle33):
	//	return vid.alphamap[pcolor + dstcolor*256];
	movb C(vid.alphamap), %ebp
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
	movb C(vid.alphamap), %ebp
	xor %ebx, %ebx

	shl $8, %eax
	movb (%edi, %eax), %bl

	addl %ebx, %ebp
	addl %eax, %ebp

	__asm mov al,  byte ptr [ebp]

	__asm mov byte ptr [edi], al

	ret

.global C(BlendParticle100)
C(BlendParticle100):
	movb (%edi, %
	__asm mov byte ptr [edi], al
	ret

#endif /* id386 */
