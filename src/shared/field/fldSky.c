#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldLightSet_t fldLightSet_t;

struct fldLightSet_t {
    u8 fadeArea;
    u8 pad1[3];
    s32 displayStateSlot14;
    s32 fadeMode;
    s32 swayMode;
    u8 pad10[0xC];
    s32 fogNearValue;
    s32 depthNearValue;
    s32 fogFarValue;
    s32 depthFarValue;
    s32 drawColorFixedR;
    s32 drawColorFixedG;
    s32 drawColorFixedB;
    f32 lightColorAX;
    f32 lightColorAY;
    f32 lightColorAZ;
    f32 lightDirectionAX;
    f32 lightDirectionAY;
    f32 lightDirectionAZ;
    f32 lightColorBX;
    f32 lightColorBY;
    f32 lightColorBZ;
    f32 lightDirectionBX;
    f32 lightDirectionBY;
    f32 lightDirectionBZ;
    f32 lightColorCX;
    f32 lightColorCY;
    f32 lightColorCZ;
    f32 lightDirectionCX;
    f32 lightDirectionCY;
    f32 lightDirectionCZ;
    f32 backgroundColorX;
    f32 backgroundColorY;
    f32 backgroundColorZ;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
};

enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

typedef struct devState_t devState_t;

extern u32 fldSkyDisplayPointers[];
extern u32 fldCallMemset4Data[];
extern f32 fldSkyPlayerIdx[];

/* Filter records: a mode followed by the 8-by-9 corner-color grid (292 bytes). */
typedef struct {
    s32 mode;
    s32 colors[8][9];
} fldSkyData_t;

#include "sdk_libc.h"

void fldCallMemset4(void);

void fldCallMemset4(void) {
    u32 *displayPointers = fldSkyDisplayPointers;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = EE_ADDR(fldCallMemset4Data);
    displayPointers[1] = EE_ADDR(fldSkyPlayerIdx);
}
