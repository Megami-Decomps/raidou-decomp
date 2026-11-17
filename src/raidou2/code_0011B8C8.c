#include "common.h"
#include "basic.h"

INCLUDE_ASM(const s32, "code_0011B8C8", dds3MoverBasicGetPathControl);

INCLUDE_ASM(const s32, "code_0011B8C8", dds3MoverBasicUpdate);

u32 dds3ObjectBaseSetControlBasicSub(u32 type)
{
    u32 itemType = 0;

    switch (type) {
    case BASIC_TYPE_MOVER:
        itemType = 1;
        break;
    case BASIC_TYPE_STATICMODEL:
    case BASIC_TYPE_EFFECT:
    case BASIC_TYPE_EVENT:
        itemType = 3;
        break;
    case BASIC_TYPE_AREA:
        itemType = 4;
        break;
    }
    return itemType;
}
