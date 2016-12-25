#include "g_local.h"
#include "m_player.h"

/* FS: Flags for sv_vote_disallow_flags */
#define	VOTE_NOGAMEMODE			0x00000001
#define	VOTE_NOVANILLA			0x00000002
#define	VOTE_NOXATRIX			0x00000004
#define	VOTE_NOROGUE			0x00000008
#define	VOTE_NOZAERO			0x00000010
#define	VOTE_NOCOOPSKILL		0x00000020
#define	VOTE_NOMAP				0x00000040
#define	VOTE_NORANDOMMAPS		0x00000080
#define VOTE_NORESETMAP			0x00000100

/* Globals */
int bVoteInProgress;
int voteNo;
int voteYes;
int voteClients;
int printOnce;
int voteCoopSkill;
char voteCbufCmdExecute[MAX_OSPATH];
char voteMap[MAX_OSPATH];
char voteType[16];
char whatAreWeVotingFor[MAX_OSPATH];
char voteGamemode[16];
float voteTimer;

#define MAX_RANDOM_HISTORY 6 /* FS: Remember the last 6 random maps chosen */
char random_map_array[MAX_RANDOM_HISTORY][MAX_OSPATH];
int random_map_head = 0, random_map_tail = 0;
int random_map_index = 0;

/* Prototypes */
void vote_map (edict_t *ent, const char *mapName);
void vote_gamemode(edict_t *ent, const char *gamemode);
void vote_coopskill(edict_t *ent, int skillVote);
void vote_random (edict_t *ent);
void vote_restartmap (edict_t *ent);
void vote_stop (edict_t *ent);
void vote_yes (edict_t *ent, qboolean bAssume);
void vote_no (edict_t *ent);
void vote_disconnect_recalc(edict_t *ent);
void vote_Passed (void);
void vote_Failed (qboolean bTied);
void vote_Reset (void);
void vote_Broadcast(const char *fmt, ...);
void vote_Think (void);
void Vote_Random_History_Add (const char *mapname);
void Vote_Clear_Random_History(void);
qboolean Vote_Check_Random_History(const char *mapname);
void vote_DefaultNoVotes (void);
void vote_progress (edict_t *ent);
qboolean vote_mapcheck (edict_t *ent, const char *mapName);
void vote_warp (edict_t *ent, const char *mapName);

void vote_command(edict_t *ent)
{
	int argc = 0;

	argc = gi.argc();

	if (maxclients->value <= 1.0f)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting is only available in multiplayer.\n");
		return;
	}

	if (!sv_vote_enabled->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting disabled on this server!\n");
		return;
	}

	if (ent->client->pers.spectator)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting is not allowed by spectators.\n");
		return;
	}

	if(level.intermissiontime)
	{
		gi.cprintf(ent, PRINT_HIGH, "You can't vote during an intermission!\n");
		return;
	}

	if (argc <= 1 ||
	    (argc >= 2 && (!Q_stricmp(gi.argv(1), "help") || !Q_stricmp(gi.argv(1), "list") || !Q_stricmp(gi.argv(1), "cmds") || !Q_stricmp(gi.argv(1), "commands"))) )
	{
		gi.cprintf(ent, PRINT_HIGH, "usage: vote map <mapname>, vote gamemode <gamemode>, vote skill <coopskill>, vote restartmap, vote yes, vote no, vote stop, and vote progress.\n");
		return;
	}
	if(!Q_stricmp(gi.argv(1), "map"))
	{
		if(argc <= 2)
		{
			gi.cprintf(ent, PRINT_HIGH, "error: you must supply a map name! i.e. vote map base1\n");
			return;
		}
		else
		{
			vote_map(ent, gi.argv(2));
			return;
		}
	}
	else if(!Q_stricmp(gi.argv(1), "restartmap"))
	{
		vote_restartmap(ent);
		return;
	}
	else if(!Q_stricmp(gi.argv(1), "gamemode"))
	{
		if(argc <= 2)
		{
			gi.cprintf(ent, PRINT_HIGH, "error: you must supply a gamemode!  valid options are: dm, tdm, ctf, ra, dt, instagib, and coop.\n");
			return;
		}
		else
		{
			vote_gamemode(ent,gi.argv(2));
			return;
		}
	}
	else if(!Q_stricmp(gi.argv(1), "skill"))
	{
		if(argc <= 2)
		{
			gi.cprintf(ent, PRINT_HIGH, "error: you must supply a coop difficulty level!  valid options are: 0 (easy), 1 (medium), 2 (hard), and 3 (nightmare).\n");
			return;
		}
		else
		{
			vote_coopskill(ent,atoi(gi.argv(2)));
			return;
		}
	}
	else if(!Q_stricmp(gi.argv(1), "yes"))
	{
		vote_yes(ent, false);
		return;
	}
	else if(!Q_stricmp(gi.argv(1), "no"))
	{
		vote_no(ent);
		return;
	}
	else if(!Q_stricmp(gi.argv(1), "stop"))
	{
		vote_stop(ent);
		return;
	}
