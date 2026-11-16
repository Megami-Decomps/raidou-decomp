#ifndef EVT_POLYGON_MOVIE_H
#define EVT_POLYGON_MOVIE_H

#include "event.h"
#include "effect.h"

#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
/* Raidou's 44-byte frame record (PM2 files whose header kind is not 4): pmdFrameTable_t's
   first four fields, two reserved halfwords, then 32 bytes of data. */
typedef union pmdFrameData_t {
    s8 b[32];
    u8 ub[32];
    s16 h[16];
    u16 uh[16];
    s32 w[8];
    f32 f[8];
    struct { u16 uh[2]; s8 nameIndex[4]; } link; /* MG1/MG2: four linked name-table indices */
} pmdFrameData_t;

typedef struct pmdFrameTable2_t {
    u16 ObjectType;
    u16 Frame;
    u16 Length;
    s16 NameIndex;
    s16 Reserve1;
    s16 Reserve2;
    pmdFrameData_t data;
} pmdFrameTable2_t;
SIZE_CHECK(pmdFrameTable2SizeCheck, pmdFrameTable2_t, 0x2C);

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
    pmdHeader_t *header1;         /* 0x030: PM1 file */
    pmdEntry_t *entryTable1;      /* 0x034 */
    char (*nameTable1)[32];       /* 0x038 */
    s32 nameCount1;               /* 0x03C */
    pmdStageTable_t *stageTable;  /* 0x040 */
    s32 stageCount;               /* 0x044 */
    void *stageF1;                /* 0x048 */
    void *stageF2;                /* 0x04C */
    void *stageTB;                /* 0x050 */
    pmdUnitTable_t *unitTable;    /* 0x054 */
    s32 unitCount;                /* 0x058 */
    void *type9Data;              /* 0x05C */
    pmdEffectTable_t *effectTable; /* 0x060 */
    s32 effectCount;              /* 0x064 */
    void *type8Data;              /* 0x068 */
    void *messageData;            /* 0x06C */
    pmdTmxTable_t *tmxTable;      /* 0x070 */
    s32 tmxCount;                 /* 0x074 */
    void *pm1Type23Data;          /* 0x078 */
    struct fileRequest *pm2Request; /* 0x07C */
    pmdHeader_t *header2;         /* 0x080: PM2 cut */
    pmdEntry_t *entryTable2;      /* 0x084 */
    char (*nameTable)[32];        /* 0x088 */
    s32 nameCount2;               /* 0x08C */
    pmdCutInfo_t *cutInfo;        /* 0x090 */
    pmdFrameTable_t *frameTable;  /* 0x094: PM2 kind 4 frame records */
    pmdFrameTable2_t *frameTable2; /* 0x098: frame records of other kinds */
    s32 frameCount;               /* 0x09C */
    void *cameraTable;            /* 0x0A0 */
    s32 cameraCount;              /* 0x0A4 */
    pmdSceneLightTable_t *slightTable; /* 0x0A8: kind 11 holds three-light records (pmdSceneLightTable1_t) */
    s32 slightCount;              /* 0x0AC */
    pmdSceneFogTable_t *sfogTable; /* 0x0B0 */
    s32 sfogCount;                /* 0x0B4 */
    dds3Effect_Blur_t *blur2Table; /* 0x0B8 */
    s32 blur2Count;               /* 0x0BC */
    dds3Effect_PartialMultiBlurParam_t *mblurTable; /* 0x0C0 */
    s32 mblurCount;               /* 0x0C4 */
    dds3Effect_DistortionBlurParam_t *dblurTable; /* 0x0C8 */
    s32 dblurCount;               /* 0x0CC */
    dds3Effect_Filter_t *filterTable; /* 0x0D0 */
    s32 filterCount;              /* 0x0D4 */
    dds3Effect_PartialMultiFilterParam_t *mfilterTable; /* 0x0D8 */
    s32 mfilterCount;             /* 0x0DC */
    dds3Effect_RippleBlurParam_t *rblurTable; /* 0x0E0 */
    s32 rblurCount;               /* 0x0E4 */
    pmdHeikouBlurTable_t *hblurTable; /* 0x0E8 */
    s32 hblurCount;               /* 0x0EC */
    struct evtGroupMetadata_t *objectTable; /* 0x0F0: group links */
    s32 objectCount;              /* 0x0F4 */
    void *rainDataTable;          /* 0x0F8 */
    s32 rainDataCount;            /* 0x0FC */
    struct fileRequest *pm3Request; /* 0x100 */
    pmdHeader_t *header3;         /* 0x104: PM3 cut */
    pmdEntry_t *entryTable3;      /* 0x108 */
    pmdFrameTable2_t *soundFrameTable; /* 0x10C */
    s32 soundFrameCount;          /* 0x110 */
    sdfMemHandle_t *pm1File;      /* 0x114 */
    void *pm1Data;                /* 0x118: locked PM1 file */
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

evtPolygonMovieWork_t *GetWorkData1(evtPolygonMovieWork_t *wdata, pmdHeader_t *pm1data);
evtPolygonMovieWork_t *InitPolygonMovieWorkData(void);
void evtCheckLoad(evtPolygonMovieWork_t *wdata);
s32 evtCheckSeqEnd(dds3ProcessID_t *id);
s32 evtCheckRun(dds3ProcessID_t *id);
f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2);
void evtHokanSceneLight(s32 hokan, f32 ratio, pmdSceneLightTable_t *key1, pmdSceneLightTable_t *key2, pmdSceneLightTable_t *d);
void evtFreeWorkData1(evtPolygonMovieWork_t *wdata);
void evtFreeWorkData2and3(evtPolygonMovieWork_t *wdata);
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
evtPolygonMovieWork_t *evtGetWorkData2and3(evtPolygonMovieWork_t *wdata, pmdHeader_t *pm2cut, pmdHeader_t *pm3cut);
#else
struct pmdProcessWork_t *evtGetWorkData2and3(struct pmdProcessWork_t *wdata, struct pmdSeqData_t *pm2dataParam, struct pmdSeqData_t *pm3dataParam);
#endif
void evtMoverFrameSet(dds3Basic_t *mover, s32 sw, s32 baseframe, s32 nowframe, s32 offset);
void evtResetFlag(dds3ProcessID_t *id, PMD_FLAG flag);
void evtRunUnitMode(dds3Basic_t *unit, PMD_UNIT mode, u32 addflag, u32 downflag);
void evtSetFlag(dds3ProcessID_t *id, PMD_FLAG flag);
dds3ProcessID_t *evtStart2(DDS3_PRIORITY priority, s32 eventno, s32 cutno, s32 seqno);
s32 evtStopMover(dds3Basic_t *mover, s32 sw, s32 baseframe, s32 nowframe);

#endif
