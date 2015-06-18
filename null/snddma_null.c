
// snddma_null.c
// all other sound mixing is portable

#include "../client/client.h"
#include "../client/snd_loc.h"

int	havegus;

//
// Fake dma is a synchronous faking of the DMA progress used for
// isolating performance in the renderer.  The fakedma_updates is
// number of times S_Update() is called per second.
//

qboolean fakedma = false;
static short *dma_buffer = NULL;

void GUS_ClearDMA(void)
{
}

qboolean SNDDMA_Init(void)
{
	Com_Printf("NULL Sound driver initialization\n");
// create a piece of DMA memory
	dma_buffer = (short *)malloc(SND_BUFFER_SIZE); // FS: This is the pointer to the real address if it was a real sound card.
	if(!dma_buffer)
		Sys_Error("NULL SNDDMA_Init: Couldn't allocate memory for NULL sound buffer!\n");
	memset(dma_buffer, 0, SND_BUFFER_SIZE);//Hunk_AllocName(1<<16, "dmabuf");
	dma.samplebits = 16;
	dma.speed = 22050;
	dma.channels = 2;
	dma.samples = SND_BUFFER_SIZE/(dma.samplebits/8);
	dma.samplepos = 0;
	dma.submission_chunk = 1;
	dma.buffer = (unsigned char *)dma_buffer;

	return true;
}

int	SNDDMA_GetDMAPos(void)
{
	// FS: This would grab the current position where we're at in the buffer, but since it's fake it doesn't matter.
	return 0;
}

void SNDDMA_Shutdown(void)
{
}

void SNDDMA_BeginPainting (void)
{
	// FS: Not used in DOS
}

void SNDDMA_Submit(void)
{
	// FS: Not used in DOS
}

void S_Activate (qboolean active)
{
}
