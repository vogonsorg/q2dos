/* FS: WAV streaming.  Basically a paraphrased snd_stream.c for OGG/Vorbis */

#include "client.h"
#include "snd_loc.h"

static wavinfo_t musicWavInfo;
static byte musicWavData[MAX_RAW_SAMPLES];
static channel_t	*s_streamingChannel;
static bgTrack_t	s_bgTrack;

typedef enum {
	WAV_PLAY,
	WAV_PAUSE,
	WAV_STOP
} wav_status_t;

qboolean		wav_first_init = true;	// First initialization flag
qboolean		wav_started = false;	// Initialization flag
wav_status_t	wav_status;		// Status indicator

#define			MAX_WAVLIST 512
char			**wav_filelist;		// List of Ogg Vorbis files
int				wav_curfile;		// Index of currently played file
int				wav_numfiles;		// Number of Ogg Vorbis files
int				wav_loopcounter;

cvar_t			*wav_loopcount;
cvar_t			*wav_ambient_track;

qboolean S_StartWAVBackgroundTrack(const char *introTrack, const char *loopTrack)
{
	int streamWavLen;

	if (!wav_started) // was sound_started
		return false;

	// Stop any playing tracks
	S_StopWAVBackgroundTrack();

	Q_strncpyz(s_bgTrack.introName, introTrack, sizeof(s_bgTrack.introName));
	Q_strncpyz(s_bgTrack.loopName, loopTrack, sizeof(s_bgTrack.loopName));
	Com_sprintf(s_bgTrack.ambientName, sizeof(s_bgTrack.ambientName), "music/%s.wav", wav_ambient_track->string);

	streamWavLen = FS_FOpenFile(s_bgTrack.introName, &s_bgTrack.file);

	if(streamWavLen > 0)
	{
		S_StreamWav_GetInfo(s_bgTrack.introName, streamWavLen);
	}
	else
	{
		Com_Printf("Error: Couldn't find %s!\n", s_bgTrack.introName);
		return false;
	}

	// set a loop counter so that this track will change to the ambient track later
	wav_loopcounter = 0;
	
	S_StartWAVStreaming();

	wav_status = WAV_PLAY;

	StreamWAVTrack();
	return true;
}

static void S_CloseWAVBackgroundTrack(void)
{
	if(!s_bgTrack.file)
		return;

	FS_FCloseFile(s_bgTrack.file);
	s_bgTrack.file = NULL;
}

void StreamWAVTrack(void)
{
	int samples, maxSamples;
	int read, maxRead, total;
	float scale;

	if (!s_bgTrack.file || !s_musicvolume->value || !cl_wav_music->intValue)
		return;

	if (s_rawend < paintedtime)
		s_rawend = paintedtime;

	scale = (float)musicWavInfo.rate / dma.speed;
	maxSamples = sizeof(musicWavData) / musicWavInfo.channels / musicWavInfo.width;

//	Com_Printf("Scale: %f.  Max Samples: %i\n", scale, maxSamples);

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
			read = FS_FRead(musicWavData, maxRead, 1, s_bgTrack.file);
			if (!read)
			{	// End of file
				if (!s_bgTrack.looping)
				{	// Close the intro track
					S_CloseWAVBackgroundTrack();

					// Open the loop track
					if (!S_StartWAVBackgroundTrack(s_bgTrack.loopName, s_bgTrack.loopName))
					{
						S_StopWAVBackgroundTrack();
						return;
					}
					s_bgTrack.looping = true;
				}
				else
				{	// check if it's time to switch to the ambient track
					if ( ++wav_loopcounter >= (int)wav_loopcount->intValue
						&& (!cl.configstrings[CS_MAXCLIENTS][0] || !strcmp(cl.configstrings[CS_MAXCLIENTS], "1")) )
					{	// Close the loop track
						S_CloseWAVBackgroundTrack();

						if (!S_StartWAVBackgroundTrack(s_bgTrack.ambientName, s_bgTrack.ambientName))
						{
							if (!S_StartWAVBackgroundTrack(s_bgTrack.loopName, s_bgTrack.loopName))
							{
								S_StopWAVBackgroundTrack();
								return;
							}
						}
						else
							s_bgTrack.ambient_looping = true;
					}
				}

				// Restart the track, skipping over the header
				FS_Seek(s_bgTrack.file, musicWavInfo.dataofs, FS_SEEK_SET);
			}
			total+= read;
//			Com_Printf("Read: %i, Samples: %i, Total: %i\n", read, samples, total);
		}
		S_RawSamples (samples, musicWavInfo.rate, musicWavInfo.width, musicWavInfo.channels, musicWavData, true);
	}
}

