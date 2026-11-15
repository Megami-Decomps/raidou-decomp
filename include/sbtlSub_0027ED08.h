#ifndef SBTLSUB_0027ED08_H
#define SBTLSUB_0027ED08_H

#include "common.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;
struct sprHandle_t;

typedef struct BtlProjectedScreenPoint {
    s32 x;
    s32 y;
} BtlProjectedScreenPoint;

typedef char BtlProjectedScreenPoint_size_must_be_0x08[
    (sizeof(BtlProjectedScreenPoint) == 0x08) ? 1 : -1];

typedef s32 BtlProjectedScreenPointResult;

enum {
    BTL_PROJECTED_SCREEN_POINT_REJECTED = 0,
    BTL_PROJECTED_SCREEN_POINT_WRITTEN = 1,
};

BtlProjectedScreenPointResult btlProjectForwardPositionToScreen(BtlProjectedScreenPoint *outPoint);

void sbtlCallDestroyFontRequestAfter(struct SBTLUNIT_WORK *wk, s32 x, s32 y, s32 z, u32 rgba, s32 id);
void sbtlCallDevilName(s32 X, s32 Y, s32 AlphaRate, void *pData, struct sprHandle_t *pSpr);
void sbtlCallGetSmallFontFRQ23(s32 x, s32 y, s32 z, u32 rgba, s32 id);
s32 sbtlCheckVerticalArea(const sdfVector4_t *pos0, const sdfVector4_t *rot, const sdfVector4_t *pos1, f32 area);
void sbtlDrawAisyoName(s32 x, s32 y, s32 z, u32 rgba, s32 id);
void sbtlDrawBSTName(s32 x, s32 y, s32 z, u32 rgba, s32 id);
char *sbtlGetAisyoHelpFont(s32 id);
char *sbtlGetBSTNameFont(s32 id);
f32 sbtlGetBezier(f32 x0, f32 x1, f32 x2, f32 x3, f32 bt);
s32 sbtlGetLargePara(s16 *adr, s32 max);
u8 sbtlGetMeireiFont(s32 id);
void sbtlGetSkillMenu(struct SBTLUNIT_WORK *wk, u16 typeflg);
char *sbtlLeaf73(s32 id);
void sbtlParaAdd(s32 *para, s32 add, s32 max, s32 reset);
void sbtlParaAdd_16(s16 *para, s16 add, s16 max, s16 reset);
void sbtlParaAdd_8(s8 *para, s8 add, s8 max, s8 reset);
void sbtlParaDec(s32 *para, s32 dec, s32 max);
void sbtlParaDec_16(s16 *para, s16 dec, s16 max);
void sbtlParaDec_8(s8 *para, s8 dec, s8 max);
const char *sdatDevilName(s32 id);

#endif
