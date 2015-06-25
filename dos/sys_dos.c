// sys_dos.c -- dos system driver

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dir.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dpmi.h>
#include <sys/nearptr.h>
#include <conio.h>
#include <crt0.h> // FS: Fake Mem Fix (QIP)

int _crt0_startup_flags = _CRT0_FLAG_UNIX_SBRK; // FS: Fake Mem Fix (QIP)

#include "zone.h"
#include "dosisms.h"
#include "../qcommon/qcommon.h"
#include "../client/keys.h"
#include "errno.h"
#include "glob.h"

#define MINIMUM_WIN_MEMORY                      0x800000
#define MINIMUM_WIN_MEMORY_LEVELPAK     (MINIMUM_WIN_MEMORY + 0x100000)

#define KEYBUF_SIZE     256
static unsigned char    keybuf[KEYBUF_SIZE];
static int                              keybuf_head=0;
static int                              keybuf_tail=0;

#ifdef USE_QDOS_ZONE
static quakeparms_t     quakeparms;
#endif
extern char     start_of_memory __asm__("start");

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

#define STDOUT 1
double	sys_msg_time;
double	sys_frame_time;
int	sys_checksum;

void MaskExceptions (void);
void Sys_PushFPCW_SetHigh (void);
void Sys_PopFPCW (void);
double Sys_FloatTime (void);

#define LEAVE_FOR_CACHE (512*1024)              //FIXME: tune
#define LOCKED_FOR_MALLOC (128*1024)    //FIXME: tune


int                     end_of_memory;
qboolean        lockmem, lockunlockmem, unlockmem;
static int      win95;
static int                      minmem;

// FS: Q2 needs it badly
// FS: See http://www.delorie.com/djgpp/doc/libc/libc_380.html for more information
void Sys_DetectLFN (void)
{
	unsigned int fd = _get_volume_info (NULL, 0, 0, NULL);

	if(!(fd & _FILESYS_LFN_SUPPORTED))
	{
		printf("WARNING: Long file name support not detected!  Grab a copy of DOSLFN!\n");
		sleep(2);
		printf("Continuing to load Quake II. . .\n");
	}
}

void Sys_DetectWin95 (void)
{
	__dpmi_regs                             r;

	r.x.ax = 0x160a;                /* Get Windows Version */
	__dpmi_int(0x2f, &r);

	if(r.x.ax || r.h.bh < 4)        /* Not windows or earlier than Win95 */
	{
		win95 = 0;
		lockmem = true;
		lockunlockmem = false;
		unlockmem = true;
	}
	else
	{
		Sys_Error("Microsoft Windows detected.  You must run Q2DOS in MS-DOS.\n"); // FS: Warning.  Too many issues in Win9x and even XP.  QDOS is the same way.  So forget it.
		win95 = 1;
		lockunlockmem = COM_CheckParm ("-winlockunlock");

		if (lockunlockmem)
			lockmem = true;
		else
			lockmem = COM_CheckParm ("-winlock");

		unlockmem = lockmem && !lockunlockmem;
	}
}


