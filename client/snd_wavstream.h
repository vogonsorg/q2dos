/* FS: Stuff for music WAV streaming */
void S_StreamWav_GetInfo(char *fileName, int fileLen);
void S_UpdateWavTrack(void);
void S_StreamWav_f(void);

void S_WAV_Init (void);
void S_WAV_Shutdown (void);
void S_WAV_Restart (void);
void S_WAV_LoadFileList (void);
void S_WAV_ParseCmd (void);
void S_StopWAVBackgroundTrack (void);
void S_StopWAVStreaming (void);
void S_StartWAVStreaming (void);
void StreamWAVTrack(void);


