#include "common.h"
#include "sbtlWork.h"
#include "mapData.h"
#include "fldPanel.h"

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlRefSbtl3);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlDrawON);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlCallGetActiveScene);

f32 sbtlFree(f32 x, f32 z) {
    return mapGetY(x, z);
}

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlFree_sbtlScene);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlRestart_Scene);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlRefSbtl4);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlRefSbtl31);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlRefSbtl5);

INCLUDE_ASM(const s32, "code_0026A9B0", sbtlBGMLOADSTART);

s32 sbtlLeaf54(dds3ProcessID_t *id) {
    return sbtlOnDataLoadInit();
}

INCLUDE_ASM(const s32, "code_0026A9B0", BasicSceneFree);
