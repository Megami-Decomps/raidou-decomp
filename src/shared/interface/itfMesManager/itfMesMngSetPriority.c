#include "shared/interface/itfMesManager/decls.h"

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(s32 id, s32 z_buf) {
    instanceMes_tag *mes = pinst[id].mes;

    if (mes->z == z_buf) {
        return;
    }
    ChangeZBuffer(mes->speaker.pfont, z_buf);
    ChangeZBuffer(mes->message.pfont, z_buf);
    ChangeZBuffer(mes->select.pfont, z_buf);
    mes->z = z_buf;
}
