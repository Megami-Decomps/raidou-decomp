#include "shared/event/evtCommand/decls.h"

u32 evtCommand_EFFECT_POS_XYZ(void) {
    dds3Basic_t *obj;
    evtModelHeader_t *header;

    obj = evtSearchBasic(7, scrGetIntPara(0));
    header = (evtModelHeader_t *)obj->workdata;
    if (header->flags & 4) {
        header->positionX = scrGetFloatPara(1);
        header->positionY = scrGetFloatPara(2);
        header->positionZ = scrGetFloatPara(3);
    } else {
        obj->poseset->pos.x = scrGetFloatPara(1);
        obj->poseset->pos.y = scrGetFloatPara(2);
        obj->poseset->pos.z = scrGetFloatPara(3);
        obj->poseset->flag = (obj->poseset->flag | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    }
    return 1;
}
