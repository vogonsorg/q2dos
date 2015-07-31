#if 0 /* FS: Yamagi removed */
#include "g_local.h"

void assertMsg(const char *msg, const char *file, int line)
{
	gi.dprintf(DEVELOPER_MSG_GAME, "DEBUG[%s:%i] %s\n", file, line, msg);
}
#endif // 0
