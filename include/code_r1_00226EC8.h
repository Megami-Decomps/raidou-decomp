#ifndef CODE_R1_00226EC8_H
#define CODE_R1_00226EC8_H

#include "common.h"
#include "enums.h"

struct Standard;
struct eff2dHandle_t;
struct effBehavior_Params_Basic_t;
struct effObjectParticle_Parts_t;
struct effPartsHandle_t;
struct effPolygonTexTemp_t;
struct opcHandle_t;
struct ptexHandle_t;

void BehaviorSet(struct opcHandle_t *hParticle, EFFBEHAVIOR_TYPE type, struct effBehavior_Params_Basic_t *pParams);
void Calc(struct opcHandle_t *hParticle);
void CalcAndDraw(struct effPartsHandle_t *handle);
void Delete_effObjectParticle(struct effPartsHandle_t *handle);
void Delete_effPolygonTex(struct effPartsHandle_t *handle);
void Draw_effMuzzleFlash(struct effPartsHandle_t *handle);
void Draw_effObjectParticle(struct opcHandle_t *hParticle);
void Draw_effPolygonTex(struct effPartsHandle_t *handle);
struct opcHandle_t *ParamsCreate(struct effBehavior_Params_Basic_t *pParams);
void PartsCalc(struct opcHandle_t *hParticle, struct effObjectParticle_Parts_t *pParts, s32 frame);
struct eff2dHandle_t *PartsCreate(struct Standard *pParts);
void PartsInit(struct opcHandle_t *hParticle, struct effObjectParticle_Parts_t *pParts);
void PartsListSet_effObjectParticle(struct opcHandle_t *hParticle, void *pEPL);
void Render(struct ptexHandle_t *hPolygon, struct effPolygonTexTemp_t *wk);
void ResourceCopy_effPolygonTex(struct ptexHandle_t *dst, struct ptexHandle_t *src);
void Update_effPolygonTex(struct ptexHandle_t *hPolygon, struct effPolygonTexTemp_t *wk);
void eff2Draw(struct effPartsHandle_t *handle);
struct opcHandle_t *effCreateSlotNodeFromFileJob(struct Standard *pParts);

#endif
