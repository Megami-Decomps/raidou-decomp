#include "shared/effect/billManager/decls.h"

void dds3Billboard_P2ASetAnim(dds3BillAnimResource_tag *hBillAnimRes, s32 number, dds3BillAnim_tag *pBillAnim) {
    u8 *base;
    P2A_ANIMATION_tag *entry;
    s32 offset;
    s16 value;

    base = hBillAnimRes->pHeader;
    entry = hBillAnimRes->pAnimTable + number;
    offset = entry->spranm;
    pBillAnim->pAnim = entry;
    base = base + offset;
    pBillAnim->key = 0;
    value = ((P2A_TEXANIM_tag *)base)->attr;
    pBillAnim->pTexAnim = (P2A_TEXANIM_tag *)base;
    pBillAnim->wait = SDF_FRAMES(value + 1) - 1;
}
