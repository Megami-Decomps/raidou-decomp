#include "common.h"

/* Declarations from smt3-decomp src/smt3/script/scrScriptProcess.c */
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
/* End of declarations from src/smt3/script/scrScriptProcess.c */

scrProcessWork_s *scrLoadScriptFile(char *name, s32 messageHandle)
{
    sdfMemHandle_t *mem;
    void *data;
    scrProcessWork_s *pw;

    mem = sdfDevLoadFileEx(name, (u32 *)&data, NULL);
    if (data == NULL) {
        return NULL;
    }

    pw = fldFlowParse(data, messageHandle);
    if (pw != NULL) {
        pw->MemHandle = mem;
    }
    return pw;
}

dds3ProcessID_t *scrStartScriptProcess(s32 priority, scrProcessWork_s *pw)
{
    return pw->ProcessID = dds3StartProcess((char *)pw->ProcTable[pw->StartProc].Name, priority, 1, 1,
                                            scrScriptProcess, scrScriptShutdown, pw);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", dds3GlobalWorkClearFlags);

void scrStartScriptByName(s32 priority, char *name, s32 messageHandle)
{
    scrStartScriptProcess(priority, scrLoadScriptFile(name, messageHandle));
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114978);

dds3ProcessID_t *scrStartScript2(s32 priority, void *scrdata, s32 startproc)
{
    return scrStartScriptProcess(priority, fldFlowParse(scrdata, startproc));
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_001149C8);

dds3ProcessID_t *scrRunScriptEx(s32 priority, scrHeader_t *header, scrTypeTable_t *typeTable,
                    scrProcTable_t *procTable, scrLabelTable_t *labelTable, scrCode_u *code,
                    u8 *messageData, u8 *stringData, s32 procNo)
{
    return scrStartScriptProcess(priority, scrPrintProcedureStart(header, typeTable, procTable, labelTable,
                                                           code, messageData, stringData, procNo));
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114A30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114A48);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114AE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114B58);

scrProcessWork_s *scrSearchScriptName(char *scriptname)
{
    scrProcessWork_s *pw;

    for (pw = scrHandleFirst; pw != NULL; pw = pw->next) {
        if (strcmp(scriptname, (char *)pw->Name) == 0) {
            return pw;
        }
    }
    return NULL;
}

s32 scrSearchProcedureName(void *scrdata, char *procname)
{
    scrHeader_t *header = scrdata;
    scrTypeTable_t *typeTable;
    scrProcTable_t *procTable = NULL;
    s32 i;
    s32 no;

    if (header == NULL) {
        return -1;
    }

    typeTable = (scrTypeTable_t *)(header + 1);
    for (i = 0; i < header->TypeTableCount; i++) {
        if (typeTable[i].Type == SCR_DATATYPE_PROCTABLE) {
            procTable = (scrProcTable_t *)((u8 *)header + typeTable[i].ItemAddress);
            break;
        }
    }

    if (procTable == NULL) {
        return -1;
    }

    for (no = 0; no < typeTable[i].ItemCount; no++) {
        if (strcmp((char *)procTable[no].Name, procname) == 0) {
            return no;
        }
    }
    return -1;
}

void scrSetUserData(dds3ProcessID_t *id, void *p)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    pw->data = p;
}

void *scrGetUserData(dds3ProcessID_t *id)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    return pw->data;
}

void scrScriptShutdown(dds3ProcessID_t *id)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    if (pw != NULL) {
        scrPrintProcedureEnd(pw);
    }
    dds3SetProcessData(id, NULL);
}

s32 scrRunScript(scrProcessWork_s *h)
{
    return scrTraceCode(h);
}

s32 scrScriptProcess(dds3ProcessID_t *id)
{
    switch (scrTraceCode(dds3GetProcessData(id))) {
    case SCR_RESULT_ERROR:
        return -1;
    case SCR_RESULT_END:
        return -1;
    case SCR_RESULT_CONTINUE:
    default:
        return 0;
    }
}
