
#include <sys/types.h>
#include <sys/stat.h> /* mkdir() */
#include <errno.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include <io.h>
#include <sys/time.h>

#include "../qcommon/qcommon.h"

static byte	*membase;
static int	maxhunksize;
static int	curhunksize;

void	*Hunk_Begin (int maxsize)
{
	/* reserve a huge chunk of memory, but don't commit any yet */
	maxhunksize = maxsize;
	curhunksize = 0;
	membase = malloc (maxhunksize);
	if (!membase)
		Sys_Error ("VirtualAlloc reserve failed %d bytes",maxsize);

	memset (membase, 0, maxsize);
	return (void *)membase;
}

void	*Hunk_Alloc (int size)
{
	/* round to cacheline */
	size = (size+31)&~31;

	curhunksize += size;
	if (curhunksize > maxhunksize)
		Sys_Error ("Hunk_Alloc overflow");

	return (void *)(membase+curhunksize-size);
}

void	Hunk_Free (void *buf)
{
	free (buf);
}

int	Hunk_End (void)
{
/* for realloc() to be useful here: you either need DJGPP-2.05 or newer,
 * or you need to replace malloc() & friends in any older DJGPP version
 * with nmalloc() as in DJGPP-2.05. */
	byte *n = realloc(membase, curhunksize);
	if (n != membase)
		Sys_Error("Hunk_End:  Could not remap virtual block (%d)", errno);

	return curhunksize;
}

double	curtime;
double	Sys_Milliseconds (void)
{
	/* FS: Needed *1000 to work properly */
	curtime = (double) uclock() / (double) UCLOCKS_PER_SEC*1000; /* FS: From Q1 -- Win9X/Fast PC Fix (QIP) */

	return curtime;
}

int	Sys_LinuxTime (void) /* FS: DOS needs this for random qport */
{
	int linuxtime;
	struct timeval tp;
	static int		secbase;

	gettimeofday(&tp, NULL);

	if (!secbase)
	{
		secbase = tp.tv_sec;
		return tp.tv_usec/1000;
	}

	linuxtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;

	return linuxtime;
}

void	Sys_Mkdir (char *path)
{
	mkdir (path, 0777);
}

static	struct ffblk	finddata;
static	int	findhandle = -1;
static	char	findbase[MAX_OSPATH];
static	char	findpath[MAX_OSPATH];

static qboolean CompareAttributes(char *name, unsigned found,
				  unsigned musthave, unsigned canthave)
{
	/* . and .. never match */
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
		return false;

	if (found & _A_VOLID) /* shouldn't happen */
		return false;

	if ((found & _A_SUBDIR) && (canthave & SFF_SUBDIR))
		return false;
	if ((musthave & SFF_SUBDIR) && !(found & _A_SUBDIR))
		return false;

	if ((found & _A_RDONLY) && (canthave & SFF_RDONLY))
		return false;
	if ((musthave & SFF_RDONLY) && !(found & _A_RDONLY))
		return false;

	if ((found & _A_HIDDEN) && (canthave & SFF_HIDDEN))
		return false;
	if ((musthave & SFF_HIDDEN) && !(found & _A_HIDDEN))
		return false;

	if ((found & _A_SYSTEM) && (canthave & SFF_SYSTEM))
		return false;
	if ((musthave & SFF_SYSTEM) && !(found & _A_SYSTEM))
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave)
{
	int attribs;

	if (findhandle == 0)
		Sys_Error ("Sys_BeginFind without close");

	COM_FilePath (path, findbase);
	memset (&finddata, 0, sizeof(finddata));

	attribs = FA_ARCH|FA_RDONLY;
	if (!(canthave & SFF_SUBDIR))
		attribs |= FA_DIREC;
	if (musthave & SFF_HIDDEN)
		attribs |= FA_HIDDEN;
	if (musthave & SFF_SYSTEM)
		attribs |= FA_SYSTEM;

	findhandle = findfirst(path, &finddata, attribs);
	if (findhandle != 0)
		return NULL;
	if (CompareAttributes(finddata.ff_name, finddata.ff_attrib, musthave, canthave)) {
		sprintf (findpath, "%s/%s", findbase, finddata.ff_name);
		return findpath;
	}
	return Sys_FindNext(musthave, canthave);
}

char *Sys_FindNext (unsigned musthave, unsigned canthave)
{
	if (findhandle != 0)
		return NULL;

	while (findnext(&finddata) == 0) {
		if (CompareAttributes(finddata.ff_name, finddata.ff_attrib, musthave, canthave)) {
			sprintf (findpath, "%s/%s", findbase, finddata.ff_name);
			return findpath;
		}
	}

	return NULL;
}

void Sys_FindClose (void)
{
	findhandle = -1;
}
