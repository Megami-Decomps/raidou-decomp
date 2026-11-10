#include "shared/sdf/sdfModel/decls.h"

void sdfModelNodeSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef)
{
    s32 pass;
    s32 slot;
    struct sdfShapeHeader_t **cursor;
    struct sdfShapeHeader_t *shape;

    sdfModelNodeBaseSet(aNode, aDef);
    switch (aDef->format) {
    case 0:
        for (pass = 0; pass != 2; pass++) {
            for (slot = 0; slot != 3; slot++) {
                sdfModelLinkDrawPacket(aNode, aDef->shape[slot], slot, 0, pass);
            }
        }
        break;
    case 1:
        if (aDef->shape[0] != NULL) {
            for (pass = 0; pass != 2; pass++) {
                cursor = (struct sdfShapeHeader_t **)aDef->shape[0];
                while ((shape = *cursor++) != NULL) {
                    sdfModelLinkDrawPacket(aNode, shape, 0, 1, pass);
                }
            }
        }
        break;
    }
}
