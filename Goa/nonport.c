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
