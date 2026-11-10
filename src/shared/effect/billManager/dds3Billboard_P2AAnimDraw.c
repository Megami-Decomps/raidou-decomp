#include "shared/effect/billManager/decls.h"

dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim) {
    P2A_ANIMATION_tag *entry = pBillAnim->pAnim;
    dds3BillAnimResource_tag *data = pP2A->u.animationData;
    P2A_TEXANIM_tag *record;
    dds3BillResource_tag *child;
    s32 index;
    u32 color;

    if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        return NULL;
    }
    if (pBillAnim->wait <= 0) {
        pBillAnim->key++;
        if ((u32)pBillAnim->key >= entry->times) {
            if (entry->mode & BILL_ANIMATION_FLAG_STOP_AT_END) {
                pP2A->animationActive = 0;
                pBillAnim->key = entry->times - 1;
            } else {
                pBillAnim->key = 0;
            }
        }
        pBillAnim->wait = SDF_FRAMES(pBillAnim->pTexAnim[pBillAnim->key].attr + 1) - 1;
    } else {
        pBillAnim->wait--;
    }
    index = pBillAnim->key;
    record = pBillAnim->pTexAnim + index;
    if (entry->mode & BILL_ANIMATION_FLAG_FRAME_COLORS) {
        color = ((u32 *)(data->pHeader + entry->alpha))[index];
    } else {
        color = 0x80808080;
    }
    pP2A->abgr = color;
    child = data->pResList[record->texid];
    if (entry->mode & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
        pP2A->mode = 2;
    } else if (entry->mode & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
        pP2A->mode = 3;
    } else {
        pP2A->mode = 1;
    }
    pP2A->rot = record->angle;
    child->uv.components[0] = record->u1;
    child->uv.components[1] = record->v1;
    child->uv.components[2] = record->u2;
    child->uv.components[3] = record->v1;
    child->uv.components[4] = record->u2;
    child->uv.components[5] = record->v2;
    child->uv.components[6] = record->u1;
    child->uv.components[7] = record->v2;
    {
        f32 halfHeight = record->sy * 0.5f;
        f32 halfWidth = record->sx * 0.5f;
        f32 y = halfHeight - record->cy;
        f32 x = halfWidth - record->cx;

        child->height = halfHeight;
        child->width = halfWidth;
        child->cy = y;
        child->cx = x;
    }
    return child;
}