/* FS: Not ready yet... */
//	else if (!Q_stricmp(gi.argv(1), "random"))
//	{
//		vote_random(ent);
//		return;
//	}
	else if (!Q_stricmp(gi.argv(1), "warp"))
	{
		if(argc <= 2)
		{
			gi.cprintf(ent, PRINT_HIGH, "error: you must supply a map name! i.e. vote warp base1\n");
			return;
		}
		else
		{
			vote_warp(ent, gi.argv(2));
			return;
		}
	}
	else if (!Q_stricmp(gi.argv(1), "progress"))
	{
		vote_progress(ent);
		return;
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Unknown vote command: %s.  ", gi.argv(1));
		gi.cprintf(ent, PRINT_HIGH, "valid options are: vote map <mapname>, vote gamemode <gamemode>, vote skill <coopskill>, vote restartmap, vote yes, vote no, vote stop, and vote progress.\n");
		return;
	}
}

qboolean vote_mapcheck (edict_t *ent, const char *mapName)
{
	char mapCheck[MAX_QPATH];
	char fileName[MAX_OSPATH];
	char *mapToken = NULL;
	char *mapToken2 = NULL;
	char *fileBuffer = NULL;
	char *fileBuffer2 = NULL;
	char *listPtr = NULL;
	char *listPtr2 = NULL;
	char separators[] = ",\n";
	char separators2[] = ",";
	long fileSize;
	int mapCount = 0;
	FILE *f = NULL;
	size_t toEOF = 0;
	qboolean retval = false;

	if(!ent || !mapName)
	{
		return false;
	}

	if(sv_coop_maplist->string[0] == 0)
	{
		gi.cprintf(NULL, PRINT_CHAT, "vote_mapcheck: sv_coop_maplist CVAR empty!\n");
		return false;
	}

	Sys_Mkdir(va("%s/maps", gamedir->string));
	Com_sprintf(fileName, sizeof(fileName), "%s/%s", gamedir->string, sv_coop_maplist->string);

	f = fopen(fileName, "r");
	if(!f)
	{
		gi.cprintf(NULL, PRINT_CHAT, "vote_mapcheck: couldn't find '%s'!\n", sv_coop_maplist->string);
		return false;
	}

	/* obtain file size */
	fseek (f, 0, SEEK_END);
	fileSize = ftell (f);
	fseek (f, 0, SEEK_SET);
	fileBuffer = (char *)malloc(sizeof(char)*(fileSize+2)); // FS: In case we have to add a newline terminator
	if(!fileBuffer)
	{
		gi.cprintf(NULL, PRINT_CHAT, "vote_mapcheck: can't allocate memory for fileBuffer!\n");
		fclose(f);
		return false;
	}
	toEOF = fread(fileBuffer, sizeof(char), fileSize, f);
	fclose(f);
	if(toEOF <= 0)
	{
		gi.cprintf(NULL, PRINT_CHAT, "vote_mapcheck: cannot read file '%s' into memory!\n", sv_coop_maplist->string);
		return false;
	}

	/* FS: Add newline terminator for some paranoia */
	fileBuffer[toEOF] = '\n';
	fileBuffer[toEOF+1] = '\0';

	fileBuffer2 = strdup(fileBuffer);

	mapToken = strtok_r(fileBuffer, separators, &listPtr);
	mapToken2 = strtok_r(fileBuffer2, "\n", &listPtr2);
	if(!mapToken || !mapToken2)
	{
		retval = false;
	}

	Com_sprintf(mapCheck, sizeof(mapCheck), "%s.bsp", mapName);

	while(mapToken)
	{
		if(!Q_stricmp(mapToken, mapCheck))
		{
			size_t mapTokenSize = strlen(mapToken);
			while(mapToken2)
			{
				if(!strncmp(mapToken, mapToken2, mapTokenSize))
				{
					mapToken2 = strtok_r(mapToken2, ",", &listPtr2);
					mapToken2 = strtok_r(NULL, "\n", &listPtr2);
					if(mapToken2)
					{
						Com_sprintf(voteGamemode, sizeof(voteGamemode), "%s", mapToken2);
						retval = true;
						goto cleanup;
					}
					else
					{
						gi.cprintf(NULL, PRINT_CHAT, "vote_mapcheck: %s with no gamemode in '%s'!\n", mapCheck, sv_coop_maplist->string);
						retval = false;
						goto cleanup;
					}
				}
				mapToken2 = strtok_r(NULL, "\n", &listPtr2);
			}
		}
		mapToken = strtok_r(NULL, separators, &listPtr);
	}
cleanup:
	if(fileBuffer)
		free(fileBuffer);
	if(fileBuffer2)
		free(fileBuffer2);

	return retval;
}

