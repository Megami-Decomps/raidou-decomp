#include "shared/sdf/sdfMat/decls.h"

u8 *sdfMatSet(sdfMat_t *aMatPtr, sdfItemArray_t *aTexList, u8 *aDef)
{
    u32 flags;
    u8 *cursor;
    u32 packed;

    aMatPtr->id = *(u32 *)aDef;
    aMatPtr->flags = *(u16 *)(aDef + 4);
    flags = *(u16 *)(aDef + 6);
    cursor = aDef + 8;
    if (flags & 0x1) {
        sdfMatSetAmbient(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x2) {
        sdfMatSetDiffuse(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x4) {
        sdfMatSetTex(aMatPtr, EE_ADDR_AS(sdfTexHandle_t *, ((u32 *)aTexList->items)[*(u16 *)cursor]));
        cursor += 4;
    }
    if (flags & 0x8) {
        sdfMatSetTexUvw(aMatPtr, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2],
                        ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x10) {
        sdfMatSetMultiDiffuse(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x20) {
        packed = *(u32 *)cursor;
        cursor += 4;
        sdfMatSetMultiTex(aMatPtr, EE_ADDR_AS(sdfTexHandle_t *, ((u32 *)aTexList->items)[packed & 0xFFFF]));
        sdfMatSetMultiMethod(aMatPtr, packed >> 16);
    }
    if (flags & 0x40) {
        sdfMatSetMultiTexUvw(aMatPtr, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2],
                             ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x80) {
        sdfMatSetSpecular(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x100) {
        sdfMatSetRefMap(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x200) {
        sdfMatSetConstant(aMatPtr, *(f32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x400) {
        sdfMatSetFaceDivParm(aMatPtr, ((f32 *)cursor)[0], ((f32 *)cursor)[1]);
        cursor += 8;
    }
    return cursor;
}
