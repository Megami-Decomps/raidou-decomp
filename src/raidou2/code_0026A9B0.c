#include "common.h"
#include "sbtlWork.h"
#include "mapData.h"
#include "fldPanel.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "sdfDelayedResrc.h"

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

s32 DataLoadShutdown(dds3ProcessID_t *id) {
    return sbtlOnDataLoadInit();
}

s32 BasicSceneFree(void) {
    dds3Basic_t *scene;
    dds3SceneBasic_t *work;

    scene = dds3GetActiveScene();
    if (scene == NULL) {
        return 0;
    }
    work = scene->workdata;
    while (work->basictable[BASIC_TYPE_UNITOBJECT].firstbasic != NULL) {
        dds3DeleteSceneBasicData(work->basictable[BASIC_TYPE_UNITOBJECT].firstbasic);
    }
    while (sdfDelayedResrcCheck()) {
    }
    dds3DeleteScene(scene);
    return 1;
}
