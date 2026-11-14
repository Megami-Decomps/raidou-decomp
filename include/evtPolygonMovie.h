#ifndef EVT_POLYGON_MOVIE_H
#define EVT_POLYGON_MOVIE_H

#include "event.h"
#include "effect.h"

#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
/* Raidou's work (0x934, sdfMemAllocDirect): PM1 and PM2 directories, then a pointer to each
   PM2 and PM3 cut record. */
typedef struct evtPolygonMovieWork_t {
    sdfMemHandle_t *mem;          /* 0x000 */
    u32 unk004;                   /* 0x004 */
    s32 eventNo;                  /* 0x008 */
    s32 cutNo;                    /* 0x00C */
    u32 unk010;                   /* 0x010 */
    void *chipBlock;              /* 0x014: 0x1FC bytes from sdfChipAllocDirect */
    s32 unk018;                   /* 0x018 */
    u32 unk01C;                   /* 0x01C */
    s32 pm2CutCount;              /* 0x020 */
    s32 pm3CutCount;              /* 0x024 */
    s32 cutIndex;                 /* 0x028: cutNo - 1 */
    u32 unk02C;                   /* 0x02C */
    void *header1;                /* 0x030: PM1 file */
    pmdEntry_t *entryTable1;      /* 0x034 */
    void *pm1Type1Data;           /* 0x038 */
    s32 pm1Type1Value;            /* 0x03C */
    void *pm1Type2Data;           /* 0x040 */
    s32 pm1Type2Value;            /* 0x044 */
    u32 unk048[3];                /* 0x048 */
    void *pm1Type3Data;           /* 0x054 */
    s32 pm1Type3Value;            /* 0x058 */
    u32 unk05C[9];                /* 0x05C */
    void *header2;                /* 0x080: PM2 cut */
    pmdEntry_t *entryTable2;      /* 0x084 */
    void *pm2Type1Data;           /* 0x088 */
    s32 pm2Type1Value;            /* 0x08C */
    void *pm2Type0Data;           /* 0x090 */
    u32 unk094[32];               /* 0x094 */
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

#endif
