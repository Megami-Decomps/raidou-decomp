#include "shared/event/evtStage/decls.h"

s32 evtStageAnimeControl(s32 type, s32 mode) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *node;
    s32 combinedMode;

    if (world == NULL) {
        return 0;
    }
    combinedMode = type * 3 + mode;
    for (node = ((evtWorldTable_t *)world->workdata)->slots[6].head; node != NULL; node = node->next) {
        dds3ObjectBaseMatterAnimeSpeed(node, combinedMode);
    }
    return 1;
}
