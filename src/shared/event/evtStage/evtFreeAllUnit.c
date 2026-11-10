#include "shared/event/evtStage/decls.h"

/* Detach every unit node from the secondary object's per-kind list. */
void evtFreeAllUnit(void) {
    dds3Basic_t *secondaryObject = (dds3Basic_t *)dds3GetControlScene();
    evtWorldTable_t *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((evtWorldTable_t *)secondaryObject->workdata);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3DeleteSceneBasicData(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}
