#ifndef EVT_POLYGON_MOVIE_H
#define EVT_POLYGON_MOVIE_H

#include "event.h"
#include "effect.h"

#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
/* Raidou's work (0x934, sdfMemAllocDirect): PM1 and PM2 directories, then a pointer to each
   PM2 and PM3 cut record. */
typedef struct evtPolygonMovieWork_t {
    sdfMemHandle_t *mem;          /* 0x000 */
    u32 loadFlags;                /* 0x004: 2 PM1, 4 PM2, 0x10 PM3 requested */
    union {
        s32 eventNo;              /* 0x008 */
        u16 eventNoLow;           /* the viewer reads the low halfword */
    };
    union {
        s32 cutNo;                /* 0x00C */
        u16 cutNoLow;
    };
    u32 unk010;                   /* 0x010 */
    void *chipBlock;              /* 0x014: 0x1FC bytes from sdfChipAllocDirect */
    s32 MesHandle;                /* 0x018: message handle, -1 none */
    struct dds3ProcessID_t *task; /* 0x01C: viewer task */
    s32 pm2CutCount;              /* 0x020 */
    s32 pm3CutCount;              /* 0x024 */
    s32 seqIndex;                 /* 0x028: seqno - 1 */
    struct fileRequest *pm1Request; /* 0x02C */
    void *header1;                /* 0x030: PM1 file */
    pmdEntry_t *entryTable1;      /* 0x034 */
    void *pm1Type1Data;           /* 0x038 */
    s32 pm1Type1Value;            /* 0x03C */
    void *pm1Type2Data;           /* 0x040 */
    s32 pm1Type2Value;            /* 0x044 */
    u32 unk048[3];                /* 0x048 */
    pmdUnitTable_t *unitTable;    /* 0x054: PM1 type 3 */
    s32 unitCount;                /* 0x058 */
    u32 unk05C[8];                /* 0x05C */
    struct fileRequest *pm2Request; /* 0x07C */
    void *header2;                /* 0x080: PM2 cut */
    pmdEntry_t *entryTable2;      /* 0x084 */
    void *pm2Type1Data;           /* 0x088 */
    s32 pm2Type1Value;            /* 0x08C */
    void *pm2Type0Data;           /* 0x090 */
    u32 unk094[27];               /* 0x094 */
    struct fileRequest *pm3Request; /* 0x100 */
    u32 unk104[4];                /* 0x104 */
    void *pm1File;                /* 0x114 */
    u32 unk118;                   /* 0x118 */
    void *pm2Cuts[256];           /* 0x11C */
    void *pm3Cuts[256];           /* 0x51C */
    s32 pm2Size;                  /* 0x91C */
    sdfMemHandle_t *pm2Mem;       /* 0x920 */
    void *pm2Data;                /* 0x924 */
    s32 pm3Size;                  /* 0x928 */
    sdfMemHandle_t *pm3Mem;       /* 0x92C */
    void *pm3Data;                /* 0x930 */
} evtPolygonMovieWork_t;
SIZE_CHECK(evtPolygonMovieWorkSizeCheck, evtPolygonMovieWork_t, 0x934);
#else
typedef struct evtPolygonMovieWork_t {
    u32 movieFlags;
    void *file04;
    sdfMemHandle_t *mem08;
    void *pm1data;
    void *header1;
    pmdEntry_t *entryTable1;
    char (*nameTable1)[32];
    s32 nameCount1;
    pmdStageTable_t *stageTable;
    s32 stageCount;
    void *stageF1;
    void *stageF2;
    void *stageTB;
    pmdUnitTable_t *unitTable;
    s32 unitCount;
    void *type9Data;
    pmdEffectTable_t *effectTable;
    s32 effectCount;
    void *type8Data;
    void *file4C;
    sdfMemHandle_t *mem50;
    void *pm2data;
    pmdHeader_t *header2;
    pmdEntry_t *entryTable2;
    char (*nameTable)[32];
    s32 nameCount2;
    pmdCutInfo_t *cutInfo;
    pmdFrameTable_t *frameTable;
    s32 frameCount;
    void *cameraTable;
    s32 cameraCount;
    pmdSceneLightTable_t *slightTable;
    s32 slightCount;
    pmdSceneFogTable_t *sfogTable;
    s32 sfogCount;
    dds3Effect_Blur_t *blur2Table;
    s32 blur2Count;
    dds3Effect_PartialMultiBlurParam_t *mblurTable;
    s32 mblurCount;
    dds3Effect_DistortionBlurParam_t *dblurTable;
    s32 dblurCount;
    dds3Effect_Filter_t *filterTable;
    s32 filterCount;
    dds3Effect_PartialMultiFilterParam_t *mfilterTable;
    s32 mfilterCount;
    dds3Effect_RippleBlurParam_t *rblurTable;
    s32 rblurCount;
    s32 mesHandle;
    struct dds3ProcessID_t *processId;
    s32 eventNo;
    s32 cutNo;
    s32 nowFrame;
    dds3Basic_t **effectBasicTable;
} evtPolygonMovieWork_t;
#endif

s32 evtHokanInt(s32 hokan, f32 ratio, s32 i1, s32 i2);
f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2);
void evtHokanSceneLight(s32 hokan, f32 ratio, pmdSceneLightTable_t *key1, pmdSceneLightTable_t *key2, pmdSceneLightTable_t *d);
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d);
void evtHokanHeikouBlur(s32 enable, f32 t, const dds3Effect_Blur_t *a, const dds3Effect_Blur_t *b, dds3Effect_Blur_t *out);
void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d);
void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d);
void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);
void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d);
void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d);
void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d);

#include "basic.h"
#include "enums.h"
#include "kernel.h"

struct pmdProcessWork_t;
struct pmdSeqData_t;

struct pmdProcessWork_t *GetWorkData1(struct pmdProcessWork_t *wdata, void *pm1data);
struct pmdProcessWork_t *InitPolygonMovieWorkData(void);
void evtCheckLoad(evtPolygonMovieWork_t *wdata);
s32 evtCheckSeqEnd(dds3ProcessID_t *id);
void evtFreeWorkData1(evtPolygonMovieWork_t *wdata);
void evtFreeWorkData2and3(evtPolygonMovieWork_t *wdata);
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
evtPolygonMovieWork_t *evtGetWorkData2and3(evtPolygonMovieWork_t *wdata, void *pm2dataParam, void *pm3dataParam);
#else
struct pmdProcessWork_t *evtGetWorkData2and3(struct pmdProcessWork_t *wdata, struct pmdSeqData_t *pm2dataParam, struct pmdSeqData_t *pm3dataParam);
#endif
void evtMoverFrameSet(dds3Basic_t *mover, s32 sw, s32 baseframe, s32 nowframe, s32 offset);
void evtResetFlag(dds3ProcessID_t *id, PMD_FLAG flag);
void evtRunUnitMode(dds3Basic_t *unit, PMD_UNIT mode, u32 addflag, u32 downflag);
void evtSetFlag(dds3ProcessID_t *id, PMD_FLAG flag);
dds3ProcessID_t *evtStart2(DDS3_PRIORITY priority, s32 eventno, s32 cutno, s32 seqno);
void evtStopMover(dds3Basic_t *mover, s32 sw, s32 baseframe, s32 nowframe);

#endif
