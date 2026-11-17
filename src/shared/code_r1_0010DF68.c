#include "common.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdk_libkernl.h"
#include "evtStage.h"
#include "dds3GlobalWork.h"
#include "sdfPrim.h"
#include "frFont.h"
#include "evtSky.h"
#include "evtDebug.h"
#include "sdfChip.h"
#include "sdfMemory.h"
#include "event.h"
#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_packet_alloc.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "ps2asm.h"
#include "ModeSelect.h"
#include "code_00300578.h"
#include "shpMessage.h"
#include "code_00195698.h"
#include "neko.h"
#include "jmsMain.h"
#include "rstInit.h"
#include "code_00323280.h"
#include "gohInit.h"
#include "code_r1_0010DF68.h"
#include "code_r1_002CD5A8.h"
#include "dds3KernelCore.h"
#include "fldCommand.h"
#include "code_002D7950.h"
#include "code_0034F3B8.h"
#include "mviManager.h"

s32 DIntr();
s32 EIntr(void);

#define SDF_MEM_BLOCK_STATE_FREE 0

#define SDF_MEM_BLOCK_STATE_USED 1

#define SDF_MEM_BLOCK_STATE_END_SENTINEL 2


/* Update the console state stored at the corresponding field offsets. */

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Read the active packet cursor or restore it at a quadword boundary. */

/* Return NULL when the following link is the end marker. */

/* Return the descriptor's payload base as a byte address, not the descriptor. */

/* Borrowed read-only descriptors; exclude heap mutation while traversing. */

/* Return free/used/end-sentinel state, not a resource kind. */

/* Start the background fade-out countdown; zero clears the transition. */
void dds3FadeBackgroundStartOut(s32 duration);

typedef struct sdfHeapOverlayBlock_t {
    u32 address;   /* 0x00 */
    u8 blockState; /* 0x04: compact snapshot of SdfMemBlockState */
    u8 opaque05[3];
    s32 size;     /* 0x08 */
} sdfHeapOverlayBlock_t;

typedef char sdfHeapOverlayBlock_size[(sizeof(sdfHeapOverlayBlock_t) == 0x0C) ? 1 : -1];

#ifdef VERSION_RAIDOU1
extern void evtViewerCreateTaskWithSky(void);
#else
INCLUDE_ASM(const s32, "code_r1_0010DF68", evtCreateTestAndSkyTasks);

INCLUDE_ASM(const s32, "code_r1_0010DF68", evtInitializeSelectionScene);

INCLUDE_ASM(const s32, "code_r1_0010DF68", evtKeepTestSkyModePending);

INCLUDE_ASM(const s32, "code_r1_0010DF68", evtClearSecondaryWorldAndViewer);

INCLUDE_ASM(const s32, "code_r1_0010DF68", evtInitializeSelectionScreen);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf26);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf27);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf28);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf29);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf30);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc22);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc23);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf31);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc24);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc25);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallInitSky);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallExitSky);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf32);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf33);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallProcessEndRstInit);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallProcessCheckRstInit);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf34);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf35);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf36);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf37);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf38);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf39);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf40);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc26);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc27);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf41);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallFinishMovieViewer);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf42);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf43);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc28);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf44);

INCLUDE_ASM(const s32, "code_r1_0010DF68", TestProcess3);

INCLUDE_ASM(const s32, "code_r1_0010DF68", evtCallTestProcess3);

INCLUDE_ASM(const s32, "code_r1_0010DF68", evtCallTestProcess32);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallShutDown);

INCLUDE_ASM(const s32, "code_r1_0010DF68", CheckLmap);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf32);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallShutDown2);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf33);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf34);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallInit);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf35);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf36);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc18);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc19);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf37);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc20);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc21);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf38);

INCLUDE_ASM(const s32, "code_r1_0010DF68", ExitHanafudaTest);

INCLUDE_ASM(const s32, "code_r1_0010DF68", CheckHanafudaTest);

INCLUDE_ASM(const s32, "code_r1_0010DF68", CallGomaden);

INCLUDE_ASM(const s32, "code_r1_0010DF68", ExitGomaden);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf39);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallDebugProcessEnd);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallDebugProcessCheck);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf40);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc22);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc23);
#endif

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};

extern u32 D_00435D28;
extern u32 dds3KernelDebugSign;
extern sdfDevConsCB_tag *evtConsoleFontResourceChain;
extern void evtEventViewerDestroyTask(void);
extern sdfGrOt_t sdfDebugGrOt;
extern s8 jmsChkCommonInputData4[];
extern void evtDrawHeapUsageOverlay(sdfGrOt_t *);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct evtDrawState_t {
    u8 pad00[0x1C];
    f32 value1C;
} evtDrawState_t;

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct evtGsCommand_t {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} evtGsCommand_t;

