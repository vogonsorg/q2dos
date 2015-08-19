/*
 * Select the syntax appropriate to the 386 assembler being used.
 *
 *  Copyright (c) 2002 - Borca Daniel
 *  Email : dborca@users.sourceforge.net
 *  Web   : http://www.geocities.com/dborca
 *
 * $Header: /cvsroot/glide/glide3x/h3/glide3/src/Attic/assyntax.h,v 1.1.2.2 2003/07/29 10:04:32 dborca Exp $
 */


#ifndef ASSYNTAX_H_included
#define ASSYNTAX_H_included

#ifdef __DJGPP__

#define type version
#define size version
#define CONCAT(a,b) a##b
#define GLOBL(name) .globl CONCAT(_,name) ; CONCAT(_,name) :
#define ALIGN(n)    .balign n
#define EXTRN(name) CONCAT(_,name)

#else

#define GLOBL(name) .globl name; .type name, @function; name:
#define ALIGN(n)    .align n
#define EXTRN(name) name

#endif

#endif
