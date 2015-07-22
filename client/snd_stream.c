/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// snd_stream.c -- Ogg Vorbis stuff


#include "client.h"
#include "snd_loc.h"

#ifdef OGG_SUPPORT

static bgTrack_t	s_bgTrack;

static channel_t	*s_streamingChannel;

qboolean		ogg_first_init = true;	// First initialization flag
qboolean		ogg_started = false;	// Initialization flag
ogg_status_t	ogg_status;		// Status indicator

#define			MAX_OGGLIST 512
char			**ogg_filelist;		// List of Ogg Vorbis files
int				ogg_numfiles;		// Number of Ogg Vorbis files
int				ogg_loopcounter;

cvar_t			*ogg_loopcount;
cvar_t			*ogg_ambient_track;


/*
=======================================================================

OGG VORBIS STREAMING

=======================================================================
*/


static size_t ovc_read (void *ptr, size_t size, size_t nmemb, void *datasource)
{
	bgTrack_t	*track = (bgTrack_t *)datasource;

	if (!size || !nmemb)
		return 0;
#ifdef OGG_DIRECT_FILE
	return fread(ptr, 1, size * nmemb, track->file) / size;
#else
	Com_Printf("Calling FS_FRead\n");
//	return FS_Read(ptr, size * nmemb, track->file) / size;
	return FS_FRead(ptr, size * nmemb, 1, track->file) / size;
#endif
}


static int ovc_seek (void *datasource, ogg_int64_t offset, int whence)
{
	bgTrack_t	*track = (bgTrack_t *)datasource;

	switch (whence)
	{
#ifdef OGG_DIRECT_FILE
	case SEEK_SET:
		fseek(track->file, (int)offset, SEEK_SET);
		break;
	case SEEK_CUR:
		fseek(track->file, (int)offset, SEEK_CUR);
		break;
	case SEEK_END:
		fseek(track->file, (int)offset, SEEK_END);
#else
	Com_Printf("Calling FS_Seek\n");
	case SEEK_SET:
		FS_Seek(track->file, (int)offset, FS_SEEK_SET);
		break;
	case SEEK_CUR:
		FS_Seek(track->file, (int)offset, FS_SEEK_CUR);
		break;
	case SEEK_END:
		FS_Seek(track->file, (int)offset, FS_SEEK_END);
#endif
		break;
	default:
		return -1;
	}

	return 0;
}


static int ovc_close (void *datasource)
{
	return 0;
}


static long ovc_tell (void *datasource)
{
	bgTrack_t	*track = (bgTrack_t *)datasource;

#ifdef OGG_DIRECT_FILE
	return ftell(track->file);
#else
	Com_Printf("Calling FS_Tell\n");
	return FS_Tell(track->file);
#endif
}


/*
=================
S_OpenBackgroundTrack
=================
*/
static qboolean S_OpenBackgroundTrack (char *name, bgTrack_t *track)
{
	OggVorbis_File	*vorbisFile;
	vorbis_info		*vorbisInfo;
	ov_callbacks	vorbisCallbacks = {ovc_read, ovc_seek, ovc_close, ovc_tell};
#ifdef OGG_DIRECT_FILE
	char	filename[1024];
	char	*path = NULL;
#endif

//	Com_Printf("Opening background track: %s\n", name);

#ifdef OGG_DIRECT_FILE
	do {
		path = FS_NextPath( path );
		Com_sprintf( filename, sizeof(filename), "%s/%s", path, name );
		if ( (track->file = fopen(filename, "rb")) != 0)
			break;
	} while ( path );
#else
	FS_FOpenFile(name, &track->file);
#endif
	if (!track->file)
	{
		Com_Printf("S_OpenBackgroundTrack: couldn't find %s\n", name);
		return false;
	}

	track->vorbisFile = vorbisFile = Z_Malloc(sizeof(OggVorbis_File));

//	Com_Printf("Opening callbacks for background track\n");

	// bombs out here- ovc_read, FS_Read 0 bytes error
	if (ov_open_callbacks(track, vorbisFile, NULL, 0, vorbisCallbacks) < 0)
	{
		Com_Printf("S_OpenBackgroundTrack: couldn't open OGG stream (%s)\n", name);
		return false;
	}

//	Com_Printf("Getting info for background track\n");

	vorbisInfo = ov_info(vorbisFile, -1);
	if (vorbisInfo->channels != 1 && vorbisInfo->channels != 2)
	{
		Com_Printf("S_OpenBackgroundTrack: only mono and stereo OGG files supported (%s)\n", name);
		return false;
	}

	track->start = ov_raw_tell(vorbisFile);
	track->rate = vorbisInfo->rate;
	track->width = 2;
	track->channels = vorbisInfo->channels; // Knightmare added

//	Com_Printf("Vorbis info: frequency: %i channels: %i bitrate: %i\n",
//		vorbisInfo->rate, vorbisInfo->channels, vorbisInfo->bitrate_nominal);

	return true;
}


