#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdf/sdfMath.c */
#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

#define SDF_PI_2 1.5707963f

f32 sdfSin(f32 aRadius);

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern f32 sdfAsinTable[];

extern s32 (*p2sdSmgBuf)(void);

/* Mirror the phase into a quarter turn, then evaluate an odd ninth-degree polynomial. */
f32 sdfSin(f32 aRadius);

f32 sdfCos(f32 aRadius);

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolation(f32 aValue, const f32 *aTable, s32 aTableSize);

f32 sdfAtan(f32 aX);

f32 sdfAtan2(f32 aY, f32 aX);

/* Restore the input sign after sampling; magnitudes at or above one use half pi. */
f32 sdfAsin(f32 aX);

/* Return the input-signed complement of the sampled angle, not standard acos(x).
 * Magnitudes at or above one leave the angle magnitude at zero.
 */
f32 sdfAcos(f32 aX);

/* Keep the cast-plus/minus-one turn adjustment; this is not general modulo.
 * Inputs already within the pi thresholds are returned unchanged.
 */
f32 sdfClipAngleValuePI(f32 aValue);
/* End of declarations from src/smt3/sdf/sdfMath.c */

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfSin);

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfCos);

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolation(f32 aValue, const f32 *aTable, s32 aTableSize) {
    f32 positionStep = 1 / aTableSize;
    s32 lowerIndex = 0;
    s32 upperIndex = aTableSize;
    s32 sampleIndex;
    f32 lowerValue;
    f32 upperValue;
    do {
        sampleIndex = lowerIndex + upperIndex;
        sampleIndex >>= 1;
        upperValue = aTable[sampleIndex];
        if (aValue < upperValue) {
            upperIndex = sampleIndex;
        } else {
            sampleIndex++;
            lowerIndex = sampleIndex;
        }
    } while (lowerIndex < upperIndex);
    upperValue = aTable[sampleIndex];
    lowerValue = 0.0f;
    if (sampleIndex != 0) {
        lowerValue = aTable[sampleIndex - 1];
    }
    return sampleIndex * positionStep + (aValue - lowerValue) * positionStep / (upperValue - lowerValue);
}

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfAtan);

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfAtan2);

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfAsin);

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfAcos);

INCLUDE_ASM(const s32, "sdf/sdfMath", sdfClipAngleValuePI);
