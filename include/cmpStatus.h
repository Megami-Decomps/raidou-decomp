#ifndef CMPSTATUS_H
#define CMPSTATUS_H

#include "common.h"
#include "kernel.h"

struct tagCMP_STATUS_WORK;

#ifdef VERSION_RAIDOU2
/* HD tagOBJECT_WORK: one sprite object of the summoner rank screen. */
typedef struct tagOBJECT_WORK {
    s8 bEnabled;      /* 0x00 */
    char szString[11]; /* 0x01 */
    s32 iRankSprNo;   /* 0x0C: HD iSprNo of the two rank objects */
    s32 iSprNo;       /* 0x10 */
    f32 fSize;        /* 0x14 */
    f32 fSizeW;       /* 0x18 */
    f32 fSizeH;       /* 0x1C */
    f32 fDelta;       /* 0x20 */
    s32 iAlpha;       /* 0x24 */
    f32 fAlpha;       /* 0x28 */
    f32 fCounter;     /* 0x2C */
    s32 iMode;        /* 0x30 */
    f32 fQ_x_range;   /* 0x34 */
    f32 fQ_y_range;   /* 0x38 */
    s32 iQ_frame;     /* 0x3C */
    s32 iInX;         /* 0x40 */
} tagOBJECT_WORK;

void cmpSummnerRankObjectInit(tagOBJECT_WORK *psSelf);

typedef struct cmpSummnerRank_t {
    s8 bSummnerRankStart;           /* 0x00 */
    s8 bSummnerRankWait;            /* 0x01 */
    struct sdatUnitWork_t *psUnit;  /* 0x04 */
    s32 iAlphaRate;                 /* 0x08 */
    s32 iCounter;                   /* 0x0C */
    tagOBJECT_WORK sObjBG;          /* 0x10 */
    tagOBJECT_WORK sObjBGSRank;     /* 0x54 */
    tagOBJECT_WORK sObjBGRaido;     /* 0x98 */
    tagOBJECT_WORK sObjCyusei;      /* 0xDC */
    tagOBJECT_WORK sObjSRank;       /* 0x120 */
    tagOBJECT_WORK sObjRaido;       /* 0x164 */
    tagOBJECT_WORK sObjBlur;        /* 0x1A8 */
} cmpSummnerRank_t;

typedef void *(*cmpSummnerRankFunc_t)(cmpSummnerRank_t *psSelf);

typedef struct cmpSummnerRankMng_t {
    cmpSummnerRankFunc_t cbState;   /* 0x00 */
    cmpSummnerRank_t sSummnerRank;  /* 0x04 */
} cmpSummnerRankMng_t;

void cmpSummnerRankMngInit(cmpSummnerRankMng_t *psSelf);
void cmpSummnerRankMngSetState(cmpSummnerRankMng_t *psSelf, cmpSummnerRankFunc_t cbFunc);
void cmpSummnerRankInit(cmpSummnerRank_t *psSelf);
void *cmpSummnerRankStateWait(cmpSummnerRank_t *psSelf);
#endif

dds3ProcessFunc_t DrawProcess_cmpStatus(dds3ProcessID_t *PID);
s32 ProcessCheck_cmpStatus(void);
dds3ProcessID_t *ProcessStart_cmpStatus(void);
void Shutdown_cmpStatus(dds3ProcessID_t *PID);
s32 StatusGoSword(void);
s32 cmpChkDraw(void);
s32 cmpChkEnd(void);
void cmpDrawPartsExp(struct tagCMP_STATUS_WORK *wk, s32 a_alphaRate);
s32 cmpGetFlagCallDebug(void);
s32 cmpGetFlagScreenShot(void);
void cmpGetMotionParam(struct tagCMP_STATUS_WORK *wk);
void cmpSeqRankUp(struct tagCMP_STATUS_WORK *wk);
void cmpSetFlagCamp(void);
void cmpSetFlagDraw(s32 a_bDraw);
void cmpSetModelLight(struct tagCMP_STATUS_WORK *wk);
s32 cmpStartRankUp(void);
void cmpUpdatemodel(struct tagCMP_STATUS_WORK *wk);
void cmpViewerModel(struct tagCMP_STATUS_WORK *wk);

#endif