/*
=================
S_CloseBackgroundTrack
=================
*/
static void S_CloseBackgroundTrack (bgTrack_t *track)
{

	if (track->vorbisFile)
	{
		ov_clear(track->vorbisFile);

		Z_Free(track->vorbisFile);
		track->vorbisFile = NULL;
	}

	if (track->file)
	{
	#ifdef OGG_DIRECT_FILE
		fclose(track->file);
	#else
		FS_FCloseFile(track->file);
	#endif
		track->file = 0;
	}
}

/*
============
S_StreamBackgroundTrack
============
*/
void S_StreamBackgroundTrack (void)
{
	int		samples, maxSamples;
	int		read, maxRead, total, dummy;
	float	scale;
	byte	data[MAX_RAW_SAMPLES]; /* FS: Was MAX_RAW_SAMPLES*4 and appears to be unnecessary */

	if (!s_bgTrack.file || !s_musicvolume->value || !cl_ogg_music->intValue)
		return;

	if (!s_streamingChannel)
		return;

	if (s_rawend < paintedtime)
		s_rawend = paintedtime;

	scale = (float)s_bgTrack.rate / dma.speed;
	maxSamples = sizeof(data) / s_bgTrack.channels / s_bgTrack.width;

	while (1)
	{
		samples = (paintedtime + MAX_RAW_SAMPLES - s_rawend) * scale;
		if (samples <= 0)
			return;
		if (samples > maxSamples)
			samples = maxSamples;
		maxRead = samples * s_bgTrack.channels * s_bgTrack.width;

		total = 0;
		while (total < maxRead)
		{
			/* # ov_read() from libvorbisfile returns the decoded PCM audio
			 *   in requested endianness, signedness and word size.
			 * # ov_read() from Tremor (libvorbisidec) returns decoded audio
			 *   always in host-endian, signed 16 bit PCM format.
			 * # For both of the libraries, if the audio is multichannel,
			 *   the channels are interleaved in the output buffer.
			 */
			read = ov_read(s_bgTrack.vorbisFile, (char *)(data + total), maxRead - total,
#if !defined(VORBIS_USE_TREMOR)
											0, 2, 1,
#endif /* ! VORBIS_USE_TREMOR */
											&dummy);
			if (!read)
			{	// End of file
				if (!s_bgTrack.looping)
				{	// Close the intro track
					S_CloseBackgroundTrack(&s_bgTrack);

					// Open the loop track
					if (!S_OpenBackgroundTrack(s_bgTrack.loopName, &s_bgTrack)) {
						S_StopBackgroundTrack();
						return;
					}
					s_bgTrack.looping = true;
				}
				else
				{	// check if it's time to switch to the ambient track
					if ( ++ogg_loopcounter >= (int)ogg_loopcount->intValue
						&& (!cl.configstrings[CS_MAXCLIENTS][0] || !strcmp(cl.configstrings[CS_MAXCLIENTS], "1")) )
					{	// Close the loop track
						S_CloseBackgroundTrack(&s_bgTrack);

						if (!S_OpenBackgroundTrack(s_bgTrack.ambientName, &s_bgTrack)) {
							if (!S_OpenBackgroundTrack(s_bgTrack.loopName, &s_bgTrack)) {
								S_StopBackgroundTrack();
								return;
							}
						}
						else
							s_bgTrack.ambient_looping = true;
					}
				}

				// Restart the track, skipping over the header
				ov_raw_seek(s_bgTrack.vorbisFile, (ogg_int64_t)s_bgTrack.start);
			}

			total += read;
		}
		S_RawSamples (samples, s_bgTrack.rate, s_bgTrack.width, s_bgTrack.channels, data, true );
	}
}

/*
============
S_UpdateBackgroundTrack

Streams background track
============
*/
void S_UpdateBackgroundTrack (void)
{
	// stop music if paused
	if (ogg_status == PLAY)// && !cl_paused->value)
		S_StreamBackgroundTrack ();
}

// =====================================================================


/*
=================
S_StartBackgroundTrack
=================
*/
void S_StartBackgroundTrack (const char *introTrack, const char *loopTrack)
{
	if (!ogg_started) // was sound_started
		return;

	// Stop any playing tracks
	S_StopBackgroundTrack();

	// Start it up
	Q_strncpyz(s_bgTrack.introName, introTrack, sizeof(s_bgTrack.introName));
	Q_strncpyz(s_bgTrack.loopName, loopTrack, sizeof(s_bgTrack.loopName));
	Q_strncpyz(s_bgTrack.ambientName, va("music/%s.ogg", ogg_ambient_track->string), sizeof(s_bgTrack.ambientName));

	// set a loop counter so that this track will change to the ambient track later
	ogg_loopcounter = 0;

	S_StartStreaming();

	// Open the intro track
	if (!S_OpenBackgroundTrack(s_bgTrack.introName, &s_bgTrack))
	{
		S_StopBackgroundTrack();
		return;
	}

	ogg_status = PLAY;

	S_StreamBackgroundTrack();
}

