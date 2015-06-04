//
//
// simple test of the Quake 1 VESA code
//
//
// i586-pc-msdosdjgpp-gcc x.c dos/dos_v2.c -o /Users/jsteve/dos/x.exe
// Known issues
//2011-11-02 (SVN):
//vid_ext.c: Fixed stack corruption in dos_getmemory()/seginfo due to the missing balancing dos_freememory() calls in VID_InitExtra() and VID_ExtraGetModeInfo() for failures and unwanted cases. The bug was discovered when h2dos.exe was compiled using gcc-4.5.4.
//dos_v2.c: Add Sys_Error() calls to dos_getmemory() against reaching MAX_SEGINFO and dos_freememory() against uncached seginfo.
//quakedef.h: updated beta date stamp to 2011-11-02.
//
//http://uhexen2.sourceforge.net/changes.html
// 
//This just dumps the first 40 SVGA modes 

#include <stdio.h>
#include <dpmi.h>
#include <pc.h>

#include "../client/client.h"
#include "../client/qmenu.h"
#include "vid_dos.h"
#include "dosisms.h"

#define MAX_RESOLUTIONS 10
struct vid_resolutions_t {
	int mode;
	int vesa_mode;
	int height;
	int width;
	int address;
	char menuname[30];
} vid_resolutions[MAX_RESOLUTIONS];
int num_vid_resolutions=1;	//we always have mode 13

#define MODE_SUPPORTED_IN_HW		0x0001
#define COLOR_MODE			0x0008
#define GRAPHICS_MODE			0x0010
#define VGA_INCOMPATIBLE		0x0020
#define LINEAR_FRAME_BUFFER		0x0080
#define LINEAR_MODE			0x4000

#define MAXWIDTH		1280
#define MAXHEIGHT		1024



vmode_t	*pvidmodes;
static int	totalvidmem;
static byte	*ppal;
int		numvidmodes;


typedef struct {
	int	pages[3];	// either 2 or 3 is valid
	int	vesamode;	// LINEAR_MODE set if linear mode
	void	*plinearmem;	// linear address of start of frame buffer
	qboolean	vga_incompatible;
} vesa_extra_t;


#define MAX_VESA_MODES			40
static vmode_t		vesa_modes[MAX_VESA_MODES] = 
	{{NULL, NULL, "    ********* VESA modes *********    "}};
static char			names[MAX_VESA_MODES][10];

static vesa_extra_t	vesa_extra[MAX_VESA_MODES];



extern regs_t regs;


typedef struct vbeinfoblock_s {
     byte			VbeSignature[4];
     byte			VbeVersion[2];
     byte			OemStringPtr[4];
     byte			Capabilities[4];
     byte			VideoModePtr[4];
     byte			TotalMemory[2];
     byte			OemSoftwareRev[2];
     byte			OemVendorNamePtr[4];
     byte			OemProductNamePtr[4];
     byte			OemProductRevPtr[4];
     byte			Reserved[222];
     byte			OemData[256];
} vbeinfoblock_t;

typedef struct
{
	int modenum;
	int mode_attributes;
	int	winasegment;
	int	winbsegment;
	int	bytes_per_scanline; // bytes per logical scanline (+16)
	int win; // window number (A=0, B=1)
	int win_size; // window size (+6)
	int granularity; // how finely i can set the window in vid mem (+4)
	int width, height; // displayed width and height (+18, +20)
	int bits_per_pixel; // er, better be 8, 15, 16, 24, or 32 (+25)
	int bytes_per_pixel; // er, better be 1, 2, or 4
	int memory_model; // and better be 4 or 6, packed or direct color (+27)
	int num_pages; // number of complete frame buffer pages (+29)
	int red_width; // the # of bits in the red component (+31)
	int red_pos; // the bit position of the red component (+32)
	int green_width; // etc.. (+33)
	int green_pos; // (+34)
	int blue_width; // (+35)
	int blue_pos; // (+36)
	int pptr;
	int	pagesize;
	int	numpages;
} modeinfo_t;

static modeinfo_t modeinfo;

int VID_ExtraInitMode (viddef_t *vid, vmode_t *pcurrentmode);
void VID_ExtraSwapBuffers (viddef_t *vid, vmode_t *pcurrentmode,
	vrect_t *rects);
void VID_SetVESAPalette (viddef_t *lvid, vmode_t *pcurrentmode,
	unsigned char *pal);
void VGA_BankedBeginDirectRect (viddef_t *lvid, struct vmode_s *pcurrentmode,
	int x, int y, byte *pbitmap, int width, int height);
