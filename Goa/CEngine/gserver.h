/******
gserver.h
GameSpy C Engine SDK

Copyright 1999 GameSpy Industries, Inc

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757

******

 Please see the GameSpy C Engine SDK documentation for more 
 information

******/

#include "goaceng.h"
#include "hashtable.h"

#ifndef _GSERVER_H_
#define _GSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct GServerImplementation
{
	unsigned long ip;
	unsigned short port;
	unsigned short ping;
	HashTable keyvals;
};

typedef struct
{
	char *key, *value;
} GKeyValuePair;

typedef struct
{
	KeyEnumFn EnumFn;
	void *instance;
} GEnumData;

void ServerFree(void *elem);
GServer ServerNew(unsigned long ip, unsigned short port);
void ServerParseKeyVals(GServer server, char *keyvals);

#ifdef __cplusplus
}
#endif

#endif
