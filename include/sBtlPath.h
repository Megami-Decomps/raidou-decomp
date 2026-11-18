#ifndef SBTLPATH_H
#define SBTLPATH_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

typedef struct SBTLENMY_ACTDATA {
    s32 pathid;
    SBTLENMY_FLG flg;
    SBTL_ACTAIMODE type;
    s32 frame;
    s16 cnt;
    s16 jmpid;
} SBTLENMY_ACTDATA;

typedef struct SBTLENMY_PATHDATA {
    sdfVector4_t nowpos;
    sdfVector4_t nextpos;
} SBTLENMY_PATHDATA;

extern s16 tbldatacnt[];
extern SBTLENMY_PATHDATA pathdata[5];
extern SBTLENMY_ACTDATA *data[4];

s32 sbtlSetNextPathData(struct SBTLUNIT_WORK *wk);
s16 sbtlGetPathDataCount(s16 id);
SBTLENMY_ACTDATA *sbtlGetPathACTData(s16 id, s16 no);
SBTLENMY_PATHDATA *sbtlGetPathData(s16 id);

#endif
