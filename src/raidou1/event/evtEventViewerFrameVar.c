#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtEventViewerFrameVar.c */
#include "common.h"

#include "kernel.h"

#include "basic.h"

#include "event.h"

extern evtViewerInfo_t *viewerinfo;

extern const char evtFrameVarName[];

extern char evtFrameNumFmt[];

extern sdfGrOt_t sdfGrOtTable[];

extern sdfGrOt_t sdfDebugGrOt;

extern void *dds3FontReqAllocDrawTag(void);

extern void dds3ConsPanelTagLink(void *aTag, s32 aX, s32 aY, s32 aSx, s32 aSy);

extern s32 sdfDevPrintf(s32 aX, s32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(void *aTag, addr_t aPacket);

extern void sdfPrimLinkRect(void *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                            s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

s32 FrameVarProcess(dds3ProcessID_t *id);

dds3ProcessID_t *evtInitFrameVar(void);

extern const char evtFrameVarName[24];

extern const char evtFrameHeadSoftenBias[16];
/* End of declarations from src/smt3/event/evtEventViewerFrameVar.c */

dds3ProcessID_t *evtInitFrameVar(void)
{
    return dds3StartProcess(evtFrameVarName, 0x2AF9, 1, 1, FrameVarProcess, NULL, NULL);
}

const char evtFrameVarName[24] = "FrameVar";

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0044FEC0);

