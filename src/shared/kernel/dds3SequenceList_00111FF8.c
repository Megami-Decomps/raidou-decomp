#include "common.h"
#include "kernel.h"
#include "scr.h"
#include "event.h"
#include "field.h"
#include "sdk_libc.h"
#include "sdfChip.h"
#include "sdfMemory.h"
#include "itfMesManager.h"
#include "code_r1_0010DF68.h"
#include "dds3SequenceList_00111FF8.h"

extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;
extern s32 scrHandleCount;

extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                                scrProcTable_t *proctable,
                                                scrLabelTable_t *labeltable, scrCode_u *code,
                                                u8 *mesdata, char *strdata, s32 procno);



void scrLinkHandle(scrProcessWork_s *work)
{
    scrProcessWork_s *last = scrHandleLast;

    if (last == NULL) {
        scrHandleFirst = work;
        scrHandleLast = work;
        work->prev = NULL;
        work->next = NULL;
    } else {
        work->prev = last;
        last->next = work;
        work->next = NULL;
        scrHandleLast = work;
    }
    scrHandleCount++;
}

void scrUnlinkHandle(scrProcessWork_s *work)
{
    if (scrHandleFirst == work) {
        scrHandleFirst = work->next;
    } else {
        work->prev->next = work->next;
    }
    if (scrHandleLast == work) {
        scrHandleLast = work->prev;
    } else {
        work->next->prev = work->prev;
    }
    work->prev = NULL;
    work->next = NULL;
    scrHandleCount--;
}

scrProcessWork_s *AddScript(scrHeader_t *header, scrTypeTable_t *typetable, scrProcTable_t *proctable,
                           scrLabelTable_t *labeltable, scrCode_u *code, u8 *mesdata, char *strdata,
                           s32 procno)
{
    scrProcessWork_s *work;
    s32 i;

    if (header == NULL || typetable == NULL || proctable == NULL || code == NULL || procno < 0 ||
        procno >= typetable->ItemCount) {
        return NULL;
    }
    work = sdfChipAllocDirect(sizeof(scrProcessWork_s));
    if (work == NULL) {
        return NULL;
    }
    for (i = 0; (work->Name[i] = proctable[procno].Name[i]) != 0; i++) {
    }
    work->CP = proctable[procno].CodeAddress;
    work->SP = 0;
    {
        scrStackData_u *stackValue = work->StackData;
        u8 *stackType = work->StackType;

        for (i = 27; i >= 0; i--) {
            *stackType++ = 0;
            stackValue++->i = 0;
        }
    }
    work->Header = header;
    work->TypeTable = typetable;
    work->ProcTable = proctable;
    work->LabelTable = labeltable;
    work->Code = code;
    work->MessageData = mesdata;
    work->StringData = strdata;
    work->StartProc = procno;
    work->MesHandle = -1;
    work->ScriptTimer = 0;
    work->CommandTimer = 0;
    work->MemHandle = NULL;
    work->LocalInt = NULL;
    work->LocalFloat = NULL;
    work->ProcessID = NULL;
    work->prev = NULL;
    work->next = NULL;
    if (header->LocalIntCount > 0) {
#ifdef VERSION_RAIDOU2
        work->LocalInt = sdfMemLock(sdfMemAllocDirect(header->LocalIntCount * 4));
#else
        work->LocalInt = sdfChipAllocDirect(header->LocalIntCount * 4);
#endif
        for (i = 0; i < header->LocalIntCount; i++) {
            work->LocalInt[i] = 0;
        }
    } else {
        work->LocalInt = NULL;
    }
    if (header->LocalFloatCount > 0) {
        work->LocalFloat = sdfChipAllocDirect(header->LocalFloatCount * 4);
        for (i = 0; i < header->LocalFloatCount; i++) {
            work->LocalFloat[i] = 0.0f;
        }
    } else {
        work->LocalFloat = NULL;
    }
    if (mesdata != NULL) {
        work->MesHandle = itfMesMngInitialize(mesdata);
    }
    scrLinkHandle(work);
    datDebugPrintf("start <%s>\n", proctable[procno].Name);
    return work;
}

scrProcessWork_s *fldFlowParse(scrHeader_t *header, s32 procno)
{
    scrTypeTable_t *typetable;
    scrProcTable_t *proctable = NULL;
    scrLabelTable_t *labeltable = NULL;
    scrCode_u *code = NULL;
    u8 *mesdata = NULL;
    char *strdata = NULL;
    s32 i;

    typetable = (scrTypeTable_t *)&header[1];
    if (*(s32 *)header->MagicCode != 0x30574C46) {
        return NULL;
    }
    for (i = 0; i < header->TypeTableCount; i++) {
        switch (typetable[i].Type) {
        case SCR_DATATYPE_PROCTABLE:
            proctable = (scrProcTable_t *)((u8 *)header + typetable[i].ItemAddress);
            break;
        case SCR_DATATYPE_LABELTABLE:
            labeltable = (scrLabelTable_t *)((u8 *)header + typetable[i].ItemAddress);
            break;
        case SCR_DATATYPE_SCRDATA:
            code = (scrCode_u *)((u8 *)header + typetable[i].ItemAddress);
            break;
        case SCR_DATATYPE_MESSAGE:
            if (typetable[i].ItemCount != 0) {
                mesdata = (u8 *)header + typetable[i].ItemAddress;
            }
            break;
        case SCR_DATATYPE_STRINGS:
            strdata = (char *)header + typetable[i].ItemAddress;
            break;
        default:
            return NULL;
        }
    }
    return scrPrintProcedureStart(header, typetable, proctable, labeltable, code, mesdata, strdata,
                                  procno);
}
s32 scrHandleCount = 0;

scrProcessWork_s *scrHandleFirst = NULL;

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList_00111FF8", scrHandleLast);