extern s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *);

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

dds3ProcessID_t *evtCreateTestAndSkyTasks(void);
u32 evtInitializeSelectionScene(void);
u32 evtKeepTestSkyModePending(void);
void evtClearSecondaryWorldAndViewer(void);
u32 evtInitializeSelectionScreen(void);
u32 evtEnsureFontResourceChain(void);
void evtDestroyFontResourceChain(void);
extern u16 D_00435D40;
extern u16 D_00435D42;
extern char evtGeneralHeapPercentFormat[];
s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task);
extern u32 D_00435D4C;

void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState);

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);
extern char D_00435D50[];
extern char D_00435D58[];
extern void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface);
s32 evtDrawConditionalHeapUsageOverlay(void);
extern s32 effGetFontListCount(void);
extern const char D_004113A0[];
s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);
extern s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);
#ifdef VERSION_RAIDOU1

dds3ProcessID_t *evtCreateTestAndSkyTasks(void) {
    dds3KernelDebugSign = 0;
    D_00435D28 = 0;
    evtInitEventTest();
    return evtInitSky();
}

u32 evtInitializeSelectionScene(void) {
    dds3KernelDebugSign = 1;
    dds3FadeBackgroundStartOut(0);
    evtChkEndProcess();
    evtExitSky();
    return 1;
}

/* Admin mode 16 cleanup: zero leaves the mode pending. */
u32 evtKeepTestSkyModePending(void) {
    return 0;
}

void evtClearSecondaryWorldAndViewer(void) {
    dds3KernelDebugSign = 0;
    evtFreeStage();
    evtViewerCreateTaskWithSky();
}

u32 evtInitializeSelectionScreen(void) {
    evtEventViewerDestroyTask();
    dds3KernelDebugSign = 1;
    dds3FadeBackgroundStartOut(0);
    return 0;
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf26);

void sysLeaf27(void) {
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf28);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf29);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E038.c:1, match */
void sysLeaf30(void) {
    cmpProcessStart_cmpMain();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc22);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc23);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E090.c:1, match */
void sysLeaf31(void) {
    gohShopDebug();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc24);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc25);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallInitSky);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallExitSky);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf32);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E130.c:1, match */
dds3ProcessID_t *sysLeaf33(void) {
    return ProcessStart_rstInit();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallProcessEndRstInit);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallProcessCheckRstInit);

void sysLeaf34(void) {
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf35);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf36);

void sysLeaf37(void) {
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf38);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf39);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E1B8.c:1, match */
void sysLeaf40(void) {
    mjnTestFontCalc();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc26);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc27);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E210.c:1, match */
void sysLeaf41(void) {
    InitMovieViewer();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysCallFinishMovieViewer);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf42);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E250.c:1, match */
void sysLeaf43(void) {
    fclLeaf12();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysFunc28);

INCLUDE_ASM(const s32, "code_r1_0010DF68", sysLeaf44);

s32 sysLeaf(s32 restored, void *reqdata)
{
    if (reqdata == NULL) {
        return getLmpMainProc(0);
    }
    return getLmpMainProc(*(s32 *)reqdata);
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", ExitLmap);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf);

s32 datLeaf2(void) {
    return Start_mjn();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallShutDown);

INCLUDE_ASM(const s32, "code_r1_0010DF68", CheckLmap);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E328.c:1, match */
void datLeaf32(void) {
    mphStart_itf();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallShutDown2);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf33);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E368.c:1, match */
void datLeaf34(void) {
    stfCtnTestProcess();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallInit);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf35);

s32 datLeaf36(s32 restored, void *reqdata)
{
    if (reqdata == NULL) {
        return fldCommand_SET_ALLPANEL(3);
    }
    return fldCommand_SET_ALLPANEL(*(s32 *)reqdata);
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc18);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc19);

s32 datLeaf37(s32 restored, void *reqdata)
{
    if (reqdata == NULL) {
        return fldCommand_SET_ALLPANEL(0);
    }
    return fldCommand_SET_ALLPANEL(*(s32 *)reqdata);
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc20);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc21);

void datLeaf3(void) {
    titleFunc7();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf4);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E4C8.c:1, match */
void datLeaf38(void) {
    frTestFontInit();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", ExitHanafudaTest);

INCLUDE_ASM(const s32, "code_r1_0010DF68", CheckHanafudaTest);

void datLeaf5(void) {
    fclLeaf();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc2);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc3);

