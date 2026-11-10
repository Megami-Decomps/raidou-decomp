#include "common.h"

/* Declarations from smt3-decomp src/smt3/facility/fclShare.c */
#include "common.h"

#include "facility.h"

#include "fpu.h"

#include "libsdf.h"

#include "newdata.h"

#include "ps2asm.h"

#include "event.h"

#include "sdfFrame.h"

extern void sdfMemFreeDirect(void *aHandle);

extern u32 dds3GetKernelCount(void);

extern void fclDrawSpriteCore(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                              u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclDrawSpriteAngleCore(s32 X, s32 Y, s32 W, s32 H, f32 Angle, s32 *pCol,
                                   fclSprite_t *pSprite, u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclSetSprite(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2,
                         s32 V2, s32 Col, s32 TexHandle);

extern void fclDrawSpriteEx(s32 X, s32 Y, s32 W, s32 H, fclSprite_t *pSprite, s32 OtNo);

extern s32 fclRunPanelAnim(void *work);

extern sdfGrOt_t sdfGrOtTable[85];

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)(void *));

extern void fclRagTexCalcSizes();

extern void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 id);

extern s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

extern s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim);

extern s32 sdfTexSet(void *aTex);

s32 fclLoadTexture(char *pFileName);

void fclSetSpriteColor(u8 *pSprite, u32 Col);

void fclSetSpriteEx(fclSprite_t *pSprite, fclSpriteRect_t *pSpriteBase, s32 X, s32 Y, s32 Z, s32 Col,
                    s32 Flag, s32 TexHandle);

void fclSetSprite(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2, s32 V2,
                  s32 Col, s32 TexHandle);

void fclSetSpriteNormal(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 Col, s32 TexHandle);

void fclDrawSprite(s32 X, s32 Y, fclSprite_t *pSprite);

extern void func_002146F8(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                         u8 FlipFlag, s32 OtNo);

void fclDrawSpriteEx(s32 X, s32 Y, s32 W, s32 H, fclSprite_t *pSprite, s32 OtNo);

void func_002146F8(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                   u8 FlipFlag, s32 OtNo);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

void fclCalcRotPos(s32 *pBufX, s32 *pBufY, f32 DistX, s32 DistY, s32 CenterX, s32 CenterY, s32 Angle);

typedef struct
{
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} fclRotPoint_t;

extern void fclCalcRotPos(s32 *pBufX, s32 *pBufY, f32 DistX, s32 DistY, s32 CenterX, s32 CenterY, s32 Angle);

extern u8 dds3PadButtonState[0x40];

/* Both native mode paths test the same signed pad-state byte. */
s32 func_00214FD8(s32 mode);

s32 fclRagCalcTexSize(s32 size, s32 mode);

extern s32 fclRagCalcTexSize(s32 size, s32 mode);

void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, void (*calc)(void *));

s32 fclRunPanelAnim(void *work);

void fclSetPanelAnimState(fclPanelAnim_t *anim, u8 state);

s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim);

void func_00215188(fclPanelAnim_t *anim, s32 value);

s32 fcl_215190(fclPanelAnim_t *anim);

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u16 width;
    u16 height;
    s32 color;
    u8 speed;
    u8 reserved15[3];
    s32 timer;
    s32 timer2;
    s32 flags;
} fclPhasePanel_t;

extern void fclDrawPanelSprite2(s32 id, s32 ot);

extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

void fclDrawPhaseValue(void *work, s32 color, s32 ot);

extern s32 D_00345D88[];

extern fclSpriteTbl_t fclSpriteTable[];

extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page,
                             s32 spriteId, s32 *textures, fclSpriteTbl_t *table, s32 ot);

void fclDrawPanelBorder(void *work, s32 z, s32 inset, s32 ot);

extern void fclDrawPanelSprite(s32 id, s32 ot);

extern void fclDrawPhaseValue(void *work, s32 color, s32 ot);
/* End of declarations from src/smt3/facility/fclShare.c */

s32 fclLoadTexture(char *pFileName)
{
    void *data;
    s32 size;
    void *file;
    s32 handle;

    file = sdfDevLoadFileEx(pFileName, (u32 *)&data, &size);
    handle = sdfTexSet(data);
    sdfMemFreeDirect(file);
    return handle;
}
