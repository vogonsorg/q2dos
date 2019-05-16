//
// snd_mixa.s
// x86 assembly-language transform vector code
//

#include "qasm.h"

#if	id386

	.text

//----------------------------------------------------------------------
// transform vector code
//----------------------------------------------------------------------

#define vin		0+4
#define vout		4+4

.globl C(TransformVector)
C(TransformVector):
	movl vin(%esp),%eax
	movl vout(%esp),%edx

	fld %eax(0)
	fmuls C(vright)+0
	fld %eax(0)
	fmuls C(vup)+0
	fld %eax(0)
	fmuls C(vpn)+0

	fld %eax(4)
	fmuls C(vright)+4
	fld %eax(4)
	fmuls C(vup)+4
	fld %eax(4)
	fmuls C(vpn)+4

	fxch %st(2)

	faddp %st(0),%st(5)
	faddp %st(0),%st(3)
	faddp %st(0),%st(1)

	fstp %edx(8)
	fstp %edx(4)
	fstp %edx(0)

	ret

#endif	// id386

