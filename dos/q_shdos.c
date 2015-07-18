
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <dos.h>
#include <dirent.h>
#include <io.h>
#include <sys/time.h>

#include "../qcommon/qcommon.h"
#include "glob.h"

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

static	DIR		*fdir;
static	char	findbase[MAX_OSPATH];
static	char	findpath[MAX_OSPATH];
static	char	findpattern[MAX_OSPATH];

static qboolean CompareAttributes(char *path, char *name,
				  unsigned musthave, unsigned canthave)
{
	int attr;
	char fn[MAX_OSPATH];

	/* . and .. never match */
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
		return false;

	return true;

	sprintf (fn, "%s/%s", path, name);
	if ((attr = _chmod(fn, 0)) == -1) /* shouldn't happen */
		return false;

	if ((attr & _A_SUBDIR) && (canthave & SFF_SUBDIR))
		return false;

	if ((musthave & SFF_SUBDIR) && !(attr & _A_SUBDIR))
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave)
{
	struct dirent *d;
	char *p;

	if (fdir)
		Sys_Error ("Sys_BeginFind without close");

	strcpy(findbase, path);
	if ((p = strrchr(findbase, '/')) != NULL) {
		*p = 0;
		strcpy(findpattern, p + 1);
	} else
		strcpy(findpattern, "*");

	if (strcmp(findpattern, "*.*") == 0)
		strcpy(findpattern, "*");
	
	if ((fdir = opendir(findbase)) == NULL)
		return NULL;
	while ((d = readdir(fdir)) != NULL) {
		if (!*findpattern || glob_match(findpattern, d->d_name)) {
//			if (*findpattern)
//				printf("%s matched %s\n", findpattern, d->d_name);
			if (CompareAttributes(findbase, d->d_name, musthave, canthave)) {
				sprintf (findpath, "%s/%s", findbase, d->d_name);
				return findpath;
			}
		}
	}
	return NULL;
}

char *Sys_FindNext (unsigned musthave, unsigned canthave)
{
	struct dirent *d;

	if (fdir == NULL)
		return NULL;
	while ((d = readdir(fdir)) != NULL) {
		if (!*findpattern || glob_match(findpattern, d->d_name)) {
//			if (*findpattern)
//				printf("%s matched %s\n", findpattern, d->d_name);
			if (CompareAttributes(findbase, d->d_name, musthave, canthave)) {
				sprintf (findpath, "%s/%s", findbase, d->d_name);
				return findpath;
			}
		}
	}
	return NULL;
}

void Sys_FindClose (void)
{
	if (fdir != NULL)
		closedir(fdir);
	fdir = NULL;
}
