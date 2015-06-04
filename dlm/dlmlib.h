/*
 Copyright (C) 1996 By Ilya P. Ryzhenkov, see COPYDLM.TXT for details
*/
#ifndef __include_dlm_lib_h_
#define __include_dlm_lib_h_

#ifdef __cplusplus
extern "C" {
#endif

/* Loads DLM and resolve all posible references */
/* return 1 if success, 0 if failed and dlmerrno contains error code */
int LoadDLM(char *path);

/* Unloads DLM */
/* return 1 if success, 0 if failed and dlmerrno contains error code */
int UnloadDLM(char *path);

/* return 1 if DLM was loaded, otherwise 0 */
int isLoadedDLM(char *path);

/* Set the string for use when searching for DLM
   String must contain directories separated by ';'
   i.e "w:\\djgpp\\dlm\\dlms\\;w:\\djgpp\\dlm\\project\\dlms\\"
   Note trailing slashes.
   This function does not append paths, it replaces them. */
void SetDLMSearchPath(char *path);

/* Looks for given name in the EXPORT table and returns symbol's
   address or NULL if not found. */
void *LookupDLMSymbol(char *name);

extern int dlmerrno; /* This variable takes one of the following error codes */
                     /* if LoadDLM or UnloadDLM returned zero */
#define DLM_ENOERR 0 /* No error */
#define DLM_ENOMEM 1 /* Not enough memory */
#define DLM_EFILE  2 /* File I/O error (check errno) */
#define DLM_EALOAD 3 /* DLM Already loaded */
#define DLM_ENODLM 4 /* DLM was not loaded */
#define DLM_ECTOR  5 /* DLM's constructor() returned zero */
#define DLM_EINVF  6 /* Not DLM file */
#define DLM_EAUTO  7 /* DLM Auto-Loading failed */

extern int dlmflags; /* This variable controls overall DLM engine behavior */

 /* Valid DLM flags : */

/* This class of DLM flags makes sense only for export symbols. */
#define DLMFLAG_DUP 0x7 /* AND with this value to get/set duplicate export
                           symbol handling scheme */

#define DLMFLAG_DUP_IGNORE 0 /* Don't put duplicate exports to export
                                table at all. */
#define DLMFLAG_DUP_OVRIDE 1 /* Re-fixup all existing imports to the
                                new value and use it future. If DLM
                                unloads, reuse old values automaticaly.
                                This is default value. */
#define DLMFLAG_DUP_FUTURE 2 /* Use new value in future, leave already
                                defined as they are. If DLM
                                unloads, reuse old values automaticaly */
#define DLMFLAG_DUP_FAIL   3 /* Exit program on duplicate symbols. */

#ifdef __cplusplus
}

/* This is part of a NAMEDCLASS feature */
#define DLM_USE_NAMEDCLASS \
struct _SYSDLM_newdummy;  \
extern void *operator new(size_t ,char *,_SYSDLM_newdummy* dummy=0);

#define DLM_EXP_NAMEDCLASS(name) \
unsigned long _SYSDLM_sizeof_##name=sizeof(name);

#endif

#endif
