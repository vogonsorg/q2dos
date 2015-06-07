// sys_null.h -- null system driver to aid porting efforts

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "../qcommon/qcommon.h"
#include "../client/keys.h"
#include "errno.h"

static qboolean CompareAttributes(char *path, char *name,
        unsigned musthave, unsigned canthave );

// FS: TODO FIXME FIXME PLEASE PLEASE.  >> Take DOS_V2.C from Quake1 and SYS_DOS.C from Quake1.  Clean up this crap :S
#define KEYBUF_SIZE     256
static unsigned char    keybuf[KEYBUF_SIZE];
static int                              keybuf_head=0;
static int                              keybuf_tail=0;

byte        scantokey[128] = 
					{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6', 
	'7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
	'q',    'w',    'e',    'r',    't',    'y',    'u',    'i', 
	'o',    'p',    '[',    ']',    13 ,    K_CTRL,'a',  's',      // 1 
	'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';', 
	'\'' ,    '`',    K_SHIFT,'\\',  'z',    'x',    'c',    'v',      // 2 
	'b',    'n',    'm',    ',',    '.',    '/',    K_SHIFT,'*', 
	K_ALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10,0  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'-',K_LEFTARROW,'5',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
					}; 

byte        shiftscantokey[128] = 
					{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '!',    '@',    '#',    '$',    '%',    '^', 
	'&',    '*',    '(',    ')',    '_',    '+',    K_BACKSPACE, 9, // 0 
	'Q',    'W',    'E',    'R',    'T',    'Y',    'U',    'I', 
	'O',    'P',    '{',    '}',    13 ,    K_CTRL,'A',  'S',      // 1 
	'D',    'F',    'G',    'H',    'J',    'K',    'L',    ':', 
	'"' ,    '~',    K_SHIFT,'|',  'Z',    'X',    'C',    'V',      // 2 
	'B',    'N',    'M',    '<',    '>',    '?',    K_SHIFT,'*', 
	K_ALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10,0  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'_',K_LEFTARROW,'%',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
					}; 

int dos_inportb(int port)
{
}

int dos_inportw(int port)
{
}

void dos_outportb(int port, int val)
{
}

void dos_outportw(int port, int val)
{
}

void dos_irqenable(void)
{
}

void dos_irqdisable(void)
{
}

void TrapKey(void)
{
//      static int ctrl=0;
	keybuf[keybuf_head] = dos_inportb(0x60);
	dos_outportb(0x20, 0x20);
	/*
	if (scantokey[keybuf[keybuf_head]&0x7f] == K_CTRL)
		ctrl=keybuf[keybuf_head]&0x80;
	if (ctrl && scantokey[keybuf[keybuf_head]&0x7f] == 'c')
		Sys_Error("ctrl-c hit\n");
	*/
	keybuf_head = (keybuf_head + 1) & (KEYBUF_SIZE-1);
}

#define SC_UPARROW              0x48
#define SC_DOWNARROW    0x50
#define SC_LEFTARROW            0x4b
#define SC_RIGHTARROW   0x4d
#define SC_LEFTSHIFT   0x2a
#define SC_RIGHTSHIFT   0x36
#define SC_RIGHTARROW   0x4d

double	curtime;
//unsigned	sys_msg_time;
unsigned	sys_frame_time;


void Sys_mkdir (char *path)
{
}

void Sys_Error (char *error, ...)
{
	va_list		argptr;

	printf ("Sys_Error: ");	
	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");
#if 0
{	//we crash here so we can get a backtrace.  Yes it is ugly, and no this should never be in production!
	int j,k;
fflush(stdout);
	j=0;
	k=5/j;	//divide by zero!
}
#endif
	exit (1);
}

void Sys_Quit (void)
{
	exit (0);
}

void	Sys_UnloadGame (void)
{
}

#ifdef GAME_HARD_LINKED
void *GetGameAPI (void *import);

void	*Sys_GetGameAPI (void *parms)
{
	return GetGameAPI (parms);
}
// needs to be statically linked for null
// otherwise it sits here to satisfy the linker AFIK
#else
void	*Sys_GetGameAPI (void *parms)
{
	return NULL;
}
#endif	


char *Sys_ConsoleInput (void)
{
    static char myText[256];
    int     myLength;
    fd_set	myFDSet;
    struct timeval	myTimeOut;

    FD_ZERO (&myFDSet);
    FD_SET (0, &myFDSet);
    myTimeOut.tv_sec = 0;
    myTimeOut.tv_usec = 0;
    if (select (1, &myFDSet, NULL, NULL, &myTimeOut) == -1 || !FD_ISSET (0, &myFDSet))
    {
        return (NULL);
    }

    myLength = read (0, myText, sizeof (myText));
    if (myLength == 0)
    {
        //stdin_active = false;
        return (NULL);
    }

    if (myLength < 1)
    {
        return (NULL);
    }
    myText[myLength - 1] = 0x00;

    return (myText);
}

void	Sys_ConsoleOutput (char *string)
{
//printf("Sys_ConsoleOutput: %s",string);
printf("%s",string);
}