void vote_map (edict_t *ent, const char *mapName)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_map from a non-player!\n");
	}

	if (bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is already in progress for %s: %s!\n", voteType, whatAreWeVotingFor);
		return;
	}

	vote_Reset();

	if(strstr(mapName, "."))
	{
		gi.cprintf(ent, PRINT_HIGH, "Map name can not contain '.'\n");
		return;
	}
	else if(strstr(mapName, ";"))
	{
		gi.cprintf(ent, PRINT_HIGH, "Map name can not contain ';'\n");
		return;
	}

	if(sv_vote_disallow_flags->intValue & VOTE_NOMAP)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting for map changes are not allowed on this server.  Vote cancelled.\n");
		return;
	}

	if(!vote_mapcheck(ent,mapName))
	{
		gi.cprintf(ent, PRINT_HIGH, "Map %s doesn't exist!\n", mapName);
		return;
	}

	Com_sprintf(voteMap, MAX_OSPATH, "%s", mapName);
	vote_Broadcast("%s votes for %s! Use vote yes or vote no to submit your vote!\n", ent->client->pers.netname, voteMap);
	voteClients = P_Clients_Connected(false);
	bVoteInProgress = 1;
	Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "%s", voteMap);
	Com_sprintf(voteType, 16, "map");

	ent->voteInitiator = true;

	if(sv_vote_assume_yes->intValue)
	{
		vote_yes(ent, true); /* FS: I assume you would want to vote yes if you initiated the vote. */
	}

	return;
}

void vote_warp (edict_t *ent, const char *mapName)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_warp from a non-player!\n");
	}

	if (bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is already in progress for %s: %s!\n", voteType, whatAreWeVotingFor);
		return;
	}

	vote_Reset();

	if(strstr(mapName, "."))
	{
		gi.cprintf(ent, PRINT_HIGH, "Map name can not contain '.'\n");
		return;
	}
	else if(strstr(mapName, ";"))
	{
		gi.cprintf(ent, PRINT_HIGH, "Map name can not contain ';'\n");
		return;
	}

	if(sv_vote_disallow_flags->intValue & VOTE_NOMAP)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting for map changes are not allowed on this server.  Vote cancelled.\n");
		return;
	}

	if(!vote_mapcheck(ent,mapName))
	{
		gi.cprintf(ent, PRINT_HIGH, "Map %s doesn't exist!\n", mapName);
		return;
	}
	else
	{
		if (Q_stricmp(sv_coop_gamemode->string, voteGamemode))
		{
			gi.cprintf(ent, PRINT_HIGH, "Game mode is %s but Map is for game mode %s.  Vote cancelled!\n", sv_coop_gamemode->string, voteGamemode);
			return;
		}
	}

	Com_sprintf(voteMap, MAX_OSPATH, "%s", mapName);
	vote_Broadcast("%s votes for warping to %s! Use vote yes or vote no to submit your vote!\n", ent->client->pers.netname, voteMap);
	voteClients = P_Clients_Connected(false);
	bVoteInProgress = 1;
	Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "%s", voteMap);
	Com_sprintf(voteType, 16, "warp");

	ent->voteInitiator = true;

	if(sv_vote_assume_yes->intValue)
	{
		vote_yes(ent, true); /* FS: I assume you would want to vote yes if you initiated the vote. */
	}

	return;
}

