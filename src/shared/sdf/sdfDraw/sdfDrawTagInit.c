#include "shared/sdf/sdfDraw/decls.h"

void sdfDrawTagInit(sdfDrawTag_t *a) {
    a->z = 0xFFFF;
    a->next = NULL;
    a->head = 0;
    a->tail = 0;
    a->lightPacket = 0;
    a->fogPacket = 0;
    a->texPacket[0] = 0;
    a->texPacket[1] = 0;
}
