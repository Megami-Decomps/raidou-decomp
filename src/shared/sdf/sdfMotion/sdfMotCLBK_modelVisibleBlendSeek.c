#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_modelVisibleBlendSeek(sdfMotNodeModelBase_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    sdfModelNode_t *node;
    u8 *key;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    key = itf.key0;
    node = aThis->modelNode;
    if (*key == 0) {
        node->flags |= SDF_MODEL_NODE_FLAG_MOTION_HIDE;
    } else {
        node->flags &= ~SDF_MODEL_NODE_FLAG_MOTION_HIDE;
    }
}
