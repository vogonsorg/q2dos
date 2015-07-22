#include "client.h"
#include "snd_loc.h"

static FILE *wavFile = NULL;
static wavinfo_t musicWavInfo;
static byte musicWavData[MAX_RAW_SAMPLES];
static qboolean streamingWav = false;

void S_StreamWav_f(void)
{
	char musicWavFilename[MAX_OSPATH];
	int streamWavLen;

	Com_sprintf(musicWavFilename, sizeof(musicWavFilename), "music/track02.wav"); /* FS: Don't need FS_Gamedir as FS_FOpenFile takes care of the search */
	streamWavLen = FS_FOpenFile(musicWavFilename, &wavFile);

	if(streamWavLen > 0)
	{
		Com_Printf("Length: %i\n", streamWavLen);
		S_StreamWav_GetInfo(musicWavFilename, streamWavLen);
	}
	else
	{
		Com_Printf("Error: Couldn't find %s!\n", musicWavFilename);
	}
}

void S_StreamWav_GetInfo(char *fileName, int fileLen)
{
	byte data[256];

	FS_Read(data, sizeof(data), wavFile);
	musicWavInfo = GetWavinfo(fileName, data, sizeof(data));
	FS_Seek(wavFile, musicWavInfo.dataofs, FS_SEEK_SET);

	Com_Printf("Rate: %i.  Data Offset: %i. Width: %i.  Channels: %i.\n", musicWavInfo.rate, musicWavInfo.dataofs, musicWavInfo.width, musicWavInfo.channels);
	streamingWav = true;
}

void S_UpdateWavTrack(void)
{
	int samples, maxSamples;
	int read, maxRead, total;
	float scale;

	if (!streamingWav)
		return;

	if (s_rawend < paintedtime)
		s_rawend = paintedtime;

	scale = (float)musicWavInfo.rate / dma.speed;
	maxSamples = sizeof(musicWavData) / musicWavInfo.channels / musicWavInfo.width;

	Com_Printf("Scale: %f.  Max Samples: %i\n", scale, maxSamples);

	while (1)
	{
		samples = (paintedtime + MAX_RAW_SAMPLES - s_rawend) * scale;
		if (samples <= 0)
			return;
		if (samples > maxSamples)
			samples = maxSamples;
		maxRead = samples * musicWavInfo.channels * musicWavInfo.width;

		total = 0;
		while (total < maxRead)
		{
//			FS_Read(musicWavData, maxRead, wavFile);
			read = FS_FRead(musicWavData, maxRead, 1, wavFile);
			if(read == 0)
			{
				Com_Printf("No more to read... restart someday...\n");
				streamingWav = false;
				FS_FCloseFile(wavFile);
				return;
			}
			total+= read;
			Com_Printf("Read: %i, Samples: %i, Total: %i\n", read, samples, total);
		}
		S_RawSamples (samples, musicWavInfo.rate, musicWavInfo.width, musicWavInfo.channels, musicWavData, true);
	}
}
