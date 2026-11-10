#include "shared/effect/effPCPThunder/decls.h"

effPCP_ThunderPolyList_t *effPCP_ThunderPolyListCreate(effPCP_ThunderPolyListParam_t *src) {
    effPCP_ThunderPolyList_t *group = sdfChipAllocDirect(sizeof(effPCP_ThunderPolyList_t));
    s32 i;

    memset(group->handles, 0, sizeof(group->handles));
    group->head = *src;
    group->handles[0] = effPCP_ThunderPolyCreate(&group->head.TPParam);
    effPCP_ThunderPolySetVCAcute2(group->handles[0]);
    for (i = 1; i < src->count - 2; i++) {
        group->handles[i] = effPCP_ThunderPolyCreate(&group->head.TPParam);
        effPCP_ThunderPolySetVCNormal(group->handles[i]);
    }
    group->handles[i] = effPCP_ThunderPolyCreate(&group->head.TPParam);
    effPCP_ThunderPolySetVCAcute(group->handles[i]);
    group->color = EFF_THUNDER_NEUTRAL_COLOR;
    return group;
}