void vote_gamemode(edict_t *ent, const char *gamemode)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_gamemode from a non-player!\n");
	}

	if (bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is already in progress for %s: %s!\n", voteType, whatAreWeVotingFor);
		return;
	}

	vote_Reset();

	if(sv_vote_disallow_flags->intValue & VOTE_NOGAMEMODE)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting for gamemode changes are not allowed on this server.  Vote cancelled.\n");
		return;
	}

	if (!Q_stricmp((char *)gamemode, "vanilla"))
	{
		if(sv_vote_disallow_flags->intValue & VOTE_NOVANILLA)
		{
			gi.cprintf(ent, PRINT_HIGH, "Voting for Vanilla gamemode change is not allowed on this server.  Vote cancelled.\n");
			return;
		}

		Com_sprintf(voteGamemode, 16, "vanilla");
	}
	else if (!Q_stricmp((char *)gamemode, "xatrix"))
	{
		if(sv_vote_disallow_flags->intValue & VOTE_NOXATRIX)
		{
			gi.cprintf(ent, PRINT_HIGH, "Voting for Xatrix gamemode change is not allowed on this server.  Vote cancelled.\n");
			return;
		}

		Com_sprintf(voteGamemode, 16, "xatrix");
	}
	else if (!Q_stricmp((char *)gamemode, "rogue"))
	{
		if(sv_vote_disallow_flags->intValue & VOTE_NOROGUE)
		{
			gi.cprintf(ent, PRINT_HIGH, "Voting for Rogue gamemode change is not allowed on this server.  Vote cancelled.\n");
			return;
		}

		Com_sprintf(voteGamemode, 16, "rogue");
	}
	else if (!Q_stricmp((char *)gamemode, "zaero"))
	{
		if(sv_vote_disallow_flags->intValue & VOTE_NOZAERO)
		{
			gi.cprintf(ent, PRINT_HIGH, "Voting for Rogue gamemode change is not allowed on this server.  Vote cancelled.\n");
			return;
		}

		Com_sprintf(voteGamemode, 16, "zaero");
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "error: invalid gamemode!  valid options are: vanilla, xatrix, and rogue.\n");
		return;
	}

	voteClients = P_Clients_Connected(false);
	bVoteInProgress = 1;
	voteClients = P_Clients_Connected(false);
	Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "%s", voteGamemode);
	Com_sprintf(voteType, 16, "gamemode");
	vote_Broadcast("%s votes for %s: %s! Use vote yes or vote no to submit your vote!\n", ent->client->pers.netname, voteType, whatAreWeVotingFor);

	if(sv_vote_assume_yes->intValue)
		vote_yes(ent, true); /* FS: I assume you would want to vote yes if you initiated the vote. */

	ent->voteInitiator = true;
}