void S_UpdateWavTrack(void)
{
	// stop music if paused
	if (wav_status == WAV_PLAY)// && !cl_paused->value)
		StreamWAVTrack();
}

void S_StopWAVBackgroundTrack (void)
{
	if (!wav_started) // was sound_started
		return;

	S_StopWAVStreaming ();

	S_CloseWAVBackgroundTrack();

	wav_status = WAV_STOP;

	memset(&musicWavInfo, 0, sizeof(wavinfo_t));
}

void S_StartWAVStreaming (void)
{
	if (!wav_started) // was sound_started
		return;

	if (s_streamingChannel)
		return;		// Already started

	s_streamingChannel = S_PickChannel(0, 0);
	if (!s_streamingChannel)
		return;

	s_streamingChannel->streaming = true;
}

void S_StopWAVStreaming (void)
{
	if (!wav_started) // was sound_started
		return;

	if (!s_streamingChannel)
		return;		// Already stopped

	s_streamingChannel->streaming = false;

	s_streamingChannel = NULL;
}

void S_WAV_LoadFileList (void)
{
	char	*p, *path = NULL;
	char	**list;			// List of .ogg files
	char	findname[MAX_OSPATH];
	char	lastPath[MAX_OSPATH];	// Knightmare added
	int		i, numfiles = 0;

	wav_filelist = malloc(sizeof(char *) * MAX_WAVLIST);
	memset( wav_filelist, 0, sizeof( char * ) * MAX_WAVLIST );
	lastPath[0] = 0;	// Knightmare added

	// Set search path
	path = FS_NextPath(path);
	while (path) 
	{
		// Knightmare- catch repeated paths
		if ( strlen(lastPath) > 0 && !strcmp (path, lastPath) ) {
			path = FS_NextPath( path );
			continue;
		}

		// Get file list
		Com_sprintf( findname, sizeof(findname), "%s/music/*.wav", path );
		list = FS_ListFiles(findname, &numfiles, 0, SFF_SUBDIR | SFF_HIDDEN | SFF_SYSTEM);

		// Add valid Ogg Vorbis file to the list
		for (i=0; i<numfiles && wav_numfiles<MAX_WAVLIST; i++)
		{
			if (!list || !list[i])
				continue;
			p = list[i];

			if (!strstr(p, ".wav"))
				continue;
		//	if (!S_OGG_Check(p))
		//		continue;
			if (!FS_ItemInList(p, wav_numfiles, wav_filelist)) // check if already in list
			{
				wav_filelist[wav_numfiles] = malloc(strlen(p)+1);
				sprintf(wav_filelist[wav_numfiles], "%s", p);
				wav_filelist[wav_numfiles][strlen(p)] = '\0'; // FS: Compiler warning, was using %s\0 in the sprintf
				wav_numfiles++;
			}
		}
		if (numfiles) // Free the file list
			FS_FreeFileList(list, numfiles);

		Q_strncpyz (lastPath, path, sizeof(lastPath));	// Knightmare- copy to lastPath
		path = FS_NextPath( path );
	}
}

void S_WAV_ListCmd (void)
{
	int i;

	if (wav_numfiles <= 0) {
		Com_Printf("No WAV files to list.\n");
		return;
	}

	for (i = 0; i < wav_numfiles; i++)
		Com_Printf("%d %s\n", i+1, wav_filelist[i]);

	Com_Printf("%d WAV files.\n", wav_numfiles);
}

