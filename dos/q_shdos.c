
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/nearptr.h>
#include <dpmi.h>
#include <conio.h>
#include <bios.h>
#include <crt0.h> // FS: Fake Mem Fix (QIP)

#include "dosisms.h"
#include "../qcommon/qcommon.h"
#include "../client/keys.h"
#include "errno.h"
#include "glob.h"

int		hunkcount;
byte	*membase;
int		hunkmaxsize;
int		cursize;

void	*Hunk_Begin (int maxsize)
{
	// reserve a huge chunk of memory, but don't commit any yet
	cursize = 0;
	hunkmaxsize = maxsize;
	membase = malloc (maxsize);

	if (!membase)
	{
		Sys_Error ("VirtualAlloc reserve failed %d bytes",maxsize);
	}

	memset (membase, 0, maxsize);
	return (void *)membase;

}

void	*Hunk_Alloc (int size)
{
//	void	*buf;

	// round to cacheline
	size = (size+31)&~31;

	cursize += size;
	if (cursize > hunkmaxsize)
	{
		Sys_Error ("Hunk_Alloc overflow");
	}

	return (void *)(membase+cursize-size);
}

void	Hunk_Free (void *buf)
{
	free (buf);
	hunkcount--;
}

int	Hunk_End (void)
{
	hunkcount++;
	return cursize;
}

double	curtime;
double	Sys_Milliseconds (void)
{
	// FS: Needed *1000 to work properly
	curtime = (double) uclock() / (double) UCLOCKS_PER_SEC*1000; // FS: Win9X/Fast PC Fix (QIP)

	return curtime;
}

int	Sys_LinuxTime (void) // FS: DOS needs this for random qport
{
	int linuxtime;
	struct timeval tp;
	struct timezone tzp;
	static int		secbase;

	gettimeofday(&tp, &tzp);
	
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
	unsigned musthave, unsigned canthave )
{
	struct stat st;
	char fn[MAX_OSPATH];

// . and .. never match
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
		return false;

	return true;

	if (stat(fn, &st) == -1)
		return false; // shouldn't happen

	if ( ( st.st_mode & S_IFDIR ) && ( canthave & SFF_SUBDIR ) )
		return false;

	if ( ( musthave & SFF_SUBDIR ) && !( st.st_mode & S_IFDIR ) )
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canhave)
{
	struct dirent *d;
	char *p;

	if (fdir)
		Sys_Error ("Sys_BeginFind without close");

//	COM_FilePath (path, findbase);
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
			if (CompareAttributes(findbase, d->d_name, musthave, canhave)) {
				sprintf (findpath, "%s/%s", findbase, d->d_name);
				return findpath;
			}
		}
	}
	return NULL;
}

char *Sys_FindNext (unsigned musthave, unsigned canhave)
{
	struct dirent *d;

	if (fdir == NULL)
		return NULL;
	while ((d = readdir(fdir)) != NULL) {
		if (!*findpattern || glob_match(findpattern, d->d_name)) {
//			if (*findpattern)
//				printf("%s matched %s\n", findpattern, d->d_name);
			if (CompareAttributes(findbase, d->d_name, musthave, canhave)) {
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
