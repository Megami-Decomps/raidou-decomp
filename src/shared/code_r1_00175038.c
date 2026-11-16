#include "common.h"
#include "fld.h"
#include "fldNpcPath.h"

extern mapNpcPathTableInfo_t mapNpcPathFastInitData;
extern mapNpcPathTableInfo_t mapNpcPathFastInitData2;
extern mapNpcPathTableInfo_t mapNpcPathFastInitData3;
extern mapNpcPathTableInfo_t mapNpcPathFastInitData4;

void mapNpcPathFastInit(void) {
    s32 i;

    DDS3_GBWK4[0].info = &mapNpcPathFastInitData;
    DDS3_GBWK4[1].info = &mapNpcPathFastInitData2;
    DDS3_GBWK4[2].info = &mapNpcPathFastInitData3;
    DDS3_GBWK4[3].info = &mapNpcPathFastInitData4;
    for (i = 0; i < 4; i++) {
        mapNpcPathMakeControl(i);
    }
}

void scrClearProcessGlobals(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        DDS3_GBWK4[i].used = 0;
        DDS3_GBWK4[i].unk2 = 0;
        DDS3_GBWK4[i].pBasic = NULL;
    }
}

#ifdef VERSION_RAIDOU1
extern mapNpcPathData_t mapNpcPathFastInitDataEntry2;
extern mapNpcPathData_t mapNpcPathFastInitDataEntry3;

mapNpcPathTable_t mapNpcPathFastInitDataEntry[2] = {
    { 0, &mapNpcPathFastInitDataEntry2 },
    { 2, &mapNpcPathFastInitDataEntry3 },
};
#endif