void vote_coopskill(edict_t *ent, int skillVote)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_coopskill from a non-player!\n");
		return;
	}

	if (bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is already in progress for %s: %s!\n", voteType, whatAreWeVotingFor);
		return;
	}

	vote_Reset();

	if(!coop->intValue)
	{
		gi.cprintf(ent, PRINT_HIGH, "error: not in a coop game!\n");
		return;
	}

	if(sv_vote_disallow_flags->intValue & VOTE_NOCOOPSKILL)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting for coop skill changes are not allowed on this server.  Vote cancelled.\n");
		return;
	}

	switch(skillVote)
	{
		case 0:
			voteCoopSkill = 0;
			Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "easy");
			break;
		case 1:
			voteCoopSkill = 1;
			Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "medium");
			break;
		case 2:
			voteCoopSkill = 2;
			Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "hard");
			break;
		case 3:
			voteCoopSkill = 3;
			Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "nightmare");
			break;
		default:
			gi.cprintf(ent, PRINT_HIGH, "error: invalid coop difficulty level!  valid options are: 0 (easy), 1 (medium), 2 (hard), and 3 (nightmare).\n");
			return;
	}

	if(skill->intValue == skillVote)
	{
		gi.cprintf(ent, PRINT_HIGH, "error: skill is already %s!  Vote cancelled.\n", whatAreWeVotingFor);
		return;
	}

	voteClients = P_Clients_Connected(false);
	bVoteInProgress = 1;
	voteClients = P_Clients_Connected(false);
	Com_sprintf(voteType, 16, "coop difficulty");
	vote_Broadcast("%s votes for %s: %s! Use vote yes or vote no to submit your vote!\n", ent->client->pers.netname, voteType, whatAreWeVotingFor);

	if(sv_vote_assume_yes->intValue)
	{
		vote_yes(ent, true); /* FS: I assume you would want to vote yes if you initiated the vote. */
	}

	ent->voteInitiator = true;
}

void vote_random (edict_t *ent)
{
	/* FS: TODO: Not ready yet... */
}

void vote_restartmap (edict_t *ent)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_map from a non-player!\n");
	}

	if (bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is already in progress for %s: %s!\n", voteType, whatAreWeVotingFor);
		return;
	}

	vote_Reset();

	if(sv_vote_disallow_flags->intValue & VOTE_NORESETMAP)
	{
		gi.cprintf(ent, PRINT_HIGH, "Voting for map restarts are not allowed on this server.  Vote cancelled.\n");
		return;
	}

	Com_sprintf(voteMap, MAX_OSPATH, "%s", level.mapname);
	vote_Broadcast("%s votes for restarting the map! Use vote yes or vote no to submit your vote!\n", ent->client->pers.netname);
	voteClients = P_Clients_Connected(false);
	bVoteInProgress = 1;
	Com_sprintf(whatAreWeVotingFor, MAX_OSPATH, "%s", voteMap);
	Com_sprintf(voteType, 16, "restartmap");

	ent->voteInitiator = true;

	if(sv_vote_assume_yes->intValue)
	{
		vote_yes(ent, true); /* FS: I assume you would want to vote yes if you initiated the vote. */
	}
}

void vote_stop(edict_t *ent)
{
	if(!ent || !ent->client) /* FS: This should never happen, but you never know... */
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_stop from a non-player!\n");
		return;
	}

	if (!bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "No vote in progress!\n");
		return;
	}

	if(ent->voteInitiator == true)
	{
		vote_Broadcast("Voting cancelled by %s!\n", ent->client->pers.netname);
		vote_Reset();
		return;
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "You can't cancel a vote you didn't initiate!\n");
		return;
	}
}

void vote_yes(edict_t *ent, qboolean bAssume)
{
	if(!ent || !ent->client) /* FS: This should never happen, but you never know... */
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_yes from a non-player!\n");
		return;
	}

	if (!bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "No vote in progress, ");
		gi.cprintf(ent, PRINT_HIGH, "use vote map <mapname>, vote gamemode <gamemode>, vote skill <coopskill>, vote fraglimit <fraglimit>, or vote timelimit <timelimit> to start a vote!\n");
		return;
	}

	if(ent->hasVoted)
	{
		gi.cprintf(ent, PRINT_HIGH, "You have already voted!\n");
		return;
	}
	else
	{
		/* FS: count a vote... */
		ent->hasVoted = VOTE_YES;
		voteYes++;
		if(sv_vote_private->intValue && !bAssume)
			gi.cprintf(ent, PRINT_HIGH, "Your vote 'yes' for %s has been counted\n", whatAreWeVotingFor);
		else if (!bAssume)
			vote_Broadcast("%s votes yes. Yes: %i, No: %i.\n", ent->client->pers.netname, voteYes, voteNo);
		vote_Think();
		return;
	}
}

