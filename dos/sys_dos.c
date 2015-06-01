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

#include "dosisms.h"
#include "../qcommon/qcommon.h"
#include "../client/keys.h"
#include "errno.h"

// FS: TODO FIXME FIXME PLEASE PLEASE.  >> Take DOS_V2.C from Quake1 and SYS_DOS.C from Quake1.  Clean up this crap :S
int _crt0_startup_flags = _CRT0_FLAG_UNIX_SBRK; // FS: Fake Mem Fix (QIP)
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

int	curtime;
unsigned	sys_msg_time;
unsigned	sys_frame_time;

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
	dos_restoreintr(9); // FS: Give back the keyboard
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


// FS: Doesn't work
char *Sys_ConsoleInput (void)
{
	static char     text[256];
	static int      len = 0;
	char            ch;

	if (!dedicated || !dedicated->value)
		return NULL;

	if (! kbhit())
		return NULL;

	ch = getche();

	switch (ch)
	{
		case '\r':
			putch('\n');
			if (len)
			{
				text[len] = 0;
				len = 0;
				return text;
			}
			break;

		case '\b':
			putch(' ');
			if (len)
			{
				len--;
				putch('\b');
			}
			break;

		default:
			text[len] = ch;
			len = (len + 1) & 0xff;
			break;
	}

	return NULL;
}

void	Sys_ConsoleOutput (char *string)
{
//printf("Sys_ConsoleOutput: %s",string);
//		printf("%s",string);
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
		sys_msg_time = Sys_Milliseconds();
//		sys_frame_time = Sys_Milliseconds();
		k = keybuf[keybuf_tail++];
		keybuf_tail &= (KEYBUF_SIZE-1);

		if (k==0xe0)
			continue;               // special / pause keys
		next = keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)];
		if (next == 0xe1)
			continue;                               // pause key bullshit
		if (k==0xc5 && next == 0x9d) 
		{ 
			Key_Event (K_PAUSE, true, sys_msg_time); // FS: FIXME is Sys_Milliseconds() right?
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
		{
			Key_Event (outkey, false, sys_msg_time); // FS: FIXME is Sys_Milliseconds() right?
		}
		else
		{
			Key_Event (outkey, true, sys_msg_time); // FS: FIXME is Sys_Milliseconds() right?
		}
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
#if 1
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
#else // FS: Busted :(
	curtime = (int)uclock() / (int)UCLOCKS_PER_SEC;
	return curtime;  //FS: Accurate Clock (QIP)
#endif
}

void	Sys_Mkdir (char *path)
{
	mkdir (path, 0777);
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

//=============================================================================

int main (int argc, char **argv)
{
	int	time, oldtime, newtime;

	Sys_Init();
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
		Qcommon_Frame (time);
		sys_frame_time = newtime; // FS: Need to update this for input to work properly
		oldtime = newtime;
	}
	return oldtime; // FS: Compiler warning
}


