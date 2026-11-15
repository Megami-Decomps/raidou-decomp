#ifndef JMSINIT_H
#define JMSINIT_H

#include "common.h"
#include "kernel.h"

struct jmsData_t;
struct sprHandle_t;

s32 frLeaf15(struct jmsData_t *pData);
void jmsDebugProcessCheck(void);
void jmsDestroy(dds3ProcessID_t *PID);
struct sprHandle_t *jmsGetSpriteHandle(void);
s32 jmsProcessCheck(void);
void jmsProcessEnd(void);
s32 jmsScrCmd_JMS_EXIT(void);
s32 jmsScrCmd_JMS_MOTION(void);
s32 jmsScrCmd_JMS_SET_MODE(void);
void jmsStartEtcProcess(dds3ProcessID_t *PID);

#endif
