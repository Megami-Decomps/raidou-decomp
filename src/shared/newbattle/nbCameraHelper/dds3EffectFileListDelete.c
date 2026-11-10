#include "shared/newbattle/nbCameraHelper/decls.h"

void dds3EffectFileListDelete(nbCameraHelper_effFileListHandle_t *a) {
    dds3EffectFileNode_t *node = a->head;

    if (node != NULL) {
        do {
            dds3EffectFileNode_t *next = node->next;
            sdfChipFreeDirect(node);
            node = next;
        } while (node != NULL);
    }
    sdfChipFreeDirect(a->directoryPath);
    sdfChipFreeDirect(a);
}
