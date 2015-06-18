
// snddma_null.c
// all other sound mixing is portable

#include "../client/client.h"
#include "../client/snd_loc.h"
#include "../wss/wss.h"

int	havegus;
static  short *dma_buffer = NULL; // sezero


typedef enum
{
        dma_none,
} dmacard_t;

dmacard_t                dmacard;


void GUS_ClearDMA(void)
{
}

qboolean SNDDMA_Init(void)
{
  w_set_device_master_volume(0);
  if(w_sound_device_init(1, 11025) == FALSE)
        {
        Com_Printf("WSS error:%s\n", w_get_error_message());
	return false;
        }
  else {
	dmacard = dma_none;
	dma.speed = 11025;
	dma.channels = 1;
	dma.samplebits = 8;

	dma_buffer = malloc(1024*256);
	//dma_size = 1024*256;
	dma.samples = 1024*256/(dma.samplebits/8);
        dma.samplepos = 0;
        dma.submission_chunk = 1;
        dma.buffer = (unsigned char *) dma_buffer;
        dma.samples = 1024*256/(dma.samplebits/8);
	return true;	
	}	
return false;
}

int	SNDDMA_GetDMAPos(void)
{
	//magic happens here
        w_lock_mixing_buffer(256);
        w_mixing_stereo(dma.buffer, 256, 256, 256);
        w_unlock_mixing_buffer();
	__dpmi_yield();
	return 0;
}

void SNDDMA_Shutdown(void)
{
Com_Printf("SNDDMA_Shutdown\n");
        w_sound_device_exit();
}

void SNDDMA_BeginPainting (void)
{
}

void SNDDMA_Submit(void)
{
}

void S_Activate (qboolean active)
{
}
