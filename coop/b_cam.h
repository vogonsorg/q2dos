// b_coopcam.h
// blinky
// Created: 2000/06
// Edited:  2000/07/17
//

#ifndef B_CAM_H
#define B_CAM_H

struct BlinkyClient_s
{
	edict_t	*cam_target;
	edict_t *cam_decoy; // if camming, this is our sub
	// save some stuff when camming (to restore correctly)
	int runrun;
	float save_fov;
	int save_hand;
	int save_weapon;

	int nosummon; // disallow summons
	int nopickup;
};

typedef struct BlinkyClient_s BlinkyClient_t;

void Blinky_BeginClientThink(edict_t *ent, usercmd_t *ucmd);
void Blinky_EndClientThink(edict_t *ent, usercmd_t *ucmd);
void Blinky_BeginRunFrame();
void Blinky_ClientEndServerFrame(edict_t * ent);

void Cmd_Cam_f(edict_t *ent);
void Cmd_Stats_f(edict_t *ent);
void Cmd_Summon_f(edict_t *ent);
void Cmd_NoSummon_f(edict_t *ent);
void Cmd_Runrun_f(edict_t *ent);
void Blinky_OnClientTerminate(edict_t *self);
void Blinky_CalcViewOffsets(edict_t * ent, vec3_t v);
void Blinky_SpawnEntities();

void stopBlinkyCam (edict_t *ent);

extern void P_ProjectSource ( gclient_t * client , vec3_t point , vec3_t distance , vec3_t forward , vec3_t right , vec3_t result ) ;

#endif // B_CAM_H