void vote_no(edict_t *ent)
{
	if(!ent || !ent->client) /* FS: This should never happen, but you never know... */
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_no from a non-player!\n");
		return;
	}

	if (!bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "No vote in progress, ");
		gi.cprintf(ent, PRINT_HIGH, "use vote map <mapname>, vote gamemode <gamemode>, vote skill <coopskill>, vote fraglimit <fraglimit>, or vote timelimit <timelimit> to start a vote!\n");
		return;
	}

	if(ent->hasVoted)
	{
		gi.cprintf(ent, PRINT_HIGH, "You have already voted!\n");
		return;
	}
	else
	{
		/* FS: count a vote... */
		ent->hasVoted = VOTE_NO;
		voteNo++;

		if(sv_vote_private->intValue)
		{
			gi.cprintf(ent, PRINT_HIGH, "Your vote 'no' for %s has been counted\n", whatAreWeVotingFor);
		}
		else
		{
			vote_Broadcast("%s votes no. Yes: %i, No: %i.\n", ent->client->pers.netname, voteYes, voteNo);
		}

		vote_Think();
		return;
	}
}

void vote_disconnect_recalc(edict_t *ent)
{
	if(!ent || !ent->client) /* FS: This should never happen, but you never know... */
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_disconnect_recalc from a non-player!\n");
		return;
	}

	if (!bVoteInProgress)
	{
		return;
	}

	if (ent->hasVoted)
	{
		if ((ent->hasVoted == VOTE_YES) && (voteYes))
		{
			voteYes--;
		}
		else if ((ent->hasVoted == VOTE_NO) && (voteNo))
		{
			voteNo--;
		}
	}
}

void vote_Reset(void)
{
	int z;
	edict_t *pClient;

	voteClients = voteNo = voteYes = voteCoopSkill = 0;
	voteTimer = level.time + sv_vote_timer->value;
	bVoteInProgress = 0;
	printOnce = 0;
	whatAreWeVotingFor[0] = voteMap[0] = voteGamemode[0] = voteType[0] = '\0';

	/* FS: Don't do this in single player */
	if (game.maxclients <= 1)
	{
		return;
	}

	/* FS: Reset the hasVoted qbooleanean. */
	for(z = 0; z < game.maxclients; z++ )
	{
		pClient = &g_edicts[z + 1];
		if( !pClient->inuse )
		{
			continue;
		}

		if( pClient->client )
		{
			pClient->hasVoted = NOT_VOTED;
			pClient->voteInitiator = false;
		}
	}

}

void vote_Passed (void)
{
	vote_Broadcast("Vote passed for %s: %s! Yes: %i, No: %i\n", voteType, whatAreWeVotingFor, voteYes, voteNo);

	if (!Q_stricmp(voteType, "gamemode"))
	{
		if (!Q_stricmp(voteGamemode, "vanilla"))
		{
			gi.cvar_forceset("sv_coop_gamemode", "vanilla");
			Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "deathmatch 0; coop 1; wait;wait;wait;wait;wait;map base1\n");
		}
		else if(!Q_stricmp(voteGamemode, "xatrix"))
		{
			gi.cvar_forceset("sv_coop_gamemode", "xatrix");
			Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "deathmatch 0; coop 1; wait;wait;wait;wait;wait;map xswamp\n");
		}
		else if(!Q_stricmp(voteGamemode, "rogue"))
		{
			gi.cvar_forceset("sv_coop_gamemode", "rogue");
			Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "deathmatch 0; coop 1; wait;wait;wait;wait;wait;map rmine1\n");
		}
		else if(!Q_stricmp(voteGamemode, "zaero"))
		{
			gi.cvar_forceset("sv_coop_gamemode", "zaero");
			Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "deathmatch 0; coop 1; wait;wait;wait;wait;wait;map zbase1\n");
		}
	}
	else if(!Q_stricmp(voteType, "coop difficulty"))
	{
		Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "skill %i; wait;wait;wait;wait;wait;map %s", voteCoopSkill, level.mapname);
	}
	else if(!Q_stricmp(voteType, "restartmap"))
	{
		Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "wait;wait;wait;wait;wait;map %s\n", voteMap); /* FS: Might want to force a DLL unload for coop overflow fuckery */
	}
	else if(!Q_stricmp(voteType, "warp"))
	{
		Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "wait;wait;wait;wait;wait;gamemap %s\n", voteMap);
	}
	else
	{
		gi.cvar_forceset("sv_coop_gamemode", voteGamemode);
		Com_sprintf(voteCbufCmdExecute, MAX_OSPATH, "wait;wait;wait;wait;wait;map %s\n", voteMap);
	}

	vote_Reset();

	gi.AddCommandString(voteCbufCmdExecute);
	return;
}

