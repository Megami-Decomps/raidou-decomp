#include "common.h"
#include "kernel.h"
#include "scr.h"
#include "event.h"
#include "field.h"
#include "sdk_libc.h"

extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;
extern s32 scrHandleCount;

extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                                scrProcTable_t *proctable,
                                                scrLabelTable_t *labeltable, scrCode_u *code,
                                                u8 *mesdata, char *strdata, s32 procno);

void scrLinkHandle(scrProcessWork_s *work);
void scrUnlinkHandle(scrProcessWork_s *work);

scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                         scrProcTable_t *proctable, scrLabelTable_t *labeltable,
                                         scrCode_u *code, u8 *mesdata, char *strdata, s32 procno);

scrProcessWork_s *fldFlowParse(scrHeader_t *header, s32 procno);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList_00114448", AddScript);

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

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList_00114448", scrHandleLast);

