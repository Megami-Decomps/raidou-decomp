#ifndef CMPSTATUS_H
#define CMPSTATUS_H

#include "common.h"
#include "kernel.h"

struct tagCMP_STATUS_WORK;

#ifdef VERSION_RAIDOU2
typedef struct cmpSummnerRank_t {
    s8 bSummnerRankStart;           /* 0x00 */
    s8 bSummnerRankWait;            /* 0x01 */
    struct sdatUnitWork_t *psUnit;  /* 0x04 */
    s32 iAlphaRate;                 /* 0x08 */
    s32 iCounter;                   /* 0x0C */
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