void vote_Failed (qboolean bTied)
{
	if(bTied)
	{
		vote_Broadcast("Vote ended in a tie for %s, not changing the %s! Yes: %i, No: %i\n", whatAreWeVotingFor, voteType, voteYes, voteNo);
	}
	else
	{
		vote_Broadcast("Vote failed for %s! Yes: %i, No: %i\n", whatAreWeVotingFor, voteYes, voteNo);
	}

	vote_Reset();
	return;
}

void vote_Broadcast(const char *fmt, ...)
{
	int z;
	int clients;
	edict_t *pClient;
	char        msg[1024];
	va_list     argptr;

	va_start (argptr,fmt);
	Q_vsnprintf(msg, sizeof(msg), fmt, argptr); /* Knightmare 10/28/12- buffer-safe version */
	va_end (argptr);

	clients = 0;

	if(dedicated->intValue)
	{
		gi.cprintf(NULL, PRINT_CHAT, "%s", msg); /* FS: Send it to dedicated console too. */
	}

	for(z = 0; z < game.maxclients; z++ )
	{
		pClient = &g_edicts[z + 1];
		if( !pClient->inuse )
		{
			continue;
		}

		if(pClient->client)
		{
			gi.cprintf(pClient, PRINT_CHAT, msg);
			gi.sound(pClient, CHAN_AUTO, gi.soundindex("misc/talk.wav"), 1, ATTN_NORM, 0);
		}
	}
}

void vote_Decide(void)
{
	/* FS: tally votes and decide something. */
	if(voteNo == 0 && voteYes == 0) /* FS: This isn't a true tie, nobody voted! */
	{
		vote_Failed(false);
		return;
	}

	if(voteYes > voteNo)
	{
		vote_Passed();
		return;
	}

	if (voteNo > voteYes)
	{
		vote_Failed(false);
		return;
	}

	if (voteNo == voteYes) /* FS: Tie is a failure */
	{
		vote_Failed(true);
		return;
	}

	return;
}

void vote_Think(void)
{
	if(!bVoteInProgress || !sv_vote_enabled->intValue)
		return;

	voteClients = P_Clients_Connected(false);

	if(voteNo + voteYes >= voteClients)
	{
		vote_Decide();
		return;
	}

	if (voteYes > (voteClients / 2)) /* FS: Greater than 50% yes votes?  Finish it up */
	{
		vote_Decide();
		return;
	}

	if (level.time > voteTimer)
	{
		if(voteNo + voteYes < voteClients) /* FS: Timer expired and not enough votes?  Count those who did not vote as No votes. */
		{
			vote_DefaultNoVotes();
		}
		vote_Decide();
		return;
	}
	else if(level.time + 10.0f >= voteTimer && level.time + 9.0f <= voteTimer && printOnce == 0)
	{
		vote_Broadcast("10 seconds remaining for %s vote: %s.\n", voteType, whatAreWeVotingFor);
		printOnce = 1;
		return;
	}
	else if(level.time + 5.0f >= voteTimer && level.time + 4.0f <= voteTimer && printOnce == 1)
	{
		vote_Broadcast("5 seconds remaining for %s vote: %s.\n", voteType, whatAreWeVotingFor);
		printOnce = 2;
		return;
	}
	else if(level.time + 4.0f >= voteTimer && level.time + 3.0f <= voteTimer && printOnce == 2)
	{
		vote_Broadcast("4 seconds remaining for %s vote: %s.\n", voteType, whatAreWeVotingFor);
		printOnce = 3;
		return;
	}
	else if(level.time + 3.0f >= voteTimer && level.time + 2.0f <= voteTimer && printOnce == 3)
	{
		vote_Broadcast("3 seconds remaining for %s vote: %s.\n", voteType, whatAreWeVotingFor);
		printOnce = 4;
		return;
	}
	else if(level.time + 2.0f >= voteTimer && level.time + 1.0f <= voteTimer && printOnce == 4)
	{
		vote_Broadcast("2 seconds remaining for %s vote: %s.\n", voteType, whatAreWeVotingFor);
		printOnce = 5;
		return;
	}
	else if(level.time + 1.0f >= voteTimer && level.time + 0.0f <= voteTimer && printOnce == 5)
	{
		vote_Broadcast("1 second remaining for %s vote: %s.\n", voteType, whatAreWeVotingFor);
		printOnce = 0;
		return;
	}

}

