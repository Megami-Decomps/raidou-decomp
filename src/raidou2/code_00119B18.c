#include "common.h"
#include "dds3Basic.h"
#include "dds3SceneBasic.h"

u32 dds3AllocObjectId(void)
{
    dds3ObjectIdCounter = (dds3ObjectIdCounter + 1) & 0xFFF;
    dds3ObjectIdCounter = dds3SceneBasicSearchSerialID_Used(dds3ObjectIdCounter);
    return dds3ObjectIdCounter;
}

void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name)
{
    if (basic != NULL) {
        basic->name = name;
    }
}

INCLUDE_ASM(const s32, "code_00119B18", dds3Leaf26);