void VGA_BankedEndDirectRect (viddef_t *lvid, struct vmode_s *pcurrentmode,
	int x, int y, int width, int height);
void *VID_ExtraFarToLinear (void *ptr);
qboolean VID_ExtraGetModeInfo(int modenum);
//to make standalone
void Sys_Error (char *error, ...);



/*
================
VID_InitExtra
================
*/
void VID_InitExtra (void)
{
	int		nummodes;
	short		*pmodenums;
	vbeinfoblock_t	*pinfoblock;
	__dpmi_meminfo	phys_mem_info;
	int		x = 0;

	pinfoblock = dos_getmemory(sizeof(vbeinfoblock_t));

	if(!pinfoblock)
		Sys_Error("pinfoblock NULL!\n");

	*(long *)pinfoblock->VbeSignature = 'V' + ('B'<<8) + ('E'<<16) + ('2'<<24);

	// We always have mode 13 VGA
	memset(vid_resolutions,0x0,sizeof(vid_resolutions));
	vid_resolutions[0].mode=0;
	vid_resolutions[0].vesa_mode=-1;
	vid_resolutions[0].height=200;
	vid_resolutions[0].width=320;
	sprintf(vid_resolutions[0].menuname,"[VGA 320x200]");

if(COM_CheckParm("-vgaonly"))
	return;	//test for VGA only

// see if VESA support is available
	regs.x.ax = 0x4f00;
	regs.x.es = ptr2real(pinfoblock) >> 4;
	regs.x.di = ptr2real(pinfoblock) & 0xf;
	dos_int86(0x10);

	if (regs.x.ax != 0x4f)
	{
		dos_freememory(pinfoblock); // FS: from HOT
		return;		// no VESA support
	}

	if (pinfoblock->VbeVersion[1] < 0x02)
	{
		dos_freememory(pinfoblock); // FS: from HOT
		return;		// not VESA 2.0 or greater
	}

	Com_Printf ("VESA 2.0 compliant adapter:\n%s\n",
				VID_ExtraFarToLinear (*(byte **)&pinfoblock->OemStringPtr[0]));

	totalvidmem = *(unsigned short *)&pinfoblock->TotalMemory[0] << 16;

	pmodenums = (short *)
			VID_ExtraFarToLinear (*(byte **)&pinfoblock->VideoModePtr[0]);

// find 8 bit modes until we either run out of space or run out of modes
	nummodes = 0;

	while ((*pmodenums != -1) && (nummodes < MAX_VESA_MODES))
	{
		if (VID_ExtraGetModeInfo (*pmodenums))
		{
			vesa_modes[nummodes].pnext = &vesa_modes[nummodes+1];
			if (modeinfo.width > 999)
			{
				if (modeinfo.height > 999)
				{
					sprintf (&names[nummodes][0], "%4dx%4d", modeinfo.width,
							 modeinfo.height);
					names[nummodes][9] = 0;
				}
				else
				{
					sprintf (&names[nummodes][0], "%4dx%3d", modeinfo.width,
							 modeinfo.height);
					names[nummodes][8] = 0;
				}
			}
			else
			{
				if (modeinfo.height > 999)
				{
					sprintf (&names[nummodes][0], "%3dx%4d", modeinfo.width,
							 modeinfo.height);
					names[nummodes][8] = 0;
				}
				else
				{
					sprintf (&names[nummodes][0], "%3dx%3d", modeinfo.width,
							 modeinfo.height);
					names[nummodes][7] = 0;
				}
			}

			vesa_modes[nummodes].name = &names[nummodes][0];
			vesa_modes[nummodes].width = modeinfo.width;
			vesa_modes[nummodes].height = modeinfo.height;
			vesa_modes[nummodes].aspect =
					((float)modeinfo.height / (float)modeinfo.width) *
					(320.0 / 240.0);
			vesa_modes[nummodes].rowbytes = modeinfo.bytes_per_scanline;
			vesa_modes[nummodes].planar = 0;
			vesa_modes[nummodes].pextradata = &vesa_extra[nummodes];
			//vesa_modes[nummodes].setmode = VID_ExtraInitMode;
			//vesa_modes[nummodes].swapbuffers = VID_ExtraSwapBuffers;
			//vesa_modes[nummodes].setpalette = VID_SetVESAPalette;

			if (modeinfo.mode_attributes & LINEAR_FRAME_BUFFER)
			{
			// add linear bit to mode for linear modes
				vesa_extra[nummodes].vesamode = modeinfo.modenum | LINEAR_MODE;
				vesa_extra[nummodes].pages[0] = 0;
				vesa_extra[nummodes].pages[1] = modeinfo.pagesize;
				vesa_extra[nummodes].pages[2] = modeinfo.pagesize * 2;
				vesa_modes[nummodes].numpages = modeinfo.numpages;

				//vesa_modes[nummodes].begindirectrect = VGA_BeginDirectRect;
				//vesa_modes[nummodes].enddirectrect = VGA_EndDirectRect;

				phys_mem_info.address = (int)modeinfo.pptr;
				phys_mem_info.size = 0x400000;

				if (__dpmi_physical_address_mapping(&phys_mem_info))
					goto NextMode;

				vesa_extra[nummodes].plinearmem =
						 real2ptr (phys_mem_info.address);
			}
			else
			{
			// banked at 0xA0000
				vesa_extra[nummodes].vesamode = modeinfo.modenum;
				vesa_extra[nummodes].pages[0] = 0;
				vesa_extra[nummodes].plinearmem =
						real2ptr(modeinfo.winasegment<<4);

				//vesa_modes[nummodes].begindirectrect = VGA_BankedBeginDirectRect;
				//vesa_modes[nummodes].enddirectrect = VGA_BankedEndDirectRect;
				vesa_extra[nummodes].pages[1] = modeinfo.pagesize;
				vesa_extra[nummodes].pages[2] = modeinfo.pagesize * 2;
				vesa_modes[nummodes].numpages = modeinfo.numpages;
			}

			vesa_extra[nummodes].vga_incompatible =
					modeinfo.mode_attributes & VGA_INCOMPATIBLE;

			nummodes++;
		}
NextMode:
		pmodenums++;
	}

// add the VESA modes at the start of the mode list (if there are any)
	if (nummodes)
	{
		vesa_modes[nummodes-1].pnext = pvidmodes;
		pvidmodes = &vesa_modes[0];
		numvidmodes += nummodes;
		ppal = dos_getmemory(256*4);
	}

	dos_freememory(pinfoblock);
}


