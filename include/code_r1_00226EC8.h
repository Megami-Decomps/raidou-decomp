#ifndef CODE_R1_00226EC8_H
#define CODE_R1_00226EC8_H

#include "common.h"
#include "enums.h"

struct Standard;
struct eff2dHandle_t;
struct effBehavior_Params_Basic_t;
struct effBillboardWork_t;
struct effObjectParticle_Parts_t;
struct effPartsHandle_t;
struct effPolygonTexTemp_t;
struct opcHandle_t;
struct ptexHandle_t;
struct fileJobPayload_t;

#define EFFOPRT_FLAG_IGNORESTOP 1

/* Object particle, file job type 24 (0x80, Create_effObjectParticle). */
typedef struct opcHandle_t {
    void *option;  /* 0x00 */
    u32 abgr;      /* 0x04 */
    f32 scale;     /* 0x08 */
    u8 unkC[0x54];
    void *unk60;
    void *unk64;
    void *unk68;
    void *unk6C;
    void *unk70;
    u8 unk74[8];
    u16 state;     /* 0x7C */
    u16 flags;     /* 0x7E: EFFOPRT_FLAG_* */
} opcHandle_t;

typedef char opcHandleSizeCheck[sizeof(opcHandle_t) == 0x80 ? 1 : -1];

void BehaviorSet(struct opcHandle_t *hParticle, EFFBEHAVIOR_TYPE type, struct effBehavior_Params_Basic_t *pParams);
void Calc(struct opcHandle_t *hParticle);
void CalcAndDraw(struct effPartsHandle_t *handle);
void Delete_effObjectParticle(struct effPartsHandle_t *handle);
void Delete_effPolygonTex(struct effPartsHandle_t *handle);
void Draw_effMuzzleFlash(struct effBillboardWork_t *work);
void Draw_effObjectParticle(struct opcHandle_t *hParticle);
void Draw_effPolygonTex(struct effPartsHandle_t *handle);
struct opcHandle_t *ParamsCreate(struct effBehavior_Params_Basic_t *pParams);
void PartsCalc(struct opcHandle_t *hParticle, struct effObjectParticle_Parts_t *pParts, s32 frame);
struct ptexHandle_t *PartsCreate(struct fileJobPayload_t *payload);
void PartsInit(struct opcHandle_t *hParticle, struct effObjectParticle_Parts_t *pParts);
void PartsListSet_effObjectParticle(struct opcHandle_t *hParticle, void *pEPL);
void Render(struct ptexHandle_t *hPolygon, struct effPolygonTexTemp_t *wk);
void ResourceCopy_effPolygonTex(struct ptexHandle_t *dst, struct ptexHandle_t *src);
void Update_effPolygonTex(struct ptexHandle_t *hPolygon, struct effPolygonTexTemp_t *wk);
void eff2Draw(struct effPartsHandle_t *handle);
void effLoadFlashTextures(void);
void effLoadWindTexture(void);
void effLoadScalyTexture(void);
struct opcHandle_t *effCreateSlotNodeFromFileJob(struct Standard *pParts);

typedef struct effActiveResource_t {
    f32 position[4];
    f32 orientation[4];
    f32 scale;
    u32 color;
    u32 frame;
    union {
        u32 index;
        s32 signedIndex;
        u16 shortIndex;
    } kind;
    void *resource;
    u8 pad_34[4];
    void *payload;
    u8 pad_3C[4];
} effActiveResource_t;

typedef char effActiveResource_size_must_be_0x40[
    (sizeof(effActiveResource_t) == 0x40) ? 1 : -1];

#include "effQuake.h"

#endif
