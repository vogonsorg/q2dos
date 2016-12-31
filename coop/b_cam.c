// b_coopcam.c
// blinky
// Created: 2000/06
// Edited:  2000/07/17
//

#include "g_local.h"

#define SCANNER_UNIT 10

static qboolean IsSpectator(edict_t * ent)
{
	if (!ent || !ent->inuse || !ent->client)
	{
		return false;
	}

	return ent->client->pers.spectator;
}

void Blinky_BeginClientThink(edict_t *ent, usercmd_t *ucmd)
{
	gclient_t * client = ent->client;
	BlinkyClient_t * bdata = &client->blinky_client;

	if (!ent || !ucmd)
	{
		return;
	}

	/* the runrun code - continuous running	 */
	if (bdata->runrun)
	{
		if (ucmd->forwardmove>0)
			bdata->runrun=1;
		else if (ucmd->forwardmove<0)
			bdata->runrun=2;
		else if (ucmd->sidemove>0)
			bdata->runrun=3;
		else if (ucmd->sidemove<0)
			bdata->runrun=4;
		switch(bdata->runrun)
		{
			case 1: ucmd->forwardmove = 400; break;
			case 2: ucmd->forwardmove = -400; break;
			case 3: ucmd->sidemove = 400; break;
			case 4: ucmd->sidemove = -400; break;
		}
	}
	else
	{
		if(bdata->cam_target) /* FS: Follow the player and ignore input */
		{
			int i = 0;

			ucmd->forwardmove = ucmd->sidemove = ucmd->upmove = 0;
			ucmd->buttons = 0;

			for (i = 0; i < 3; i++)
			{
				ent->client->ps.pmove.delta_angles[i] = ANGLE2SHORT(
						bdata->cam_target->client->v_angle[i] - ent->client->resp.cmd_angles[i]);
			}

			/* FS: FIXME: Interpolate this for ultimate smoothness */
			VectorCopy(bdata->cam_target->client->v_angle, ent->client->ps.viewangles);
			VectorCopy(bdata->cam_target->client->v_angle, ent->client->v_angle);
		}
	}

	if (bdata->cam_target && bdata->cam_target->inuse)
	{
		ent->client->ps.stats[STAT_CHASE] = CS_PLAYERSKINS +
								   (bdata->cam_target - g_edicts) - 1;
	}
	else
	{
		ent->client->ps.stats[STAT_CHASE] = 0;
	}
}

void Blinky_EndClientThink(edict_t *ent, usercmd_t *ucmd)
{
//	gclient_t * client = ent->client;
}

static void Blinky_BeginServerFrameClient(edict_t * ent)
{
	edict_t *target = NULL;
	edict_t *decoy = NULL;

	if (!ent || !ent->client)
	{
		return;
	}

	target = ent->client->blinky_client.cam_target;
	decoy = ent->client->blinky_client.cam_decoy;

	if (target && decoy)
	{
		decoy->s = ent->s;
	}
}

void Blinky_BeginRunFrame()
{
	int		i;
	edict_t	*ent;

	for (i=0 ; i<maxclients->value ; i++)
	{
		ent = g_edicts + 1 + i;

		if (!ent->inuse || !ent->client)
		{
			continue;
		}

		Blinky_BeginServerFrameClient(ent);
	}
}

void Blinky_ClientEndServerFrame(edict_t * ent)
{
//	gclient_t * client = ent->client;
}