void vote_connect (edict_t *ent)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_connect from a non-player!\n");
		return;
	}

	if(bVoteInProgress && !ent->hasVoted)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is in progress for %s: %s. Use vote yes or vote no to submit your vote!\n", voteType, whatAreWeVotingFor);
	}
}

void Vote_Random_History_Add (const char *mapname)
{
	if(!mapname || mapname[0] == 0)
		return;

	/* Taniwha's ring array */
	strcpy (random_map_array[random_map_head], mapname);
	random_map_head = (random_map_head + 1) % MAX_RANDOM_HISTORY;
	if (random_map_head == random_map_tail)
		random_map_tail = (random_map_tail + 1) % MAX_RANDOM_HISTORY;
//	random_map_array[random_map_head][0] = 0;
	random_map_index = random_map_index;
	/* end */
}

void Vote_Clear_Random_History(void)
{
	int i = MAX_RANDOM_HISTORY-1;
	while(1)
	{
		if (i < 0)
		{
			return;
		}
		random_map_array[i][0] = 0;
		i--;
	}
}

qboolean Vote_Check_Random_History(const char *mapname)
{
	int i = MAX_RANDOM_HISTORY-1;

	while(1)
	{
		if (i < 0)
		{
			return true;
		}
		if(random_map_array[i][0] != 0)
		{
			if(!strcmp(mapname, random_map_array[i]))
			{
				return false;
			}
		}
		i--;
	}
}

/* FS: Timer expired and not enough votes?  Count those who did not vote as No votes. */
void vote_DefaultNoVotes (void)
{
	int z;
	edict_t *pClient;

	for(z = 0; z < game.maxclients; z++ )
	{
		pClient = &g_edicts[z + 1];
		if( !pClient->inuse )
		{
			continue;
		}

		if( pClient->client )
		{
			if (pClient->client->pers.spectator) /* FS: Don't count spectators */
			{
				continue;
			}

			if(pClient->hasVoted)
			{
				continue;
			}

			pClient->hasVoted = VOTE_NO;
			voteNo++;
		}
	}
}

void vote_progress (edict_t *ent)
{
	if(!ent || !ent->client)
	{
		gi.dprintf(DEVELOPER_MSG_GAME, "Error: vote_NewPlayerMessage from a non-player!\n");
		return;
	}

	if (bVoteInProgress)
	{
		gi.cprintf(ent, PRINT_HIGH, "A vote is in progress for %s: %s! Yes; %i, No %i. Time remaining: %1.0f seconds.\n", voteType, whatAreWeVotingFor, voteYes, voteNo, voteTimer - level.time);
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "No vote in progress, ");
		gi.cprintf(ent, PRINT_HIGH, "use vote map <mapname>, vote gamemode <gamemode>, vote skill <coopskill>, vote fraglimit <fraglimit>, or vote timelimit <timelimit> to start a vote!\n");
	}
}