/*
=================
S_StopOGGBackgroundTrack
=================
*/

/* FS: Called from S_StopBackgroundTrack in snd_dma.c so OGG and WAV won't clash over Channel 0 */
void S_StopOGGBackgroundTrack (void)
{
	if (!ogg_started) // was sound_started
		return;

	S_StopStreaming ();

	S_CloseBackgroundTrack(&s_bgTrack);

	ogg_status = STOP;

	memset(&s_bgTrack, 0, sizeof(bgTrack_t));
}

/*
=================
S_StartStreaming
=================
*/
void S_StartStreaming (void)
{
	if (!ogg_started) // was sound_started
		return;

	if (s_streamingChannel)
		return;		// Already started

	s_streamingChannel = S_PickChannel(0, 0);
	if (!s_streamingChannel)
		return;

	s_streamingChannel->streaming = true;
}

/*
=================
S_StopStreaming
=================
*/
void S_StopStreaming (void)
{
	if (!ogg_started) // was sound_started
		return;

	if (!s_streamingChannel)
		return;		// Already stopped

	s_streamingChannel->streaming = false;

	s_streamingChannel = NULL;
}


// =====================================================================


/*
==========
S_OGG_Init

Initialize the Ogg Vorbis subsystem
Based on code by QuDos
==========
*/
void S_OGG_Init (void)
{
	if (ogg_started)
		return;

	// Cvars
	ogg_loopcount = Cvar_Get ("ogg_loopcount", "5", CVAR_ARCHIVE);
	ogg_ambient_track = Cvar_Get ("ogg_ambient_track", "track11", CVAR_ARCHIVE);

	// Console commands
	Cmd_AddCommand("ogg", S_OGG_ParseCmd);

	// Build list of files
	Com_Printf("Searching for Ogg Vorbis files...\n");
	ogg_numfiles = 0;
	S_OGG_LoadFileList ();
	Com_Printf("%d Ogg Vorbis files found.\n", ogg_numfiles);

	// Initialize variables
	if (ogg_first_init) {
		ogg_status = STOP;
		ogg_first_init = false;
	}

	ogg_started = true;
}


/*
==========
S_OGG_Shutdown

Shutdown the Ogg Vorbis subsystem
Based on code by QuDos
==========
*/
void S_OGG_Shutdown (void)
{
	int		i;

	if (!ogg_started)
		return;

	S_StopBackgroundTrack ();

	// Free the list of files
	for (i = 0; i < ogg_numfiles; i++)
		free(ogg_filelist[i]);
	if (ogg_numfiles > 0)
		free(ogg_filelist);

	// Remove console commands
	Cmd_RemoveCommand("ogg");

	ogg_started = false;
}


/*
==========
S_OGG_Restart

Reinitialize the Ogg Vorbis subsystem
Based on code by QuDos
==========
*/
void S_OGG_Restart (void)
{
	S_OGG_Shutdown ();
	S_OGG_Init ();
}


/*
==========
S_OGG_LoadFileList

Load list of Ogg Vorbis files in music/
Based on code by QuDos
==========
*/
void S_OGG_LoadFileList (void)
{
	char	*p, *path = NULL;
	char	**list;			// List of .ogg files
	char	findname[MAX_OSPATH];
	char	lastPath[MAX_OSPATH];	// Knightmare added
	int		i, numfiles = 0;

	ogg_filelist = malloc(sizeof(char *) * MAX_OGGLIST);
	memset( ogg_filelist, 0, sizeof( char * ) * MAX_OGGLIST );
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
		Com_sprintf( findname, sizeof(findname), "%s/music/*.ogg", path );
		list = FS_ListFiles(findname, &numfiles, 0, SFF_SUBDIR | SFF_HIDDEN | SFF_SYSTEM);

		// Add valid Ogg Vorbis file to the list
		for (i=0; i<numfiles && ogg_numfiles<MAX_OGGLIST; i++)
		{
			if (!list || !list[i])
				continue;
			p = list[i];

			if (!strstr(p, ".ogg"))
				continue;
		//	if (!S_OGG_Check(p))
		//		continue;
			if (!FS_ItemInList(p, ogg_numfiles, ogg_filelist)) // check if already in list
			{
				ogg_filelist[ogg_numfiles] = malloc(strlen(p)+1);
				sprintf(ogg_filelist[ogg_numfiles], "%s", p);
				ogg_filelist[ogg_numfiles][strlen(p)] = '\0'; // FS: Compiler warning, was using %s\0 in the sprintf
				ogg_numfiles++;
			}
		}
		if (numfiles) // Free the file list
			FS_FreeFileList(list, numfiles);

		Q_strncpyz (lastPath, path, sizeof(lastPath));	// Knightmare- copy to lastPath
		path = FS_NextPath( path );
	}