void *dos_getmaxlockedmem(int *size)
{
	__dpmi_free_mem_info    meminfo;
	__dpmi_meminfo                  info;
	int                                             working_size;
	void                                    *working_memory;
	int                                             last_locked;
        //int                                             extra,  i, j, allocsize;
	int                                     i, j, extra, allocsize; // FS: 2GB Fix
	static char                             *msg = "Locking data...";
	// int                                             m, n;
	byte                                    *x;
	unsigned long   ul; // FS: 2GB Fix

// first lock all the current executing image so the locked count will
// be accurate.  It doesn't hurt to lock the memory multiple times
	last_locked = __djgpp_selector_limit + 1;
	info.size = last_locked - 4096;
	info.address = __djgpp_base_address + 4096;

	if (lockmem)
	{
		if(__dpmi_lock_linear_region(&info))
		{
			Sys_Error ("Lock of current memory at 0x%lx for %ldKb failed!\n",
						info.address, info.size/1024);
		}
	}

	__dpmi_get_free_memory_information(&meminfo);

	if (!win95)             /* Not windows or earlier than Win95 */
	{
		//working_size = meminfo.maximum_locked_page_allocation_in_pages * 4096;
		ul = meminfo.maximum_locked_page_allocation_in_pages * 4096; // FS: 2GB fix
	}
	else
	{
//                working_size = meminfo.largest_available_free_block_in_bytes -
//                                LEAVE_FOR_CACHE;
		ul = meminfo.largest_available_free_block_in_bytes -
		LEAVE_FOR_CACHE; // FS: 2GB fix
	}

	if (ul > 0x7fffffff)
		ul = 0x7fffffff; /* limit to 2GB */
	working_size = (int) ul;
	working_size &= ~0xffff;                /* Round down to 64K */
	working_size += 0x10000;

	do
	{
		working_size -= 0x10000;                /* Decrease 64K and try again */
		working_memory = sbrk(working_size);
	} while (working_memory == (void *)-1);

	extra = 0xfffc - ((unsigned)sbrk(0) & 0xffff);

	if (extra > 0)
	{
		sbrk(extra);
		working_size += extra;
	}

// now grab the memory
	info.address = last_locked + __djgpp_base_address;

	if (!win95)
	{
	    info.size = __djgpp_selector_limit + 1 - last_locked;

		while (info.size > 0 && __dpmi_lock_linear_region(&info))
		{
			info.size -= 0x1000;
			working_size -= 0x1000;
			sbrk(-0x1000);
		}
	}
	else
	{                       /* Win95 section */
		j = COM_CheckParm("-winmem");

		minmem = MINIMUM_WIN_MEMORY;

		if (j)
		{
			allocsize = ((int)(Q_atoi(com_argv[j+1]))) * 0x100000 +
					LOCKED_FOR_MALLOC;

			if (allocsize < (minmem + LOCKED_FOR_MALLOC))
				allocsize = minmem + LOCKED_FOR_MALLOC;
		}
		else
		{
			allocsize = minmem + LOCKED_FOR_MALLOC;
		}

		if (!lockmem)
		{
		// we won't lock, just sbrk the memory
			info.size = allocsize;
			goto UpdateSbrk;
		}

		// lock the memory down
		write (STDOUT, msg, strlen (msg));

		for (j=allocsize ; j>(minmem + LOCKED_FOR_MALLOC) ;
			 j -= 0x100000)
		{
			info.size = j;
	
			if (!__dpmi_lock_linear_region(&info))
				goto Locked;
	
			write (STDOUT, ".", 1);
		}

	// finally, try with the absolute minimum amount
		for (i=0 ; i<10 ; i++)
		{
			info.size = minmem + LOCKED_FOR_MALLOC;

			if (!__dpmi_lock_linear_region(&info))
				goto Locked;
		}

                Sys_Error ("Can't lock memory; %ld Mb lockable RAM required. "
				   "Try shrinking smartdrv.", info.size / 0x100000);

Locked:

UpdateSbrk:

		info.address += info.size;
		info.address -= __djgpp_base_address + 4; // ending point, malloc align
		working_size = info.address - (int)working_memory;
		sbrk(info.address-(int)sbrk(0));                // negative adjustment
	}


	if (lockunlockmem)
	{
		__dpmi_unlock_linear_region (&info);
		printf ("Locked and unlocked %d Mb data\n", working_size / 0x100000);
	}
	else if (lockmem)
	{
		printf ("Locked %d Mb data\n", working_size / 0x100000);
	}
	else
	{
		printf ("Allocated %d Mb data\n", working_size / 0x100000);
	}

// touch all the memory to make sure it's there. The 16-page skip is to
// keep Win 95 from thinking we're trying to page ourselves in (we are
// doing that, of course, but there's no reason we shouldn't)
	x = (byte *)working_memory;

	for (j=0 ; j<4 ; j++) // FS: 2GB Fix
	{
		for (i=0 ; i<(working_size - 16 * 0x1000) ; i += 4)
		{
			sys_checksum += *(int *)&x[i];
			sys_checksum += *(int *)&x[i + 16 * 0x1000];
		}
	}

// give some of what we locked back for malloc before returning.  Done
// by cheating and passing a negative value to sbrk
	working_size -= LOCKED_FOR_MALLOC;
	sbrk( -(LOCKED_FOR_MALLOC));
	*size = working_size;
	return working_memory;
}

