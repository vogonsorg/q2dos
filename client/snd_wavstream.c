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

wav_status_t	wav_status;		// Status indicator

qboolean		wav_first_init = true;	// First initialization flag
qboolean		wav_started = false;	// Initialization flag

#define			MAX_WAVLIST 512
char			**wav_filelist;		// List of WAV files
int				wav_curfile;		// Index of currently played file
int				wav_numfiles;		// Number of WAV files
int				wav_loopcounter;

cvar_t			*wav_loopcount;
cvar_t			*wav_ambient_track;

static qboolean S_OpenWAVBackgroundTrack (char *name, bgTrack_t *track)
{
	char	filename[1024];
	char	*path = NULL;
	int		streamWavLen;

//	Com_Printf("Opening background track: %s\n", name);

	do
	{
		path = FS_NextPath( path );
		Com_sprintf( filename, sizeof(filename), "%s/%s", path, name );
		streamWavLen = FS_FOpenFile(name, &track->file);
		if ( streamWavLen != 0)
			break;
	}
	while ( path );

	if (!track->file)
	{
		Com_Printf("S_OpenWAVBackgroundTrack: couldn't find %s\n", name);
		return false;
	}

	S_StreamWav_GetInfo(name, streamWavLen); /* FS: Get the WAV info using Q2's existing functions */

//	Com_Printf("Getting info for background track\n");

	if (musicWavInfo.channels != 1 && musicWavInfo.channels != 2)
	{
		Com_Printf("S_OpenWAVBackgroundTrack: only mono and stereo WAV files supported (%s)\n", name);
		return false;
	}

	return true;
}

static void S_CloseWAVBackgroundTrack(void)
{
	if(!s_bgTrack.file)
		return;

	FS_FCloseFile(s_bgTrack.file);
	s_bgTrack.file = NULL;
}

void S_StreamWAVBackgroundTrack(void)
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
					if (!S_OpenWAVBackgroundTrack(s_bgTrack.loopName, &s_bgTrack))
					{
						S_StopBackgroundTrack();
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

						if (!S_OpenWAVBackgroundTrack(s_bgTrack.ambientName, &s_bgTrack))
						{
							if (!S_OpenWAVBackgroundTrack(s_bgTrack.loopName, &s_bgTrack))
							{
								S_StopBackgroundTrack();
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
		S_StreamWAVBackgroundTrack();
}

void S_StartWAVBackgroundTrack (const char *introTrack, const char *loopTrack)
{
	if (!wav_started) // was sound_started
		return;

	// Stop any playing tracks
	S_StopBackgroundTrack();

	// Start it up
	Q_strncpyz(s_bgTrack.introName, introTrack, sizeof(s_bgTrack.introName));
	Q_strncpyz(s_bgTrack.loopName, loopTrack, sizeof(s_bgTrack.loopName));
	Q_strncpyz(s_bgTrack.ambientName, va("music/%s.wav", wav_ambient_track->string), sizeof(s_bgTrack.ambientName));

	// set a loop counter so that this track will change to the ambient track later
	wav_loopcounter = 0;

	S_StartWAVStreaming();

	// Open the intro track
	if (!S_OpenWAVBackgroundTrack(s_bgTrack.introName, &s_bgTrack))
	{
		S_StopBackgroundTrack();
		return;
	}

	wav_status = PLAY;

	S_StreamWAVBackgroundTrack();
}

/* FS: Called from S_StopBackgroundTrack in snd_dma.c so OGG and WAV won't clash over Channel 0 */
void S_StopWAVBackgroundTrack (void)
{
	if (!wav_started) // was sound_started
		return;

	S_StopWAVStreaming ();

	S_CloseWAVBackgroundTrack();

	wav_status = WAV_STOP;

	memset(&musicWavInfo, 0, sizeof(wavinfo_t));
	memset(&s_bgTrack, 0, sizeof(bgTrack_t));
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
		S_StopBackgroundTrack ();
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

	S_StopBackgroundTrack ();

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
