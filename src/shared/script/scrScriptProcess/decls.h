/* Declarations of smt3-decomp src/smt3/script/scrScriptProcess.c, for its bodies in this directory. */
#ifndef SHARED_SCRIPT_SCRSCRIPTPROCESS_H
#define SHARED_SCRIPT_SCRSCRIPTPROCESS_H

#include "common.h"

#include "sdf_resource.h"

#include "newdata.h"

#include "scr.h"

extern s32 scrHandleCount;

extern scrProcessWork_s *scrHandleFirst;

extern s32 strcmp(const char *s1, const char *s2);

extern dds3ProcessID_t *dds3StartProcess(char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                         s32 (*drawExit)(dds3ProcessID_t *id),
                                         void (*deleteExit)(dds3ProcessID_t *id),
                                         void *p);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern scrProcessWork_s *fldFlowParse(void *data, s32 messageHandle);

extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typeTable,
                                                scrProcTable_t *procTable,
                                                scrLabelTable_t *labelTable, scrCode_u *code,
                                                u8 *messageData, u8 *stringData, s32 procNo);

extern void scrUnlinkHandle(scrProcessWork_s *pw);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void itfMesMngRelease(s32 id);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

#include "sdk_libc.h"

extern void datDebugPrintf(const char *fmt, ...);

extern s32 scrTraceCode(scrProcessWork_s *pwork);

extern s32 scrScriptProcess(dds3ProcessID_t *id);

void scrScriptShutdown(dds3ProcessID_t *id);

scrProcessWork_s *scrLoadScriptFile(char *name, s32 messageHandle);

dds3ProcessID_t *scrStartScriptProcess(s32 priority, scrProcessWork_s *pw);

void dds3GlobalWorkClearFlags(void);

void scrStartScriptByName(s32 priority, char *name, s32 messageHandle);

scrProcessWork_s *scr_109D80(char *name);

dds3ProcessID_t *scrStartScript2(s32 priority, void *scrdata, s32 startproc);

scrProcessWork_s *scr_109DD0(void *data);

dds3ProcessID_t *scrRunScriptEx(s32 priority, scrHeader_t *header, scrTypeTable_t *typeTable,
                    scrProcTable_t *procTable, scrLabelTable_t *labelTable, scrCode_u *code,
                    u8 *messageData, u8 *stringData, s32 procNo);

scrProcessWork_s *scr_109E38(scrHeader_t *header, scrTypeTable_t *typeTable,
                             scrProcTable_t *procTable, scrLabelTable_t *labelTable,
                             scrCode_u *code, u8 *messageData, u8 *stringData);

void scrPrintProcedureEnd(scrProcessWork_s *pw);

void scrAllReleaseScript(void);

/* Restart a procedure with an empty operand stack and fresh timers. */
s32 func_00109FD0(scrProcessWork_s *pw, s32 procNo);

scrProcessWork_s *scrSearchScriptName(char *scriptname);

s32 scrSearchProcedureName(void *scrdata, char *procname);

void scrSetUserData(dds3ProcessID_t *id, void *p);

void *scrGetUserData(dds3ProcessID_t *id);

void scrScriptShutdown(dds3ProcessID_t *id);

s32 scrRunScript(scrProcessWork_s *h);

s32 scrScriptProcess(dds3ProcessID_t *id);

#endif