int virtualmemsize;
byte *virtualmembase;

void Sys_GetMemory(void)
{
// FS: TODO FIXME FIXME doesn't have quakeparms.memsize or anything that I see
//	virtualmembase = malloc (virtualmemsize);
//	virtualmemsize = (int) 4096 * 1024 * 1024;
//	virtualmembase = dos_getmaxlockedmem(&virtualmemsize); // FS: Lock Virtual memory?
#ifdef USE_QDOS_ZONE
	int             j, tsize;

	j = COM_CheckParm("-mem");
	if (j)
	{
		quakeparms.memsize = (int) (atof(com_argv[j+1]) * 1024 * 1024); // FS: FIXME USE Q_ATOF
		quakeparms.membase = malloc (quakeparms.memsize);
	}
	else
	{
		int j;
		//quakeparms.membase = dos_getmaxlockedmem (&quakeparms.memsize);

		j=32; // FS: from QW
		quakeparms.memsize = (int) j * 1024 * 1024; 
		quakeparms.membase = malloc (quakeparms.memsize);
	}

	fprintf(stderr, "malloc'd: %d\n", quakeparms.memsize);

	if (COM_CheckParm ("-noclear")) // FS: Wanted the option
	{
		return;
	}
	else
	{
		printf("Clearing allocated memory...\n");
		memset(quakeparms.membase,0x0,quakeparms.memsize); // JASON: Clear memory on startup
		printf("Done!  Continuing to load Quake 2.\n");
	}

	if (COM_CheckParm ("-heapsize"))
	{
		tsize = Q_atoi (com_argv[COM_CheckParm("-heapsize") + 1]) * 1024;

		if (tsize < quakeparms.memsize)
			quakeparms.memsize = tsize;
	}
#endif // USE_QDOS_ZONE
}

int Sys_Get_Physical_Memory(void) // FS: From DJGPP tutorial
{
	_go32_dpmi_meminfo info;
	_go32_dpmi_get_free_memory_information(&info);

	if (info.available_physical_pages != -1)
	{
		return info.available_physical_pages * 4096;
	}
	return info.available_memory;
}

void Sys_Memory_Stats_f (void)
{
	Com_Printf("%d Mb available for Q2DOS.\n", (Sys_Get_Physical_Memory() / 0x100000) ); // FS: Added
	Com_Printf("%lu Virtual Mb available for Q2DOS.\n", (_go32_dpmi_remaining_virtual_memory() / 0x100000) ); // FS: Added
}
/*
================
Sys_PageInProgram

walks the text, data, and bss to make sure it's all paged in so that the
actual physical memory detected by Sys_GetMemory is correct.
================
*/
void Sys_PageInProgram(void)
{
	int             i, j;

	end_of_memory = (int)sbrk(0);

	if (lockmem)
	{
		if (dos_lockmem ((void *)&start_of_memory,
						 end_of_memory - (int)&start_of_memory))
			Sys_Error ("Couldn't lock text and data");
	}

	if (lockunlockmem)
	{
		dos_unlockmem((void *)&start_of_memory,
						 end_of_memory - (int)&start_of_memory);
		printf ("Locked and unlocked %d Mb image\n",
				(end_of_memory - (int)&start_of_memory) / 0x100000);
	}
	else if (lockmem)
	{
		printf ("Locked %d Mb image\n",
				(end_of_memory - (int)&start_of_memory) / 0x100000);
	}
	else
	{
		printf ("Loaded %d Mb image\n",
				(end_of_memory - (int)&start_of_memory) / 0x100000);
	}

// touch the entire image, doing the 16-page skip so Win95 doesn't think we're
// trying to page ourselves in
	for (j=0 ; j<4 ; j++)
	{
		for(i=(int)&start_of_memory ; i<(end_of_memory - 16 * 0x1000) ; i += 4)
		{
			sys_checksum += *(int *)i;
			sys_checksum += *(int *)(i + 16 * 0x1000);
		}
	}
	
	printf("%d Mb available for Q2DOS.\n", (Sys_Get_Physical_Memory() / 0x100000) ); // FS: Added
	printf("%lu Virtual Mb available for Q2DOS.\n", (_go32_dpmi_remaining_virtual_memory() / 0x100000) ); // FS: Added
}

