#include "shared/interface/itfMesManager/decls.h"

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(s32 id, u8 speed) {
    pinst[id].mes->message.color[3] = speed;
}
