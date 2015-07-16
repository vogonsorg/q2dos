/******
gserver.c
GameSpy C Engine SDK

Copyright 1999 GameSpy Industries, Inc

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757

******/
#include "../../server/server.h" /* FS */
#include "../nonport.h"
#include "goaceng.h"
#include "gserver.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static int KeyValHash(const void *elem, int numbuckets);
static int KeyValCompare(const void *entry1, const void *entry2);
static int CaseInsensitiveCompare(const void *entry1, const void *entry2);
static void KeyValFree(void *elem);


void ServerFree(void *elem)
{
	//free a server!
	GServer server = *(GServer *)elem;
	
	TableFree(server->keyvals);
	free(server);
}

GServer ServerNew(char *ip, int port)
{
	GServer server;

	server = malloc(sizeof(struct GServerImplementation));
	strncpy(server->ip,ip, 16);
	server->ip[15] = 0; //make sure it's terminated
	server->port = port;
	server->ping = 9999;
	server->keyvals = TableNew(sizeof(GKeyValuePair),16, KeyValHash, KeyValCompare, KeyValFree);
	return server;
}

/* FS: From FreeBSD */
char *Goa_strtok_r(char *s, const char *delim, char **last)
{
	char *spanp, *tok;
	int c, sc;

	if (s == NULL && (s = *last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		*last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = '\0';
				*last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

static char *mytok(char *instr, char delim)
{
	char *result;
	static char *thestr;

	if (instr)
		thestr = instr;
	result = thestr;
	while (*thestr && *thestr != delim)
	{
		thestr++;
	}
	if (thestr == result)
		result = NULL;
	if (*thestr) /* not the null term */
		*thestr++ = '\0';
	return result;
}

void ServerParsePlayerCount(GServer server, char *savedkeyvals)
{
	int numplayers = 0;
	char players[12];
	char playerSeperators[] = "\n";
	char *s = strdup(savedkeyvals);
	char *test = strtok(s, playerSeperators);
	GKeyValuePair kvpair;
	qboolean	hasBots = false;

	test = strtok(NULL, playerSeperators);

	numplayers = 0;

	while (test != NULL)
	{
		numplayers++;

		if(strstr(test, "WallFly[BZZZ]") || strstr(test, "127.0.0.1")) /* FS: Don't report servers that just have WallFly in them.  127.0.0.1 in any player parse is a bot from Alien Arena */
		{
			hasBots = true;
			numplayers--;
		}

		test = strtok(NULL, playerSeperators);
	}

	if(s) /* FS: Sezero is this right? */
		free(test);

	kvpair.key = _strdup("numplayers");
	sprintf(players, "%i", numplayers);
	kvpair.value = _strdup(players);
	TableEnter(server->keyvals, &kvpair);
}


void ServerParseKeyVals(GServer server, char *keyvals)
{
	char *k = NULL;
	char *v = NULL;
	char tokenSeperators[] = "\\";
	char *kPtr = NULL;
	char savedkeyvals[MAX_MSGLEN];
	GKeyValuePair kvpair;
	int numplayers = 0;

	if(!keyvals || strlen(keyvals) < 11) /* FS: Some kind of bad status packet, forget it. */
		return;

	/*
	*keyvals = *keyvals++; // FS: Skip past the OOB_SEQ
	*keyvals = *keyvals++;
	*keyvals = *keyvals++;
	*keyvals = *keyvals++;
	*keyvals = *keyvals++; // P
	*keyvals = *keyvals++; // R
	*keyvals = *keyvals++; // I
	*keyvals = *keyvals++; // N
	*keyvals = *keyvals++; // T
	*keyvals = *keyvals++; // newline
	*/
	keyvals += 10;

	strncpy(savedkeyvals, keyvals, sizeof(savedkeyvals));
	savedkeyvals[sizeof(savedkeyvals) - 1] = '\0';

	k = Goa_strtok_r(keyvals, tokenSeperators, &kPtr);

	while (k != NULL)
	{
		v = Goa_strtok_r(NULL, tokenSeperators, &kPtr);

		if (v != NULL)
		{
			kvpair.key = _strdup(k);
			kvpair.value = _strdup(v);

			gspyi.print("Adding: %s %s\n", k, v);

			TableEnter(server->keyvals, &kvpair);

			if(strstr(kvpair.value, "\n")) /* FS: Anything after a newline may contain players.  So don't add them as rules.  Just cut off the newline.  It has to happen at the end or else an important rule, like maxclients could get cut off. */
			{
				char *cutoffLen;
				cutoffLen = strchr(kvpair.value, '\n');
				kvpair.value[cutoffLen-kvpair.value] = '\0';
				k = NULL;
			}
		}

		k = Goa_strtok_r(NULL, tokenSeperators, &kPtr);
	}

	ServerParsePlayerCount(server, savedkeyvals);
}


/* ServerGetPing
----------------
Returns the ping for the specified server. */
int ServerGetPing(GServer server)
{
	return server->ping;
}

/* ServerGetAddress
-------------------
Returns the string, dotted IP address for the specified server */
char *ServerGetAddress(GServer server)
{
	return server->ip;
}

/* ServerGetPort
----------------
Returns the "query" port for the specified server. */
int ServerGetQueryPort(GServer server)
{
	return server->port;
}

GKeyValuePair *ServerRuleLookup(GServer server, char *key)
{
	GKeyValuePair kvp;
	kvp.key = key;
	return (GKeyValuePair *)TableLookup(server->keyvals, &kvp);
}

/* ServerGet[]Value
------------------
Returns the value for the specified key. */
char *ServerGetStringValue(GServer server, char *key, char *sdefault)
{
	GKeyValuePair *kv;

	if (strcmp(key,"hostaddr") == 0) //ooh! they want the hostaddr!
		return ServerGetAddress(server);
	kv = ServerRuleLookup(server,key);
	if (!kv)
		return sdefault;
	return kv->value;
}

int ServerGetIntValue(GServer server, char *key, int idefault)
{
	GKeyValuePair *kv;

	if (strcmp(key,"ping") == 0) //ooh! they want the ping!
		return ServerGetPing(server);
	kv = ServerRuleLookup(server,key);
	if (!kv)
		return idefault;
	return atoi(kv->value);

}

double ServerGetFloatValue(GServer server, char *key, double fdefault)
{
	GKeyValuePair *kv;

	kv = ServerRuleLookup(server,key);
	if (!kv)
		return fdefault;
	return atof(kv->value);
}

char *ServerGetPlayerStringValue(GServer server, int playernum, char *key, char *sdefault)
{
	char newkey[32];
	
	sprintf(newkey,"%s_%d",key,playernum);
	return ServerGetStringValue(server, newkey, sdefault);
}

int ServerGetPlayerIntValue(GServer server, int playernum, char *key, int idefault)
{
	char newkey[32];
	
	sprintf(newkey,"%s_%d",key,playernum);
	return ServerGetIntValue(server, newkey, idefault);
}

double ServerGetPlayerFloatValue(GServer server, int playernum, char *key, double fdefault)
{
	char newkey[32];
	
	sprintf(newkey,"%s_%d",key,playernum);
	return ServerGetFloatValue(server, newkey, fdefault);
}


/* ServerEnumKeys
-----------------
Enumerates the keys/values for a given server by calling KeyEnumFn with each
key/value. The user-defined instance data will be passed to the KeyFn callback */

static void KeyMap(void *elem, void *clientData)
{
	GKeyValuePair *kv = (GKeyValuePair *)elem;
	GEnumData *ped = (GEnumData *)clientData;

	ped->EnumFn(kv->key, kv->value, ped->instance);
}


void ServerEnumKeys(GServer server, KeyEnumFn KeyFn, void *instance)
{
	GEnumData ed;

	ed.EnumFn = KeyFn;
	ed.instance = instance;
	TableMap(server->keyvals, KeyMap, &ed);
}


/***********
 * UTILITY FUNCTIONS
 **********/

static char safetolower(char ch)
{
	return (isascii(ch) ? tolower(ch) : ch);
}

/* NonTermHash
 * ----------
 * The hash code is computed using a method called "linear congruence." 
 * This hash function has the additional feature of being case-insensitive,
 */
#define MULTIPLIER -1664117991
static int KeyValHash(const void *elem, int numbuckets)
{
	unsigned int i;
	unsigned long hashcode = 0;
	char *s = ((GKeyValuePair *)elem)->key;

	for (i = 0; i < strlen(s); i++)
		hashcode = hashcode * MULTIPLIER + safetolower(s[i]);
	return (hashcode % numbuckets);
}

/* keyval
 * Compares two gkeyvaluepair  (case insensative)
 */
static int KeyValCompare(const void *entry1, const void *entry2)
{
	return CaseInsensitiveCompare(&((GKeyValuePair *)entry1)->key, 
					  &((GKeyValuePair *)entry2)->key);
}

/* CaseInsensitiveCompare
 * ----------------------
 * Comparison function passed to qsort to sort an array of
 * strings in alphabetical order. It uses strcasecmp which is
 * identical to strcmp, except that it doesn't consider case of the
 * characters when comparing them, thus it sorts case-insensitively.
 */
static int CaseInsensitiveCompare(const void *entry1, const void *entry2)
{
	return strcasecmp(*(char **)entry1,*(char **)entry2);
}

/* KeyValFree
 * Frees the memory INSIDE a GKeyValuePair structure
 */
static void KeyValFree(void *elem)
{
	free(((GKeyValuePair *)elem)->key);
	free(((GKeyValuePair *)elem)->value);
}