#define SC_RSHIFT       0x36 
#define SC_LSHIFT       0x2a 
void Sys_SendKeyEvents (void)
{
	int k, next;
	int outkey;

// get key events

	while (keybuf_head != keybuf_tail)
	{

		k = keybuf[keybuf_tail++];
		keybuf_tail &= (KEYBUF_SIZE-1);

		if (k==0xe0)
			continue;               // special / pause keys
		next = keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)];
		if (next == 0xe1)
			continue;                               // pause key bullshit
		if (k==0xc5 && next == 0x9d) 
		{ 
			Key_Event (K_PAUSE, true, Sys_Milliseconds()); // FS: FIXME is Sys_Milliseconds() right?
			continue; 
		} 

		// extended keyboard shift key bullshit 
		if ( (k&0x7f)==SC_LSHIFT || (k&0x7f)==SC_RSHIFT ) 
		{ 
			if ( keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)]==0xe0 ) 
				continue; 
			k &= 0x80; 
			k |= SC_RSHIFT; 
		} 

		if (k==0xc5 && keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)] == 0x9d)
			continue; // more pause bullshit

		outkey = scantokey[k & 0x7f];

		if (k & 0x80)
			Key_Event (outkey, false, Sys_Milliseconds()); // FS: FIXME is Sys_Milliseconds() right?
		else
			Key_Event (outkey, true, Sys_Milliseconds()); // FS: FIXME is Sys_Milliseconds() right?

	}
}

void Sys_AppActivate (void)
{
}

void Sys_CopyProtect (void)
{
}

char *Sys_GetClipboardData( void )
{
	return NULL;
}

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
		Sys_Error ("VirtualAlloc reserve failed");
	memset (membase, 0, maxsize);
	return (void *)membase;

}

void	*Hunk_Alloc (int size)
{
	void	*buf;

	// round to cacheline
	size = (size+31)&~31;

	cursize += size;
	if (cursize > hunkmaxsize)
		Sys_Error ("Hunk_Alloc overflow");

	return (void *)(membase+cursize-size);
}

void	Hunk_Free (void *buf)
{
	free (buf);
	hunkcount--;
}

int		Hunk_End (void)
{
	hunkcount++;
//Com_Printf ("hunkcount: %i\n", hunkcount);
	return cursize;
}

double		Sys_Milliseconds (void)
{
	struct timeval tp;
	struct timezone tzp;
	static int		secbase;

	gettimeofday(&tp, &tzp);
	
	if (!secbase)
	{
		secbase = tp.tv_sec;
		return tp.tv_usec/1000;
	}

	curtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;
	
	return curtime;
}

void	Sys_Mkdir (char *path)
{
    mkdir (path, 0777);
}

char *strlwr (char *s)
{
        while (*s) {
                *s = tolower(*s);
                s++;
        }
}


//============================================

static  char    findbase[MAX_OSPATH];
static  char    findpath[MAX_OSPATH];
static  char    findpattern[MAX_OSPATH];
static  DIR             *fdir;



char *Sys_FindFirst (char *path, unsigned musthave, unsigned canhave)
{
        struct dirent *d;
        char *p;

        if (fdir)
                Sys_Error ("Sys_BeginFind without close");

//      COM_FilePath (path, findbase);
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
//                      if (*findpattern)
//                              printf("%s matched %s\n", findpattern, d->d_name);
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
//                      if (*findpattern)
//                              printf("%s matched %s\n", findpattern, d->d_name);
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

void	Sys_Init (void)
{
}
static struct handlerhistory_s
{
} handlerhistory[4];

static int handlercount=0;

void	dos_registerintr(int intr, void (*handler)(void))
{
}

void	dos_restoreintr(int intr)
{
}


//=============================================================================

void main (int argc, char **argv)
{
	int				time, oldtime, newtime;

	Qcommon_Init (argc, argv);
	oldtime = Sys_Milliseconds ();
	dos_registerintr(9, TrapKey); // FS: FIXME FREE THIS WHEN CLOSED

    /* main window message loop */
	while (1)
	{
		do
		{
			newtime = Sys_Milliseconds ();
			time = newtime - oldtime;
		} while (time < 1);
//			Con_Printf ("time:%5.2f - %5.2f = %5.2f\n", newtime, oldtime, time);

		//	_controlfp( ~( _EM_ZERODIVIDE /*| _EM_INVALID*/ ), _MCW_EM );
//		_controlfp( _PC_24, _MCW_PC ); // FS: Win32 only maybe?  Can't test this
		Qcommon_Frame (time);

		oldtime = newtime;
	}
}

static qboolean CompareAttributes(char *path, char *name,
        unsigned musthave, unsigned canthave )
{
        struct stat st;
        char fn[MAX_OSPATH];

// . and .. never match
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
                return false;

        sprintf(fn, "%s/%s", path, name);
        if (stat(fn, &st) == -1)
                return false; // shouldn't happen

        if ( ( st.st_mode & S_IFDIR ) && ( canthave & SFF_SUBDIR ) )
                return false;

        if ( ( musthave & SFF_SUBDIR ) && !( st.st_mode & S_IFDIR ) )
                return false;

        return true;
}

int     Sys_LinuxTime (void) // FS: DOS needs this for random qport
{
        int linuxtime;
        struct timeval tp;
        struct timezone tzp;
        static int              secbase;

        gettimeofday(&tp, &tzp);

        if (!secbase)
        {
                secbase = tp.tv_sec;
                return tp.tv_usec/1000;
        }

        linuxtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;

        return linuxtime;
}