static void
ShowStats(edict_t *ent, edict_t *player)
{
	vec3_t v;
	int j;
	char stats[500];
	BlinkyClient_t * bdata;
	char pname[11];
	int health, armor, armorpow;
	int xd,yd,zd;
	vec3_t dp, normal={0,0,-1};
	static int CellsIndex = -1;
	int index;

	if (!ent || !player)
	{
		return;
	}

	j = 0;
	if (-1 == CellsIndex)
		CellsIndex = ITEM_INDEX(FindItem("cells"));

	bdata = &ent->client->blinky_client;

	VectorSubtract(ent->s.origin, player->s.origin, v);
	zd = -v[2]/SCANNER_UNIT; // save height differential
	v[2] = 0; // remove height component

	RotatePointAroundVector(dp, normal, v, ent->s.angles[1]);
	xd = -dp[0]/SCANNER_UNIT;
	yd = dp[1]/SCANNER_UNIT;
	if (player->client)
		strncpy(pname, player->client->pers.netname, sizeof(pname)-1);
	else
		strncpy(pname, player->classname, sizeof(pname)-1);
	pname[sizeof(pname)-1] = 0;
	health = player->health;

	armorpow = 0;
	if (PowerArmorType(ent))
		armorpow = ent->client->pers.inventory[CellsIndex];

	index = ArmorIndex (ent);
	if (index)
		armor = ent->client->pers.inventory[index];

	if (armorpow)
		sprintf(stats+j, "%s: armor=%3d(%3d), health=%3d (f=%+5d,r=%+5d,u=%+5d)\n"
			, pname, armor,armorpow,health,xd, yd, zd);
	else
		sprintf(stats+j, "%s: armor=%3d, health=%3d (f=%+5d,r=%+5d,u=%+5d)\n"
			, pname, armor,health,xd, yd, zd);
	gi.cprintf(ent, PRINT_CHAT, "%s", stats);
}

void MoveToAngles(edict_t * ent, vec3_t pv1)
{
	int i;
	for (i=0 ; i<3 ; i++)
	{
		ent->client->ps.pmove.delta_angles[i] = ANGLE2SHORT(pv1[i] - ent->client->resp.cmd_angles[i]);
//		ent->client->ps.viewangles[i] = pv1[i];
	}
}

void stopBlinkyCam(edict_t * ent)
{
	BlinkyClient_t *bdata = NULL;
		
	if (!ent || !ent->client)
		return;

	bdata = &ent->client->blinky_client;
	if(!bdata)
	{
		return;
	}

	bdata->cam_target = 0;
	ent->svflags &= ~SVF_NOCLIENT;
	ent->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
	bdata->cam_decoy->svflags |= SVF_NOCLIENT;
	VectorCopy(bdata->cam_decoy->s.origin, ent->s.origin);
	MoveToAngles(ent, bdata->cam_decoy->s.angles);
	ent->client->ps.fov = bdata->save_fov;
	ent->client->pers.hand = bdata->save_hand;
	ent->client->ps.gunindex = bdata->save_weapon;

	gi.linkentity(ent);

	/* FS: Get rid of the decoy, free up some edicts */
	G_FreeEdict(bdata->cam_decoy);
	bdata->cam_decoy = NULL;
//	gi.linkentity(bdata->cam_decoy);
}

void StartCam(edict_t * ent, edict_t * target)
{
	BlinkyClient_t * bdata = &ent->client->blinky_client;
	edict_t * decoy = bdata->cam_decoy;

	bdata->cam_target = target;
	ent->svflags |= SVF_NOCLIENT;
	ent->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;
	if (!decoy)
	{
		bdata->cam_decoy = G_Spawn();
		decoy = bdata->cam_decoy;
		decoy->client = ent->client;
		decoy->s = ent->s;
	}
	bdata->cam_decoy->svflags &= ~SVF_NOCLIENT;
	VectorCopy(ent->s.origin, bdata->cam_decoy->s.origin);
	MoveToAngles(ent, target->s.angles);
	VectorCopy(ent->s.angles, bdata->cam_decoy->s.angles);
	bdata->save_fov = ent->client->ps.fov;
	bdata->save_hand = ent->client->pers.hand;
	bdata->save_weapon = ent->client->ps.gunindex; /* FS: Added */
	ent->client->pers.hand = CENTER_HANDED;
	if (target->client)
		ent->client->ps.fov = target->client->ps.fov;
	ent->client->ps.gunindex = 0;
	gi.linkentity(ent);
	gi.linkentity(bdata->cam_decoy);
}