/*
================
VID_ExtraFarToLinear
================
*/
void *VID_ExtraFarToLinear (void *ptr)
{
	int		temp;

	temp = (int)ptr;
	return real2ptr(((temp & 0xFFFF0000) >> 12) + (temp & 0xFFFF));
}



/*
================
VID_ExtraGetModeInfo
================
*/
qboolean VID_ExtraGetModeInfo(int modenum)
{
	char	*infobuf;
	int		numimagepages;

	infobuf = dos_getmemory(256);

	regs.x.ax = 0x4f01;
	regs.x.cx = modenum;
	regs.x.es = ptr2real(infobuf) >> 4;
	regs.x.di = ptr2real(infobuf) & 0xf;
	dos_int86(0x10);
	if (regs.x.ax != 0x4f)
	{
		dos_freememory(infobuf);
		return false;
	}
	else
	{
		modeinfo.modenum = modenum;
		modeinfo.bits_per_pixel = *(char*)(infobuf+25);
		modeinfo.bytes_per_pixel = (modeinfo.bits_per_pixel+1)/8;
		modeinfo.width = *(short*)(infobuf+18);
		modeinfo.height = *(short*)(infobuf+20);

	// we do only 8-bpp in software
		if ((modeinfo.bits_per_pixel != 8) ||
			(modeinfo.bytes_per_pixel != 1) ||
			(modeinfo.width > MAXWIDTH) ||
			(modeinfo.height > MAXHEIGHT))
		{
			dos_freememory(infobuf);
			return false;
		}

		modeinfo.mode_attributes = *(short*)infobuf;

	// we only want color graphics modes that are supported by the hardware
		if ((modeinfo.mode_attributes &
			 (MODE_SUPPORTED_IN_HW | COLOR_MODE | GRAPHICS_MODE)) !=
			(MODE_SUPPORTED_IN_HW | COLOR_MODE | GRAPHICS_MODE))
		{
			dos_freememory(infobuf);
			return false;
		}

	// we only work with linear frame buffers, except for 320x200, which can
	// effectively be linear when banked at 0xA000
		if (!(modeinfo.mode_attributes & LINEAR_FRAME_BUFFER))
		{
			if ((modeinfo.width != 320) || (modeinfo.height != 200))
				{
				dos_freememory(infobuf);
				return false;
				}
		}

		modeinfo.bytes_per_scanline = *(short*)(infobuf+16);

		modeinfo.pagesize = modeinfo.bytes_per_scanline * modeinfo.height;

		if (modeinfo.pagesize > totalvidmem)
		{
			dos_freememory(infobuf);
			return false;
		}

	// force to one page if the adapter reports it doesn't support more pages
	// than that, no matter how much memory it has--it may not have hardware
	// support for page flipping
		numimagepages = *(unsigned char *)(infobuf+29);

		if (numimagepages <= 0)
		{
		// wrong, but there seems to be an ATI VESA driver that reports 0
			modeinfo.numpages = 1;
		}
		else if (numimagepages < 3)
		{
			modeinfo.numpages = numimagepages;
		}
		else
		{
			modeinfo.numpages = 3;
		}

		if (*(char*)(infobuf+2) & 5)
		{
			modeinfo.winasegment = *(unsigned short*)(infobuf+8);
			modeinfo.win = 0;
		}
		else if (*(char*)(infobuf+3) & 5)
		{
			modeinfo.winbsegment = *(unsigned short*)(infobuf+8);
			modeinfo.win = 1;
		}
		modeinfo.granularity = *(short*)(infobuf+4) * 1024;
		modeinfo.win_size = *(short*)(infobuf+6) * 1024;
		modeinfo.bits_per_pixel = *(char*)(infobuf+25);
		modeinfo.bytes_per_pixel = (modeinfo.bits_per_pixel+1)/8;
		modeinfo.memory_model = *(unsigned char*)(infobuf+27);
		modeinfo.num_pages = *(char*)(infobuf+29) + 1;

		modeinfo.red_width = *(char*)(infobuf+31);
		modeinfo.red_pos = *(char*)(infobuf+32);
		modeinfo.green_width = *(char*)(infobuf+33);
		modeinfo.green_pos = *(char*)(infobuf+34);
		modeinfo.blue_width = *(char*)(infobuf+35);
		modeinfo.blue_pos = *(char*)(infobuf+36);

		modeinfo.pptr = *(long *)(infobuf+40);
//8bit linear only
if((modeinfo.memory_model==0x4)&&(modeinfo.bits_per_pixel==8))
	{
	Com_Printf("VESA mode 0x%0x %dx%d supported\n",modeinfo.modenum,modeinfo.width,modeinfo.height);
	if(num_vid_resolutions<10) {
	vid_resolutions[num_vid_resolutions].mode=num_vid_resolutions;
	vid_resolutions[num_vid_resolutions].vesa_mode=modeinfo.modenum;
	vid_resolutions[num_vid_resolutions].height=modeinfo.height;
	vid_resolutions[num_vid_resolutions].width=modeinfo.width;
	vid_resolutions[num_vid_resolutions].address=modeinfo.pptr;
	sprintf(vid_resolutions[num_vid_resolutions].menuname,"[VESA %dx%d]",modeinfo.width,modeinfo.height);
	num_vid_resolutions++;
		}
	}
#if 0
		printf("VID: (VESA) info for mode 0x%x\n", modeinfo.modenum);
		printf("  mode attrib = 0x%0x\n", modeinfo.mode_attributes);
		printf("  win a attrib = 0x%0x\n", *(unsigned char*)(infobuf+2));
		printf("  win b attrib = 0x%0x\n", *(unsigned char*)(infobuf+3));
		printf("  win a seg 0x%0x\n", (int) modeinfo.winasegment);
		printf("  win b seg 0x%0x\n", (int) modeinfo.winbsegment);
		printf("  bytes per scanline = %d\n",
				modeinfo.bytes_per_scanline);
		printf("  width = %d, height = %d\n", modeinfo.width,
				modeinfo.height);
		printf("  win = %c\n", 'A' + modeinfo.win);
		printf("  win granularity = %d\n", modeinfo.granularity);
		printf("  win size = %d\n", modeinfo.win_size);
		printf("  bits per pixel = %d\n", modeinfo.bits_per_pixel);
		printf("  bytes per pixel = %d\n", modeinfo.bytes_per_pixel);
		printf("  memory model = 0x%x\n", modeinfo.memory_model);
		printf("  num pages = %d\n", modeinfo.num_pages);
		printf("  red width = %d\n", modeinfo.red_width);
		printf("  red pos = %d\n", modeinfo.red_pos);
		printf("  green width = %d\n", modeinfo.green_width);
		printf("  green pos = %d\n", modeinfo.green_pos);
		printf("  blue width = %d\n", modeinfo.blue_width);
		printf("  blue pos = %d\n", modeinfo.blue_pos);
		printf("  phys mem = %x\n", modeinfo.pptr);
#endif
	}

	dos_freememory(infobuf);
	return true;
}


