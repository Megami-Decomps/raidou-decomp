#ifndef CODE_0034F3B8_H
#define CODE_0034F3B8_H

#include "common.h"
#include "kernel.h"
#include "code_r1_001CC1D0.h"

/* A battle's rewards, built by sbtlCallBtlSearchUnitID30 from the battle's
 * drop work and copied by rstSetReward. Each count is clamped by the builder. */
typedef struct rstWork_t {
    s32 money;          /* 0..99999 */
    s32 item[8][2];     /* item id and count */
    s32 unk44;
    s32 exp;            /* Raidou's, 0..99999 */
    s32 stockExp[12];   /* the summoned stock demon's, 0..99999 */
    s32 stockUnk7C[12]; /* the summoned stock demon's, 0..100 */
    s32 mag;            /* 0..9999 */
    s32 summonIndex;    /* the stock slot summoned, or -1 */
} rstWork_t;

extern rstWork_t rstWork;
extern u32 rstData4;
extern dds3ProcessID_t *inputpanel_id;
extern dds3ProcessID_t *g_mainPID3;

/* Partial layouts of process work read through dds3GetProcessData. */
typedef struct jmsInputPanelWork_t {
    struct sdfMemHandle_t *hMem;
    u8 unk04[4];
    dds3ProcessID_t *drawPID;
    u8 unk0C[4];
    u8 step;
    u8 flags;
    u16 count;
    s32 unk14;
} jmsInputPanelWork_t;

typedef struct rstMainWork_t {
    u8 unk00[4];
    dds3ProcessID_t *giftMsgPID;
    u8 unk08[0x18];
    u32 flags;
} rstMainWork_t;

extern const char cmpCalcProcName[];
extern const char campDrawProcName[];
extern const char campUpdateProcName[];


void rstSetReward(const rstWork_t *pReward);
void Release_jmsMenu(jmsInputPanelWork_t *pWork);

/* Partial layout of the process work that rstCallKillProcess releases. */
typedef struct rstProcWork_t {
    struct sdfMemHandle_t *hMem;
    u8 unk04[4];
    struct sdfMemHandle_t *hDelayed;
    s32 unk0C;
    u8 unk10[4];
    dds3ProcessID_t *pid;
} rstProcWork_t;

void rstCallKillProcess(rstProcWork_t *pWork);
void rstResultResultDraw(jmsSeqWork_t *pWork);

typedef struct cmpMemWork_t {
    struct sdfMemHandle_t *handle;
    s32 unk04;
} cmpMemWork_t;
void cmpStartProcessPanelEff(void);

#endif