s32 CallGomaden(s32 restored, void *reqdata)
{
    if (reqdata == NULL) {
        return fldFacilityMistDraw(1);
    }
    return fldFacilityMistDraw(*(s32 *)reqdata);
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", ExitGomaden);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E5B0.c:1, match */
s32 datLeaf39(void) {
    return gohCheckSequence();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallDebugProcessEnd);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallDebugProcessCheck);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0010E628.c:1, match */
void datLeaf40(void) {
    cmpStartProcessPanelEff();
}

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc22);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datFunc23);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf6);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datCallInit2);

INCLUDE_ASM(const s32, "code_r1_0010DF68", datLeaf7);
#endif

u32 evtEnsureFontResourceChain(void) {
    if (evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsOpen(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetMode(evtConsoleFontResourceChain, 2);
        sdfDevConsAttribute(evtConsoleFontResourceChain, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (evtConsoleFontResourceChain != 0) {
        sdfDevConsClose(evtConsoleFontResourceChain);
        evtConsoleFontResourceChain = 0;
    }
}

extern char datDebugPrintfFmt[];

void datDebugPrintf(const char *format, ...)
{
    char buf[512];
    __builtin_va_list args;

    if (evtConsoleFontResourceChain != 0) {
        __builtin_stdarg_start(args, format);
        vsprintf(buf, format, args);
        sdfDevConsPrintf(evtConsoleFontResourceChain, datDebugPrintfFmt, buf);
    }
}

extern char datDEBUGCONSOLEData[];

void datDEBUGCONSOLE(const char *fmt, ...)
{
    char buf[512];
    __builtin_va_list args;

    if (dds3SearchProcessName("DEBUG CONSOLE") == NULL && evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsOpen(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetMode(evtConsoleFontResourceChain, 2);
        sdfDevConsAttribute(evtConsoleFontResourceChain, 7);
    }
    if (evtConsoleFontResourceChain != 0) {
        __builtin_stdarg_start(args, fmt);
        vsprintf(buf, fmt, args);
        sdfDevConsPrintf(evtConsoleFontResourceChain, datDebugPrintfFmt, buf);
        printf(datDEBUGCONSOLEData, buf);
    }
}

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task) {
    sdfMemInfo_t heapStats;
    sdfDrawTag_t *list;
    void *packet;
    s32 freePercent;
    s32 originX = 0x7100;
    s32 currentColumn;
    s32 peakColumn;

    sdfMemGetInfo(&heapStats);
    freePercent = heapStats.freeSize * 100 / heapStats.total;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70D0, 0x85A8, 0xFFFF7F, 0x1260, 0xC0, 0x20000000, 0x40806040));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x85C0, 0xFFFF80, 0, evtGeneralHeapPercentFormat, freePercent));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8080, 0x85C0, 0xFFFF80, 6, evtGeneralHeapPercentFormat, D_00435D40));
    currentColumn = (s32)((f32)freePercent / 100.0f * 200.0f) + 0x2A;
    sdfPrimLinkRect(list, 0x8000C000, 0, 0x73A0, 0x85F0, currentColumn * 16 + originX, 0x8600, 0xFF0000, 0);
    peakColumn = (s32)((f32)D_00435D40 / 100.0f * 200.0f) + 0x2B;
    sdfPrimLinkRect(list, 0x8000FFFF, 0, peakColumn * 16 + originX, 0x85D0, (peakColumn + 2) * 16 + originX, 0x8610, 0xFF0000, 0);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (D_00435D42 >= 0x47 || D_00435D40 < freePercent) {
        D_00435D40 = freePercent;
        D_00435D42 = 0;
    } else {
        D_00435D42 = D_00435D42 + 1;
    }
    return 0;
}

void datLeaf41(void) {
}

void evtToggleDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        evtToggleDebugTimeGraphTaskWork = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, evtDrawGeneralHeapUsageGraph,
                                                           (dds3ProcessFunc_t)datLeaf41, NULL);
    } else if (mode == 0) {
        dds3KillProcess(evtToggleDebugTimeGraphTaskWork, 0);
    }
}

