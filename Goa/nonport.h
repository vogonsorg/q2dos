/******
nonport.h
GameSpy Developer SDK

Copyright 1999 GameSpy Industries, Inc

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757

******/

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <ctype.h>
	#include <errno.h>
	#include <sys/time.h>
#ifdef __DJGPP__ /* WATT-32 library */
	#include <tcp.h>  /* select_s() */
#endif
#endif

#include "gspy.h" /* FS: GameSpy DLL */

#ifdef __cplusplus
extern "C" {
#endif

unsigned long current_time(void);
void msleep(unsigned long msec);

void SocketStartUp(void);
void SocketShutDown(void);

extern gspyimport_t gspyi;

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR (-1)
#endif

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET (-1)
#endif

#ifdef _WIN32
	#define strcasecmp _stricmp
	#define selectsocket select
	#define IOCTLARG_T
	typedef int socklen_t;
#elif defined(__DJGPP__)
	#define _strdup strdup
	#define IOCTLARG_T	(char*)
	#define SOCKET int
	#define selectsocket select_s
	typedef int socklen_t;
#else
	#define _strdup strdup
	#define SOCKET int
	#define ioctlsocket ioctl
	#define closesocket close
	#define selectsocket select
	#define IOCTLARG_T
#endif

#ifdef __cplusplus
}
#endif
