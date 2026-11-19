#ifndef JMSINIT_H
#define JMSINIT_H

#include "common.h"
#include "kernel.h"
#include "code_r1_001CC1D0.h"

/* Work of the jms menu processes. HD holds a pointer to the sequence handle; R1 embeds it. Partial layout. */
#ifdef VERSION_RAIDOU2
typedef struct jmsData_t {
    struct sdfMemHandle_t *hMem;
    u32 flag;
    u8 pad08[0xD0 - 0x8];
    dds3ProcessID_t *fieldD0; /* script process checked by jmsCheckScrRun */
    u8 padD4[0xE4 - 0xD4];
    s32 fieldE4;
} jmsData_t;
#else
typedef struct jmsData_t {
    struct sdfMemHandle_t *hMem;
    u32 flag;
    jmsSeqHandle_t panel;
    s32 seqUnit;
} jmsData_t;
#endif
struct sprHandle_t;

s32 frLeaf15(struct jmsData_t *pData);
s32 jmsDebugProcessCheck(void);
s32 jmsDestroy(dds3ProcessID_t *PID);
struct sprHandle_t *jmsGetSpriteHandle(void);
s32 jmsProcessCheck(void);
void jmsProcessEnd(void);
s32 jmsScrCmd_JMS_EXIT(void);
s32 jmsScrCmd_JMS_MOTION(void);
s32 jmsScrCmd_JMS_SET_MODE(void);
void jmsStartEtcProcess(dds3ProcessID_t *PID);
#ifdef VERSION_RAIDOU2
s32 jmsJimusyoYoigoshiHiru(dds3ProcessID_t *PID);
s32 rstCallGetProcessData27(dds3ProcessID_t *PID);
#endif

#endif