void Cmd_Stats_f(edict_t *ent)
{
	char * name = gi.args();
	edict_t * player;

	if (!ent)
	{
		return;
	}

	for (player = &g_edicts[0]+1; player< &g_edicts[0]+(int)(maxclients->value)+1; player++)
	{
		if (!player->inuse || !player->client || player->client->pers.spectator)
			continue;
//		if (player == ent)
//			continue;
		// if a specific name requested & doesn't match, skip
		if (name[0])
		{
			if (Q_stricmp(name, player->client->pers.netname))
				continue;
		}
		ShowStats(ent, player);
	}
}


void Cmd_Cam_f(edict_t *ent)
{
	char * name = gi.args();
 // obj1 is how to tell when we've looped
	edict_t * obj1;
	edict_t * target;
	BlinkyClient_t * bdata;

	if (!ent || !ent->client || ent->client->pers.spectator)
	{
		return;
	}

	bdata = &ent->client->blinky_client;
	obj1 = bdata->cam_target;

	if (!coop->value || IsSpectator(ent))
		return;

	if (!obj1)
		obj1 = ent;
	target = obj1;
	while(1)
	{
		// advance loop thru edicts
		// to do - this ought to cycle at maxclients instead of num_edicts
		if (target < &g_edicts[0]+(int)(maxclients->value)+1)
			target++;
		else
			target = g_edicts+1;
		if (target == obj1)
			break;
		// only look at (in use) players
		if (!target->inuse || !target->client || IsSpectator(target))
			continue;
		// if a specific name requested & doesn't match, skip
		if (name[0] && Q_stricmp(name, target->client->pers.netname))
			continue;

		// found cam target
		if (target == ent)
		{
			stopBlinkyCam(ent);
		}
		else
		{
			StartCam(ent, target);
			ShowStats(ent, target);
		}
		break;
	}

}

static void Summon(edict_t *ent, edict_t *other)
{
	vec3_t offset, forward, right, start;
	trace_t		tr;

	if (!ent || !ent->client || !other || !other->client)
	{
		return;
	}

	if (other->client->blinky_client.nosummon)
	{
		gi.cprintf(ent, PRINT_HIGH, "Player \"%s\" has summon disabled!\n", other->client->pers.netname);
		return;
	}

	if (other->deadflag == DEAD_DEAD)
	{
		gi.cprintf(ent, PRINT_HIGH, "Player \"%s\" is dead, aborting summon!\n", other->client->pers.netname);
		return;
	}

	VectorSet(offset, 40, 0, ent->viewheight-8);
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	// code mostly copied from KillBox

	// unlink to make sure it can't possibly interfere with KillBox
	gi.unlinkentity (other);

	tr = gi.trace (start, other->mins, other->maxs, start, NULL, MASK_PLAYERSOLID);
	if (tr.fraction < 1.0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Collision issue.  Can not summon \"%s\".  Try from a different location.\n", other->client->pers.netname);
		gi.linkentity (other);
		return;
	}

	VectorCopy (start, other->s.origin);
	VectorCopy (start, other->s.old_origin);
	other->s.origin[2] += 10;

	// clear the velocity and hold them in place briefly
	VectorClear (other->velocity);
	other->client->ps.pmove.pm_time = 160>>3;		// hold time
	other->client->ps.pmove.pm_flags |= PMF_TIME_TELEPORT;

	// draw the teleport splash at source and on the player
	other->s.event = EV_PLAYER_TELEPORT;

	// set angles
	MoveToAngles(other, ent->s.angles);

	VectorClear (other->s.angles);
	VectorClear (other->client->ps.viewangles);
	VectorClear (other->client->v_angle);
	VectorClear (other->client->kick_angles);
	VectorClear (other->client->kick_origin);

	// kill anything at the destination
	KillBox (other);

	gi.cprintf(ent, PRINT_HIGH, "Summoned \"%s\"!\n", other->client->pers.netname);
	gi.cprintf(other, PRINT_HIGH, "Player \"%s\" has summoned you!  To disable this feature use /nosummon in console.\n", ent->client->pers.netname);

	gi.linkentity (other);

	/* FS: FIXME: Is this right? -- Do this again, view angles got jacked permanently a couple of times during live play? */
	VectorClear (other->s.angles);
	VectorClear (other->client->ps.viewangles);
	VectorClear (other->client->v_angle);
	VectorClear (other->client->kick_angles);
	VectorClear (other->client->kick_origin);
}


