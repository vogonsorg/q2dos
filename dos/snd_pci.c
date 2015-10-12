/* snd_pci.c
 * PCI sound card support through libau
 */

#ifdef USE_SNDPCI

#include <dos.h>
#include "../client/client.h"
#include "../client/snd_loc.h"
#include "dosisms.h"
#include "libaudef.h"

/*
==================
PCI_Init

Returns false if nothing is found.
==================
*/
qboolean PCI_Init(void)
{
	const struct mpxplay_audioout_info_s *aui;
	const char *c;

	c = AU_search(1);/* 1: stereo speaker output (meaningful only for Intel HDA chips) */

	if(c)
	{
		unsigned int speed=22050;
		unsigned int samplebits=16;
		unsigned int channels=2;

		Com_Printf("PCI Audio: %s\n",c);
		if(s_khz->intValue >= 11025) /* FS */
			speed = s_khz->intValue;
		dma.speed=speed;
		dma.samplebits=samplebits;
		dma.channels=channels;
		AU_setrate(&speed,&samplebits,&channels);

		aui = AU_getinfo();
		dma.speed=aui->freq_card;
		dma.samplebits=aui->bits_set;
		dma.channels=aui->chan_set;

		if(dma.speed != s_khz->intValue) /* FS: In theory, our rate was not liked, so force the change. */
			Cvar_SetValue("s_khz", dma.speed);
		Com_DPrintf(DEVELOPER_MSG_SOUND, "Post AU_setrate %d/%d/%d\n",dma.speed,dma.samplebits,dma.channels);

		dma.samples = aui->card_dmasize/aui->bytespersample_card;
		dma.samplepos = 0;
		dma.submission_chunk = 1;
		dma.buffer = (unsigned char *) aui->card_DMABUFF;
		memset(dma.buffer, 0, aui->card_dmasize);

		AU_setmixer_all(80); /* 80% volume */
		AU_start();

		return true;
	}

	Com_Printf("PCI Audio: Detection failed.\n");

	return false;
}

/*
==============
PCI_GetDMAPos

return the current sample position (in mono samples read)
inside the recirculating dma buffer, so the mixing code will know
how many sample are required to fill it up.
===============
*/
int PCI_GetDMAPos(void)
{
	dma.samplepos = AU_cardbuf_space();
	return dma.samplepos;
}


/*
==============
PCI_Shutdown

Stop and close the sound device for exiting
===============
*/
void PCI_Shutdown(void)
{
	AU_close();
}

#endif /* USE_SNDPCI */