void S_WAV_Init (void)
{
	if (wav_started)
		return;

	// Cvars
	wav_loopcount = Cvar_Get ("wav_loopcount", "5", CVAR_ARCHIVE);
	wav_ambient_track = Cvar_Get ("wav_ambient_track", "track11", CVAR_ARCHIVE);

	// Console commands
	Cmd_AddCommand("wav", S_WAV_ParseCmd);

	// Build list of files
	Com_Printf("Searching for WAV files...\n");
	wav_numfiles = 0;
	S_WAV_LoadFileList ();
	Com_Printf("%d WAV files found.\n", wav_numfiles);

	// Initialize variables
	if (wav_first_init) {
		wav_status = WAV_STOP;
		wav_first_init = false;
	}

	wav_started = true;
}

void S_StreamWav_f(void)
{
	char musicWavFilename[MAX_OSPATH];
	int streamWavLen;

	Com_sprintf(musicWavFilename, sizeof(musicWavFilename), "music/track02.wav"); /* FS: Don't need FS_Gamedir as FS_FOpenFile takes care of the search */
	streamWavLen = FS_FOpenFile(musicWavFilename, &s_bgTrack.file);

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

void S_WAV_PlayCmd (void)
{
	char	name[MAX_QPATH];

	if (Cmd_Argc() < 3) {
		Com_Printf("Usage: wav play {track}\n");
		return;
	}
	Com_sprintf(name, sizeof(name), "music/%s.wav", Cmd_Argv(2) );
	S_StartWAVBackgroundTrack (name, name);
}

void S_WAV_StatusCmd (void)
{
	char	*trackName;

	if (s_bgTrack.ambient_looping)
		trackName = s_bgTrack.ambientName;
	else if (s_bgTrack.looping)
		trackName = s_bgTrack.loopName;
	else
		trackName = s_bgTrack.introName;

	switch (wav_status)
	{
		case PLAY:
			Com_Printf("Playing file %s.\n", trackName);
			break;
		case PAUSE:
			Com_Printf("Paused file %s.\n", trackName);
			break;
		case STOP:
			Com_Printf("Stopped.\n");
			break;
	}
}

void S_StreamWav_GetInfo(char *fileName, int fileLen)
{
	byte data[1024]; /* FS: It appears ~44 bytes in is where the data begins. */

	FS_Read(data, sizeof(data), s_bgTrack.file);
	musicWavInfo = GetWavinfo(fileName, data, sizeof(data));
	FS_Seek(s_bgTrack.file, musicWavInfo.dataofs, FS_SEEK_SET);

//	Com_Printf("Rate: %i.  Data Offset: %i. Width: %i.  Channels: %i.\n", musicWavInfo.rate, musicWavInfo.dataofs, musicWavInfo.width, musicWavInfo.channels);
}

/*
=================
S_WAV_ParseCmd

Parses WAV commands
Based on code by QuDos
=================
*/
void S_WAV_ParseCmd (void)
{
	char	*command;

	if (Cmd_Argc() < 2) {
		Com_Printf("Usage: wav {play | pause | resume | stop | status | list}\n");
		return;
	}

	command = Cmd_Argv (1);

	if (Q_strcasecmp(command, "play") == 0) {
		S_WAV_PlayCmd ();
		return;
	}

	if (Q_strcasecmp(command, "pause") == 0) {
		if (wav_status == WAV_PLAY)
			wav_status = WAV_PAUSE;
		return;
	}

	if (Q_strcasecmp(command, "resume") == 0) {
		if (wav_status == WAV_PAUSE)
			wav_status = WAV_PLAY;
		return;
	}

	if (Q_strcasecmp(command, "stop") == 0) {
		S_StopWAVBackgroundTrack ();
		return;
	}

	if (Q_strcasecmp(command, "status") == 0) {
		S_WAV_StatusCmd ();
		return;
	}

	if (Q_strcasecmp(command, "list") == 0) {
		S_WAV_ListCmd ();
		return;
	}

	Com_Printf("Usage: wav {play | pause | resume | stop | status | list}\n");
}

void S_WAV_Shutdown (void)
{
	int		i;

	if (!wav_started)
		return;

	S_StopWAVBackgroundTrack ();

	// Free the list of files
	for (i = 0; i < wav_numfiles; i++)
		free(wav_filelist[i]);
	if (wav_numfiles > 0)
		free(wav_filelist);

	// Remove console commands
	Cmd_RemoveCommand("wav");

	wav_started = false;
}

void S_WAV_Restart (void)
{
	S_WAV_Shutdown ();
	S_WAV_Init ();
}
