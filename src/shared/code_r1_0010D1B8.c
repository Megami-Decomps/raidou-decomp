#include "common.h"
#include "sdfChip.h"

#include "event.h"
#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_packet_alloc.h"
#include "sdk_libc.h"

#include "include_asm.h"
#include "ps2asm.h"

/* Values stored at offset zero and dispatched by the interpolation assembly. */
typedef u32 EvtSelStateMode;

#define EVT_SEL_STATE_UNBOUNDED_LIMIT (-1)

/* The selection shake: a per-axis offset that ramps up, holds and ramps down. */
typedef struct evtSelState_t {
    EvtSelStateMode mode;  /* 0x00: current interpolation phase */
    s32 limit;              /* 0x04: deadline frame; unbounded sentinel never ends */
    s32 rampUpFrames;   /* 0x08: frames to ramp up over */
    s32 rampDownFrames; /* 0x0C: frames to ramp down over */
    s16 amplitude[3];  /* 0x10: per-axis peak shake offset */
    u8 pad16[2];
    s32 count;         /* 0x18: frames elapsed */
    f32 offset[3];     /* 0x1C: current per-axis shake limit */
} evtSelState_t;

typedef char evtSelState_size_check[(sizeof(evtSelState_t) == 0x28) ? 1 : -1];

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

extern s8 evtSelectionStateActive;
extern evtSelState_t *evtSelectionState;
extern u8 sdfViewShakeVector[];

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

extern void evtUpdateSelectionStateInterpolation(void);
extern void evtSelStateDestroy(void);

typedef struct dds3ResourceNode_t {
    s32 unk0;
    struct dds3ResourceNode_t *next;
    s32 *ready;
} dds3ResourceNode_t;

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct evtGsCommand_t {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} evtGsCommand_t;

s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);
u32 evtCheckSelectionState(void);
void evtDestroySelectionState(void);
void evtSelStateDestroy(void);

/* Zero frames leaves the prior state untouched; zero countLimit selects the -1 sentinel. */
s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue) {
    evtSelState_t *selectionState;
    if (frames == 0) {
        return 0;
    }
    if (evtSelectionStateActive != 0) {
        evtSelStateDestroy();
    }
    selectionState = sdfChipAllocZeroDirect(EVT_SELECTION_ALLOCATION_BYTES);
    evtSelectionState = selectionState;
    if (countLimit == 0) {
        selectionState->limit = EVT_SEL_STATE_UNBOUNDED_LIMIT;
    } else {
        selectionState->limit = countLimit;
    }
    evtSelectionState->rampUpFrames = firstValue;
    evtSelectionState->rampDownFrames = secondValue;
    evtSelectionState->amplitude[0] = frames;
    evtSelectionState->amplitude[1] = frames;
    evtSelectionState->amplitude[2] = frames;
    evtSelectionStateActive = 1;
    return 1;
}

/* Return zero only when inactive on entry; updating or destroying active state returns one. */
u32 evtCheckSelectionState(void) {
    evtSelState_t *selectionState;
    if (evtSelectionStateActive == 0) {
        return 0;
    }
    selectionState = evtSelectionState;
    if (selectionState->limit > selectionState->count ||
        selectionState->limit == EVT_SEL_STATE_UNBOUNDED_LIMIT) {
        evtUpdateSelectionStateInterpolation();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_0010D1B8", evtUpdateSelectionStateInterpolation);

void evtDestroySelectionState(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (evtSelectionStateActive != 0) {
        sdfChipFreeDirect(evtSelectionState);
        evtSelectionStateActive = 0;
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_STOREVECTOR(sdfViewShakeVector);
    }
}
INCLUDE_SDATA(const s32, "code_r1_0010D1B8", dds3Data40);

INCLUDE_SDATA(const s32, "code_r1_0010D1B8", dds3Data41);

INCLUDE_SDATA(const s32, "code_r1_0010D1B8", dds3Data42);

INCLUDE_SDATA(const s32, "code_r1_0010D1B8", dds3Data43);

INCLUDE_SDATA(const s32, "code_r1_0010D1B8", dds3Data44);

INCLUDE_SDATA(const s32, "code_r1_0010D1B8", dds3Data45);

