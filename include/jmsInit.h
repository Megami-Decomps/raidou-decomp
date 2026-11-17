#ifndef JMSINIT_H
#define JMSINIT_H

#include "common.h"
#include "kernel.h"
#include "code_r1_001CC1D0.h"

/* Work of the jms menu processes. HD holds a pointer to the sequence handle; R1 embeds it. Partial layout. */
typedef struct jmsData_t {
    struct sdfMemHandle_t *hMem;
    u32 flag;
    jmsSeqHandle_t panel;
    s32 seqUnit;
} jmsData_t;
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

#endif
