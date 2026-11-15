#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel.h"
#include "scr.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "dds/dds3_task_state_query.h"
#include "dds/itf.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

/* Return the scheduler state, mapping unregistered or invalid states to DETACHED. */
dds3TaskState_t dds3GetProcessStat(const struct dds3ProcessID_t *task);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#define ITF_BYTE_MASK 0xFF

#define ITF_NODE_HEADER_BYTES 8

#define ITF_ALLOCATION_HANDLE_BYTES 4

#define ITF_VECTOR_WORD_COUNT 4

#define ITF_RGBA_COMPONENT_COUNT 4

#define ITF_FIXED_FRACTION_BITS 16

#define ITF_FIXED_COMPONENT_MAX 0xff0000

#define ITF_NEUTRAL_COLOR 0x80808080

#define ITF_GS_X_BIAS 0x7000

#define ITF_GS_Y_BIAS 0x7900

#define ITF_TRIANGLE_VERTEX_COUNT 3

#define ITF_QUAD_VERTEX_COUNT 4

#define ITF_STATE_PACKET_BYTES 0x30

#define ITF_TEST_FIRST_CONTEXT 0x47

#define ITF_TEST_SECOND_CONTEXT 0x48

#define ITF_ALPHA_FIRST_CONTEXT 0x42

#define ITF_ALPHA_SECOND_CONTEXT 0x43

#define ITF_TRIANGLE_PRIMITIVE_BITS 0x4B

#define ITF_QUAD_FAN_PRIMITIVE_BITS 0x4D

#define ITF_TEXTURED_FAN_PRIMITIVE_BITS 0x5D

#define ITF_TEXTURED_SPRITE_PRIMITIVE_BITS 0x156

#define ITF_COLORED_SPRITE_PRIMITIVE_BITS 0x15E

#define ITF_SPRITE_PRIMITIVE_BITS 0x46

#define ITF_TRIANGLE_STRIP_PRIMITIVE_BITS 0x4C

#define ITF_LINE_STRIP_PRIMITIVE_BITS 0x14A

#define ITF_LINES_PRIMITIVE_BITS 0x49

extern dds3ProcessID_t *itfFontTestScriptTask;
extern sdfTexHandle_t *itfBackgroundSpriteTexture;
void itfReleaseBackgroundSpriteTexture(void);

struct EffSpriteUV;

struct sdfTexHandle_t;

union EffSpriteColor;

/* Draw only when a texture is present; native X/Y extent scaling differs. */
void itfDrawBackgroundSprite(void);
extern s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task);
s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task);
u32 itfCamp(void);

/* Drop the held texture reference without clearing the global. */
void itfReleaseBackgroundSpriteTexture(void) {
    sdfTexReleaseReference(itfBackgroundSpriteTexture);
}

INCLUDE_ASM(const s32, "code_00198B50", mjnFunc);

INCLUDE_ASM(const s32, "code_00198B50", M_InitScene);

INCLUDE_ASM(const s32, "code_00198B50", mjnDeleteScene);

INCLUDE_ASM(const s32, "code_00198B50", mjnCallGetProcessStat);

/* Draw first, then report all bits set only for a task awaiting destruction. */
s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task) {
    dds3TaskState_t taskState;
    u32 readyMask;

    itfDrawBackgroundSprite();
    taskState = dds3GetProcessStat(itfFontTestScriptTask);
    readyMask = 0xffffffff;
    if (taskState != DDS3_TASK_DESTROY_PENDING) {
        readyMask = 0;
    }
    return readyMask;
}
INCLUDE_SDATA(const s32, "code_00198B50", sdfFileSeekWork2);

INCLUDE_SDATA(const s32, "code_00198B50", itfFontTestScriptTask);

