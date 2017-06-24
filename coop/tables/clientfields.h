/*
 * =======================================================================
 *
 * Fields of the client to be saved.
 *
 * =======================================================================
 */

{"pers.weapon", CLOFS(pers.weapon), F_ITEM},
{"pers.lastweapon", CLOFS(pers.lastweapon), F_ITEM},
{"pers.lastweapon2", CLOFS(pers.lastweapon2), F_ITEM}, /* FS: Zaero specific */
{"newweapon", CLOFS(newweapon), F_ITEM},
{"owned_sphere", CLOFS(owned_sphere), F_EDICT}, /* FS: Coop: Rogue specific */
{"", CLOFS(zCameraTrack), F_EDICT}, /* FS: Zaero specific */
{"", CLOFS(zCameraLocalEntity), F_EDICT}, /* FS: Zaero specific */
{NULL, 0, F_INT}
