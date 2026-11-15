#include "common.h"
#include "sdfFrame.h"
#include "sdk_libgcc.h"
#include "pbRecord.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

typedef struct pbProcHandle_t pbProcHandle_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 flg;
    s16 x, y;
    s32 dir;
    u8 dmy[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct { s16 number, label; } pbStageEntry_t;

typedef struct { u16 blk[20][20]; } pbMapBlock_t;

typedef struct { u8 unk00[0x10]; pbMapBlock_t map; } pbMapArea_t;

typedef struct
{
    s32 pcharNum;
    u8 unk004[8];
    pbMapArea_t area;
    s32 charIndex;
    void *baseTex;
    void *tex344, *tex348, *tex34C;
    sdfMemHandle_t *mapData;
    u8 *giveupScript;
    s16 level, stage;
    u8 unk35C[4];
    struct pbProcHandle_t *menuProc;
    struct pbProcHandle_t *helpProc;
    u8 reserved368[4];
    s32 drawCount;
} pbMapWork_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 flg;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbExit_t;

typedef struct
{
    f32 scale;
    u8 unk04[0xC];
    f32 eye[4], target[4];
} pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct
{
    u16 kind;
    s16 x, y;
    u16 outline_num;
    u8 reserved08[8];
    f32 outlineVertices[8][4];
} pbBlockEntry_t;

typedef struct
{
    s32 value;
    u16 grpID;
    u16 num;
    u8 unk08[8];
    u16 kind;
    s16 cellX, cellY;
    u8 unk16[0x8A];
    sdfMemHandle_t *blockMem;
    /* Unrecovered bytes between the allocation handle and locked block pointer. */
    u8 blockStorageGap[0xC];
    pbBlockEntry_t *blk;
    union {
        sdfVector4_t rotation;
        struct { f32 rotationX, rot, rotationZ; u8 reservedC0[4]; };
    };
    f32 x, y, z;
    u8 reservedD0[4];
    u32 colors[8];
    u8 reservedF4[0xC];
} pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { s16 dir; u16 step; f32 speed; s32 unk08; } pbMoveCharWork_t;

typedef struct { s32 dir; f32 angle; } pbRoundCamWork_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { s16 x, y; } pbGoalPos_t;

typedef struct { u8 unk00[8]; pbGoalPos_t pos; } pbGoal_t;

typedef struct
{
    s16 clearBit;
    s16 unk02;
    pbStageEntry_t stage[1];
} pbLevel_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbExit_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; pbMoveCharWork_t *moveChar;
            pbRoundCamWork_t *roundCam; } data;
};

extern f32 cameraAngle, cameraRadius, cameraDegree, cameraDegreeInit;
extern pbProcHandle_t *record;
typedef void (*pbCharacterFunc_t)(void);
extern pbProcHandle_t *blockDB;
extern char pbBlockDbProcName[], pbDrawMapbaseProcName[];
extern s32 moveChar(dds3ProcessID_t *id);
extern const char pbMoveCharProcName[], pbMoveBlockProcName[], pbGameProcName[], pbMoveCameraProcName[];
extern s16 pbMyCharPosX, pbMyCharPosY;

typedef struct { s16 x, y; } pbKeyEntry_t;

extern pbProcHandle_t *bmap, *mapCursor;
extern char pbDrawBgProcName[], pbLoadMapDataProcName[];
extern s8 pbCheckPopRecord(pbProcHandle_t *p);
extern s8 pbCheckPreviousRecord(void);
extern f32 aViewEye[4], aViewTarget[4];

/* Initializer fields in the work allocation; the map starts at +0x1C. */
typedef struct
{
    s32 pcharNum;
    u8 pcharReg[4];
    s16 pcharPos[4][2];
    s16 exitX, exitY;
    pbMapBlock_t map;
} pbMapWorkInitView_t;

s32 moveChar(dds3ProcessID_t *id);
s8 pbCheckPreviousRecord(void);

#include "sdk_libc.h"

extern const char pbGameProcName[16];
extern const char pbMoveCharProcName[24];
extern const char pbMoveBlockProcName[24];

typedef struct { sdfVector4_t v[4]; } pbMatrix_t;

extern s32 pbMapCursorBaseTable[4][4], pbMapCursorUvTable[4][4], pbMapCursorColorTable[4][4];

s8 pbCheckPreviousRecord(void) {
    s8 ret = pbCheckPopRecord(record);
    return ret;
}