void Sys_Error (char *error, ...)
{
	va_list		argptr;

   {
      __dpmi_regs r;

      r.x.ax = 3;
      __dpmi_int(0x10, &r);
   }	//return to text mode

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
	__dpmi_regs r;

	if(!dedicated || !dedicated->value)
		dos_restoreintr(9); // FS: Give back the keyboard

	if (unlockmem)
	{
		dos_unlockmem (&start_of_memory,
					   end_of_memory - (int)&start_of_memory);
#ifdef USE_QDOS_ZONE
		dos_unlockmem (quakeparms.membase, quakeparms.memsize);
#endif
	}
	//return to text mode

	r.x.ax = 3;
	__dpmi_int(0x10, &r);
	exit (0);
}

void *GetGameAPI (void *import);
#ifdef GAME_HARD_LINKED
void	Sys_UnloadGame (void)
{
}

//void *GetGameAPI (void *import);

void	*Sys_GetGameAPI (void *parms)
{
	return GetGameAPI (parms);
}
// needs to be statically linked for null
// otherwise it sits here to satisfy the linker AFIK
#else

//Unload the DLL
#include <dlfcn.h>
#include <sys/dxe.h>
#include <assert.h>
#include <ctype.h>

// FS: The following is gross, but I just figured this out.
extern void vectoangles2 (vec3_t value1, vec3_t angles);
DXE_EXPORT_TABLE (syms)
  DXE_EXPORT (printf)
  DXE_EXPORT (sprintf)
  DXE_EXPORT (tan)
  DXE_EXPORT (free)
  DXE_EXPORT (strcat)
  DXE_EXPORT (strcmp)
  DXE_EXPORT (rand)
  DXE_EXPORT (ceil)
  DXE_EXPORT (_doprnt)
  DXE_EXPORT (fwrite)
  DXE_EXPORT (strncpy)
  DXE_EXPORT (atoi)
  DXE_EXPORT (memcpy)
  DXE_EXPORT (memset)
  DXE_EXPORT (fprintf)
  DXE_EXPORT (strtol)
  DXE_EXPORT (strdup)
  DXE_EXPORT (strcasecmp)
  DXE_EXPORT (fread)
  DXE_EXPORT (fopen)
  DXE_EXPORT (time)
  DXE_EXPORT (strtod)
  DXE_EXPORT (strchr)
  DXE_EXPORT (fclose)
  DXE_EXPORT (sscanf)
  DXE_EXPORT (memcmp)
  DXE_EXPORT (sqrt)
  DXE_EXPORT (strtok)
  DXE_EXPORT (localtime)
  DXE_EXPORT (cos)
  DXE_EXPORT (sin)
  DXE_EXPORT (strlen)
  DXE_EXPORT (errno)
  DXE_EXPORT (atan2)
  DXE_EXPORT (qsort)
  DXE_EXPORT (floor)
  DXE_EXPORT (strstr)
  DXE_EXPORT (strcpy)
  DXE_EXPORT (__dj_ctype_tolower)
  DXE_EXPORT (__dj_ctype_toupper)
  DXE_EXPORT (__dj_assert)
  DXE_EXPORT (atof)
  DXE_EXPORT (atan)
  DXE_EXPORT (asin)
  DXE_EXPORT (vectoangles2)
  DXE_EXPORT (crand)
  DXE_EXPORT (stpcpy)
DXE_EXPORT_END
static void (*game_library)(void);

void    Sys_UnloadGame (void)
{
	if (game_library)
	{
		dlclose (game_library);
	}
	game_library = NULL;
}