void Cmd_NoSummon_f(edict_t *ent)
{
	BlinkyClient_t *bdata = NULL;

	if (!ent || !ent->client || ent->client->pers.spectator)
	{
		return;
	}

	bdata = &ent->client->blinky_client;

	if (!bdata->nosummon)
	{
		gi.cprintf(ent, PRINT_HIGH, "Summon: Disabled!\n");
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Summon: Enabled!\n");
	}
	bdata->nosummon = !bdata->nosummon;
}

void Cmd_Runrun_f(edict_t *ent)
{
	int runrun;

	if (!ent || !ent->client || ent->client->pers.spectator)
	{
		return;
	}

	if (ent->client->blinky_client.runrun)
	{
		gi.cprintf(ent, PRINT_HIGH, "Auto-run: Disabled!\n");
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Auto-run: Enabled!\n");
	}

	runrun = !ent->client->blinky_client.runrun;
	ent->client->blinky_client.runrun = runrun;
}

void Cmd_Summon_f(edict_t *ent)
{
	char * name = gi.args();
	edict_t * target;
	int i;

	if (!ent || !ent->client || ent->client->pers.spectator)
	{
		return;
	}

	for (i = 1; i < (int)(maxclients->value)+1; i++)
	{
		target = &g_edicts[i];

		if (!target->inuse || !target->client || IsSpectator(target))
			continue;
		if (target == ent)
			continue;
		// if a specific name requested & doesn't match, skip
		if (name[0] && Q_stricmp(name, target->client->pers.netname))
			continue;

		Summon(ent, target);
		return;
	}

	if(name[0])
	{
		gi.cprintf(ent, PRINT_HIGH, "Couldn't find player \"%s\" to summon!\n", name);
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Couldn't find a player to summon!\n");
	}
}


void Blinky_OnClientTerminate(edict_t *self)
{
	edict_t * player;
	BlinkyClient_t * bdata;

	if (!self)
	{
		return;
	}

	/* disconnect any cams */
	for (player = &g_edicts[0]+1; player< &g_edicts[0]+(int)(maxclients->value)+1; player++)
	{
		if (!player->client || !player->inuse)
			continue;
		bdata = &player->client->blinky_client;
		if (bdata->cam_target == self)
			stopBlinkyCam(player);
	}
}

void Blinky_CalcViewOffsets(edict_t * ent, vec3_t v)
{
	int i;
	BlinkyClient_t * bdata = &ent->client->blinky_client;
	vec3_t forward;
	edict_t * target = bdata->cam_target;
	int offset = coop_cameraoffset->value;

	VectorSet(v,0,0,0);
	for (i=0; i<3; i++)
	{
		ent->client->ps.pmove.origin[i] = target->s.origin[i]*8;
	}
	VectorCopy(target->s.angles, ent->client->ps.viewangles);
	v[2] += target->viewheight;

	// move cam forward
	if (offset)
	{
		AngleVectors (target->client->v_angle, forward, NULL, NULL);
		VectorMA (v, offset, forward, v);
	}
}

void Blinky_SpawnEntities()
{
	
	gi.dprintf (DEVELOPER_MSG_GAME, "Blinky - CoopCam version of 3.20, 2000/08/23\n");
	gi.dprintf (DEVELOPER_MSG_GAME, "\x02""Use cam <name> to see through your buddy eyes, cam to turn back\n");
	
	srand( (unsigned)time( NULL ) );
}
