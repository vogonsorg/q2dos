// sys_null.h -- null system driver to aid porting efforts

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

#include "../qcommon/qcommon.h"
#include "../client/keys.h"
#include "errno.h"

#ifdef __DJGPP__
// FS: TODO FIXME FIXME PLEASE PLEASE.  >> Take DOS_V2.C from Quake1 and SYS_DOS.C from Quake1.  Clean up this crap :S
int _crt0_startup_flags = _CRT0_FLAG_UNIX_SBRK; // FS: Fake Mem Fix (QIP)
#endif
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
}

#define SC_UPARROW              0x48
#define SC_DOWNARROW    0x50
#define SC_LEFTARROW            0x4b
#define SC_RIGHTARROW   0x4d
#define SC_LEFTSHIFT   0x2a
#define SC_RIGHTSHIFT   0x36
#define SC_RIGHTARROW   0x4d

int	curtime;
//unsigned	sys_msg_time;
unsigned	sys_frame_time;

#ifndef id386
void Sys_SetFPCW()
{
}
#endif

void Sys_mkdir (char *path)
{
Com_Printf("mkdir %s\n",path);
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

void *GetGameAPI (void *import);
#ifdef GAME_HARD_LINKED

void    Sys_UnloadGame (void)
{
}

void	*Sys_GetGameAPI (void *parms)
{
	return GetGameAPI (parms);
}
// needs to be statically linked for null
// otherwise it sits here to satisfy the linker AFIK
#else
#include <dlmlib.h>

void    Sys_UnloadGame (void)
{
	Com_Printf("Sys_UnloadGame\n");
	UnloadDLM("gamex86.dlm");
}

void	*Sys_GetGameAPI (void *parms)
{
	Com_Printf("Sys_LoadGame\n");
	LoadDLM("gamex86.dlm");
	return GetGameAPI (parms);
}
#endif	

char *Sys_ConsoleInput (void)
{
#ifdef __DJGPP__
        static char     text[1024];
        static int index;
        int     len;
        char c;

        //cin.rdbuf()->setbuf(NULL, 0);//me vomit over a C++ book


    if (!kbhit())
                return NULL;


    c = getche();

        text[index]=c;

        index ++;

        if (c!=13)
                return NULL;

        printf("\n");

        text[index]=0;
        index = 0;

        return text;
#else
	return NULL;
#endif
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
	memset (membase, 0, maxsize);
	if (!membase)
		Sys_Error ("VirtualAlloc reserve failed");
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

int		Sys_Milliseconds (void)
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
	printf("Sys_Mkdir [%s]: UNIMPLEMENTED!\n",path);
}

char	*Sys_FindFirst (char *path, unsigned musthave, unsigned canthave)
{
	printf("Sys_FindFirst [%s]: UNIMPLEMENTED!\n",path);
	return NULL;
}

char	*Sys_FindNext (unsigned musthave, unsigned canthave)
{
	return NULL;
}

void	Sys_FindClose (void)
{
}

void	Sys_Init (void)
{
}
static struct handlerhistory_s
{
	int intr;
	_go32_dpmi_seginfo pm_oldvec;
} handlerhistory[4];

static int handlercount=0;

void	dos_registerintr(int intr, void (*handler)(void))
{

}

void	dos_restoreintr(int intr)
{
}

void Sys_MakeCodeWriteable()
{
}


//=============================================================================

void main (int argc, char **argv)
{
	int				time, oldtime, newtime;
#ifndef GAME_HARD_LINKED
	LoadDLM("libc.dlm");
	LoadDLM("libm.dlm");
#endif

	Qcommon_Init (argc, argv);
	oldtime = Sys_Milliseconds ();

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