static int lastresort ()
{
	printf ("last resort function called!\n");
	return 0;
}

void *dxe_res (const char *symname)
{
	printf ("%s: undefined symbol in dynamic module.  Please report this as a bug!\n", symname);
	Com_Printf ("%s: undefined symbol in dynamic module.  Please report this as a bug!\n", symname);

	union
	{
		int (*from)(void);
		void *to;
	} func_ptr_cast;

	func_ptr_cast.from = lastresort;
	return func_ptr_cast.to;
}

void *Sys_GetGameAPI (void *parms)
{
	void	*(*GetGameAPI) (void *);
	char	name[MAX_OSPATH];
	char	curpath[MAX_OSPATH];
	char	*path;
	const char *gamename = "gamex86.dx3";

	getcwd(curpath, sizeof(curpath));

	Com_Printf("------- Loading %s -------\n", gamename);

	  // Set the error callback function
	_dlsymresolver = dxe_res;

	// Register the symbols exported into dynamic modules
	dlregsym (syms);

	// now run through the search paths
	path = NULL;

	while (1)
	{
		path = FS_NextPath (path);
		if (!path)
			return NULL;		// couldn't find one anywhere
		sprintf (name, "%s/%s/%s", curpath, path, gamename);
		game_library = dlopen (name, RTLD_LAZY);
		if (game_library)
		{
			Com_Printf ("LoadLibrary (%s)\n",name);
			break;
		}
	}

#if 0 // FS: Me testing it out.
	if(game_library)
	{
		void (*Test_DXE)(void);

		dlerror();
		*(void **) (&Test_DXE) = dlsym(game_library, "_Test_DXE");
//		void *Test_DXE = (void*)dlsym(game_library, "Test_DXE");
		if(!Test_DXE)
			Com_Printf("Can't find Test_DXE!");
		else
			Com_Printf("Found Test_DXE!\n");
	}
#endif

	*(void **) (&GetGameAPI) = dlsym (game_library, "_GetGameAPI");

	if (!GetGameAPI)
	{
		Sys_UnloadGame ();		
		return NULL;
	}
	else
	{
//		Com_Printf("it loaded\n");
	}

	return GetGameAPI (parms);
}

#endif	


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
	if (!dedicated || !dedicated->value)
		return;
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
		sys_msg_time = Sys_Milliseconds();//Sys_FloatTime();
		k = keybuf[keybuf_tail++];
		keybuf_tail &= (KEYBUF_SIZE-1);

		if (k==0xe0)
			continue;               // special / pause keys
		next = keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)];
		if (next == 0xe1)
			continue;                               // pause key bullshit
		if (k==0xc5 && next == 0x9d) 
		{ 
			Key_Event (K_PAUSE, true, sys_msg_time);
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
			Key_Event (outkey, false, sys_msg_time);
		}
		else
		{
			Key_Event (outkey, true, sys_msg_time);
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

void	Sys_Init (void)
{
	MaskExceptions ();

	Sys_SetFPCW ();

	_go32_interrupt_stack_size = 4 * 1024;
	_go32_rmcb_stack_size = 4 * 1024;
}

double Sys_FloatTime (void)
{
	return (double) uclock() / (double) UCLOCKS_PER_SEC; // FS: Win9X/Fast PC Fix (QIP)
}

//=============================================================================

int main (int argc, char **argv)
{
	double time, oldtime, newtime;

#ifdef USE_QDOS_ZONE
	COM_InitArgv (argc, argv);

	quakeparms.argc = com_argc;
	quakeparms.argv = com_argv;
#endif
	Sys_DetectLFN();
	Sys_DetectWin95 ();
	Sys_PageInProgram ();
	Sys_GetMemory ();

	Sys_Init();

#ifndef USE_QDOS_ZONE
	Qcommon_Init (argc, argv);
#else
	Qcommon_Init(&quakeparms);
#endif

	oldtime = Sys_Milliseconds ();

	if (!dedicated || !dedicated->value)
		dos_registerintr(9, TrapKey);

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
}

void Sys_MakeCodeWriteable(void)
{
} //MS-DOS is always writeable
