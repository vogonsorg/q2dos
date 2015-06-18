
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
  if(w_sound_device_init(13, 11025) == FALSE)
        {
        Com_Printf("WSS error:%s\n", w_get_error_message());
	return false;
        }
  else {
	dmacard = dma_none;
	dma.speed = 11025;
	s_khz->value=11025;
	dma.channels = 2;
	dma.samplebits = 8;
	Cvar_SetValue( "s_loadas8bit", true );
	Com_Printf("wss is using [%s]\n",w_get_device_name());
	Com_Printf("wss rate is %d\n",w_get_actual_sample_rate());
	Com_Printf("wss format is %d\n",w_get_nominal_sample_format());

	dma_buffer = malloc(SND_BUFFER_SIZE);
	//dma.size = 1024*64;
	dma.samples = SND_BUFFER_SIZE/(dma.samplebits/8);
        dma.samplepos = 0;
        dma.submission_chunk = 1;
        dma.buffer = (unsigned char *) dma_buffer;

	Com_Printf("%5d stereo\n", dma.channels - 1);
	Com_Printf("%5d samples\n", dma.samples);
	Com_Printf("%5d samplepos\n", dma.samplepos);
	Com_Printf("%5d samplebits\n", dma.samplebits);
	Com_Printf("%5d submission_chunk\n", dma.submission_chunk);
	Com_Printf("%5d speed\n", dma.speed);
	Com_Printf("%p sound buffer\n", dma.buffer);
	return true;	
	}	
return false;
}

int	SNDDMA_GetDMAPos(void)
{

	//magic happens here
	//w_lock_mixing_buffer(int currentsamplecount)
        w_lock_mixing_buffer(0);
//	w_mixing_stereo(short data[], DWORD length, int leftvol, int rightvol)
	if(dma.samplebits==16)
	w_mixing_stereo(dma.buffer, SND_BUFFER_SIZE, 255, 255);
	else{
//	w_mixing8(char data[], DWORD length, int leftvol, int rightvol)
	w_mixing8(dma.buffer, SND_BUFFER_SIZE/2, 255, 255);
		}
//	Com_Printf("dma.samples %d %d\n",dma.samplepos,dma.samplepos/11025*16*2);
        w_unlock_mixing_buffer();
	__dpmi_yield();
//	pretend we are done?
	dma.samplepos = 0;
        dma.submission_chunk = 1;
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
