/* snddma_dos.c
 * interface to dos audio drivers.
 */

#include <dos.h>
#include "../client/client.h"
#include "../client/snd_loc.h"
#include "dosisms.h"

#define USE_QDOS_SOUND

#ifdef USE_QDOS_SOUND
typedef enum
{
	dma_none,
	dma_blaster,
	dma_pci, /* FS: From ruslans patch */
	dma_gus
} dmacard_t;

static dmacard_t	dmacard;
static int	firstInit = true;

/* GUS SUPPORT */
qboolean GUS_Init (void);
int GUS_GetDMAPos (void);
void GUS_Shutdown (void);

/* SOUND BLASTER SUPPORT */
qboolean BLASTER_Init (void);
int BLASTER_GetDMAPos (void);
void BLASTER_Shutdown (void);

/* PCI SOUND CARD SUPPORT */
qboolean PCI_Init(void);
int PCI_GetDMAPos(void);
void PCI_Shutdown(void);

/*
===============================================================================

INTERFACE

===============================================================================
*/

static void snd_shutdown_f (void) /* FS: SND_SHUTDOWN */
{
	S_Shutdown();
	Com_Printf("\nSound Shutdown.\n");
}
#endif /* USE_QDOS_SOUND */

qboolean SNDDMA_Init(void)
{
#ifndef USE_QDOS_SOUND
	return false;
#else
	if (COM_CheckParm("-nosound"))
		goto nocard;
	if (firstInit)
	{
		firstInit = false;
		Cmd_AddCommand ("snd_shutdown", snd_shutdown_f); /* FS */
	}
	if (COM_CheckParm("-hda")) /* FS: Ruslans patch */
	{
		if(!PCI_Init())
		{
			goto nocard;
		}
		Com_DPrintf(DEVELOPER_MSG_SOUND, "PCI_Init\n");
		dmacard = dma_pci;
		return true;
	}
	if (GUS_Init ())
	{
		Com_DPrintf(DEVELOPER_MSG_SOUND, "GUS_Init\n");
		dmacard = dma_gus;
		S_StopAllSounds(); /* FS: For GUS Buffer Clear Fix */
		return true;
	}
	if (BLASTER_Init ())
	{
		Com_DPrintf(DEVELOPER_MSG_SOUND, "BLASTER_Init\n");
		dmacard = dma_blaster;
		return true;
	}
nocard:
	dmacard = dma_none;
	return false;
#endif
}

int	SNDDMA_GetDMAPos(void)
{
#ifdef USE_QDOS_SOUND
	switch (dmacard)
	{
	case dma_blaster:
		return BLASTER_GetDMAPos ();
	case dma_gus:
		return GUS_GetDMAPos ();
	case dma_pci: /* FS: Ruslans patch */
		return PCI_GetDMAPos ();
	case dma_none:
		break;
	}
#endif
	return 0;
}

void SNDDMA_Shutdown(void)
{
#ifdef USE_QDOS_SOUND
	switch (dmacard)
	{
	case dma_blaster:
		BLASTER_Shutdown ();
		break;
	case dma_gus:
		GUS_Shutdown ();
		break;
	case dma_pci: /* FS: Ruslans patch */
		PCI_Shutdown ();
		break;
	case dma_none:
		break;
	}

	dmacard = dma_none;
	return;
#endif
}

void SNDDMA_BeginPainting (void)
{
	/* Nothing in DOS */
}

void SNDDMA_Submit(void)
{
	/* Nothing in DOS */
}

void S_Activate (qboolean active)
{
	Com_Printf("S_Activate %d\n", active);
}