/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState) {
    s32 color;
    u32 byteOffset;
    s32 rowIndex;
    s32 columnIndex;
    s32 leftX;
    s32 topY;
    u32 bytesPerRow;
    u32 bytesPerCell;

    switch (blockState) {
    case SDF_MEM_BLOCK_STATE_FREE:
        color = 0x80000000;
        break;
    case SDF_MEM_BLOCK_STATE_USED:
        color = 0x80008080;
        break;
    case SDF_MEM_BLOCK_STATE_END_SENTINEL:
        color = 0x80000080;
        break;
    default:
        color = 0x80800000;
        break;
    }
    byteOffset = blockAddress - heapBase;
    bytesPerRow = D_00435D4C >> 8;
    bytesPerCell = D_00435D4C >> 14;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_00435D4C < blockSize || blockSize < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 64 - columnIndex;

        if (blockSize >= remainingCells / bytesPerCell) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize -= remainingCells / (D_00435D4C >> 14);
        } else {
            s32 cellCount = blockSize / bytesPerCell;

            if (cellCount == 0 && blockSize > 0) {
                cellCount = 1;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize = 0;
        }
        topY += 8;
    }
    while (blockSize >= (D_00435D4C >> 8)) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + 0x400, topY + 8, 0x0FFFFF80, NULL);
        blockSize -= D_00435D4C >> 8;
        topY += 8;
    }
    if (blockSize > 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + (blockSize / (D_00435D4C >> 14)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y) {
    sdfHeapOverlayBlock_t *snapshot;
    sdfHeapOverlayBlock_t *entry;
    sdfMemHandle_t *block;
    u32 count;
    u32 i;
    u32 heapBase;
    u32 offset;
    s32 restoreInterrupts;
    s32 markerX;
    s32 markerY;

    snapshot = (sdfHeapOverlayBlock_t *)sdfGrOpenPacketBuff();
    restoreInterrupts = DIntr();
    block = sdfMemGetTop();
    entry = snapshot;
    heapBase = EE_ADDR(sdfMemGetPtr(block));
    /* Count only the interrupt-protected snapshot traversal. */
    count = 0;
    do {
        count++;
        entry->address = EE_ADDR(sdfMemGetPtr(block));
        entry->blockState = (u8)sdfMemGetType(block);
        entry->size = sdfMemGetSize(block);
        entry++;
        block = sdfMemGetNext(block);
    } while (block != NULL);
    if (restoreInterrupts) {
        EIntr();
    }
    sdfGrClosePacketBuff((s32)entry);

    entry = snapshot;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
        0x0FFFFF7F, 0x440, (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x20,
        0x80000000, 0x80806020));
    for (i = 0; i < count; i++) {
        sdfDrawHeapSpanOverlay(packetList, x, y, heapBase, entry->address,
            entry->size, entry->blockState);
        entry++;
    }
    markerY = y;
    markerX = x + 0x420;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        x + 0x4C0, markerY, 0x0FFFFF80, NULL);
    for (offset = 0x100000; offset < D_00435D4C; offset += 0x100000) {
        markerY = y + (offset / (D_00435D4C >> 8)) * 8;
        sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
            x + 0x470, markerY, 0x0FFFFF80, NULL);
    }
    markerY = y + (D_00435D4C / (D_00435D4C >> 8)) * 8;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        markerX + 0xA0, markerY, 0x0FFFFF80, NULL);
}

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface) {
    sdfMemInfo_t generalHeapStats;
    sdfChipInfo_t chipHeapStats;
    char statusText[100];
    sdfDrawTag_t *packetList;

    sdfMemGetInfo(&generalHeapStats);
    D_00435D4C = generalHeapStats.total;
    sdfChipGetInfo(&chipHeapStats);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawHeapMapOverlay(packetList, 0x8AC0, 0x79C0);
    sprintf(statusText, D_00435D50, generalHeapStats.freeSize);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    sprintf(statusText, D_00435D58, chipHeapStats.freeSize);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->regFunc(surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (jmsChkCommonInputData4[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&sdfDebugGrOt);
    return 0;
}

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task) {
    sdfMemInfo_t heapStats;
    s32 heapRatio;
    sdfDrawTag_t *list;
    void *packet;
    s32 fontCount;
    s32 textCount;
    s32 gsCount;

    sdfMemGetInfo(&heapStats);
    /* The unused heap ratio retains the native zero-divisor check. */
    heapRatio = heapStats.freeSize / heapStats.total;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70d0, 0x7968, 0xffff7f,
                                          0xf70, 0x98, 0x20000000, 0x40806040));
    fontCount = frGetFontHandleNum();
    textCount = frGetTextHandleNum();
    gsCount = effGetFontListCount();
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x7980,
                    0xffff80, 0, D_004113A0, fontCount, textCount, gsCount));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    return 0;
}

INCLUDE_RODATA(const s32, "code_r1_0010DF68", evtStrFh03Th03Gs03);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "code_r1_0010DF68", evtCreateTestAndSkyTasksData);

#endif
INCLUDE_SDATA(const s32, "code_r1_0010DF68", evtConsoleFontResourceChain);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", datDebugPrintfFmt);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", datDEBUGCONSOLEData);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", evtDrawGeneralHeapUsageGraphData);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", evtGeneralHeapPercentFormat);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", sdfData2);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", evtDrawHeapUsageOverlayData);

INCLUDE_SDATA(const s32, "code_r1_0010DF68", evtDrawHeapUsageOverlayData2);

