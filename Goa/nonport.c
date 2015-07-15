/******
nonport.c
GameSpy Developer SDK

Copyright 1999 GameSpy Industries, Inc

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757

******/
#include "nonport.h"

#ifdef __cplusplus
extern "C" {
#endif

// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error (char *error, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
//	vsprintf (text, error, argptr);
	Q_vsnprintf (text, sizeof(text), error, argptr);	// Knightmare- buffer overflow fix
	va_end (argptr);

	gspyi.Sys_Error (ERR_DROP, "%s", text);
}

void Com_Printf (char *msg, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
//	vsprintf (text, msg, argptr);
	Q_vsnprintf (text, sizeof(text), msg, argptr);	// Knightmare- buffer overflow fix
	va_end (argptr);

	gspyi.Con_Printf("%s", text);
}

unsigned long current_time(void) /* returns current time in msec */
{
#ifndef _WIN32
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
#else
	return (GetTickCount());
#endif
}

void msleep(unsigned long msec)
{
#ifndef _WIN32
	usleep(msec * 1000);
#else
	Sleep(msec);
#endif
}

void SocketStartUp(void)
{
#ifdef _WIN32
	WSADATA data;
	WSAStartup(0x0101, &data);
#endif
}

void SocketShutDown(void)
{
#ifdef _WIN32
	WSACleanup();
#endif
}