#ifndef OGG_DIRECT_FILE
	// check pak after
	if (list = FS_ListPak("music/", &numfiles))
	{
		// Add valid Ogg Vorbis file to the list
		for (i=0; i<numfiles && ogg_numfiles<MAX_OGGLIST; i++)
		{
			if (!list || !list[i])
				continue;
			p = list[i];
			if (!strstr(p, ".ogg"))
				continue;
		//	if (!S_OGG_Check(p))
		//		continue;
			if (!FS_ItemInList(p, ogg_numfiles, ogg_filelist)) // check if already in list
			{
				ogg_filelist[ogg_numfiles] = malloc(strlen(p)+1);
				sprintf(ogg_filelist[ogg_numfiles], "%s\0", p);
				ogg_numfiles++;
			}
		}
	}

	if (numfiles)
		FS_FreeFileList(list, numfiles);
#endif // OGG_DIRECT_FILE
}

// =====================================================================


/*
=================
S_OGG_PlayCmd
Based on code by QuDos
=================
*/
void S_OGG_PlayCmd (void)
{
	char	name[MAX_QPATH];

	if (Cmd_Argc() < 3) {
		Com_Printf("Usage: ogg play {track}\n");
		return;
	}
	Com_sprintf(name, sizeof(name), "music/%s.ogg", Cmd_Argv(2) );
	S_StartBackgroundTrack (name, name);
}


/*
=================
S_OGG_StatusCmd
Based on code by QuDos
=================
*/
void S_OGG_StatusCmd (void)
{
	char	*trackName;

	if (s_bgTrack.ambient_looping)
		trackName = s_bgTrack.ambientName;
	else if (s_bgTrack.looping)
		trackName = s_bgTrack.loopName;
	else
		trackName = s_bgTrack.introName;

	switch (ogg_status) {
	case PLAY:
#if !defined(VORBIS_USE_TREMOR)
		Com_Printf("Playing file %s at %0.2f seconds.\n",
		    trackName, ov_time_tell(s_bgTrack.vorbisFile));
#else
		Com_Printf("Playing file %s at %0.2f seconds.\n",
		    trackName, ov_time_tell(s_bgTrack.vorbisFile)/1000.0);
#endif
		break;
	case PAUSE:
#if !defined(VORBIS_USE_TREMOR)
		Com_Printf("Paused file %s at %0.2f seconds.\n",
		    trackName, ov_time_tell(s_bgTrack.vorbisFile));
#else
		Com_Printf("Paused file %s at %0.2f seconds.\n",
		    trackName, ov_time_tell(s_bgTrack.vorbisFile)/1000.0);
#endif
		break;
	case STOP:
		Com_Printf("Stopped.\n");
		break;
	}
}


/*
==========
S_OGG_ListCmd

List Ogg Vorbis files
Based on code by QuDos
==========
*/
void S_OGG_ListCmd (void)
{
	int i;

	if (ogg_numfiles <= 0) {
		Com_Printf("No Ogg Vorbis files to list.\n");
		return;
	}

	for (i = 0; i < ogg_numfiles; i++)
		Com_Printf("%d %s\n", i+1, ogg_filelist[i]);

	Com_Printf("%d Ogg Vorbis files.\n", ogg_numfiles);
}


/*
=================
S_OGG_ParseCmd

Parses OGG commands
Based on code by QuDos
=================
*/
void S_OGG_ParseCmd (void)
{
	char	*command;

	if (Cmd_Argc() < 2) {
		Com_Printf("Usage: ogg {play | pause | resume | stop | status | list}\n");
		return;
	}

	command = Cmd_Argv (1);

	if (Q_strcasecmp(command, "play") == 0) {
		S_OGG_PlayCmd ();
		return;
	}

	if (Q_strcasecmp(command, "pause") == 0) {
		if (ogg_status == PLAY)
			ogg_status = PAUSE;
		return;
	}

	if (Q_strcasecmp(command, "resume") == 0) {
		if (ogg_status == PAUSE)
			ogg_status = PLAY;
		return;
	}

	if (Q_strcasecmp(command, "stop") == 0) {
		S_StopBackgroundTrack ();
		return;
	}

	if (Q_strcasecmp(command, "status") == 0) {
		S_OGG_StatusCmd ();
		return;
	}

	if (Q_strcasecmp(command, "list") == 0) {
		S_OGG_ListCmd ();
		return;
	}

	Com_Printf("Usage: ogg {play | pause | resume | stop | status | list}\n");
}

#endif // OGG_SUPPORT
