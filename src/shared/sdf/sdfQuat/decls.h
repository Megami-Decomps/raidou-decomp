/* Declarations of smt3-decomp src/smt3/sdf/sdfQuat.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFQUAT_H
#define SHARED_SDF_SDFQUAT_H

#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

f32 sdfSin(f32 aRadius);

f32 sdfClipAngleValuePI(f32 aValue);

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

typedef union { f32 quaternion[4]; u16 monthDays[20]; } sdfMathConstants_t;

extern sdfMathConstants_t sdfMathConstants;

extern f32 sdfQuatPoleDown[4];

extern f32 sdfQuatPoleUp[4];

extern f32 sdfUnitXVector[4];

extern s32 (*p2sdSmgBuf)(void);

/* Return the input-signed complement of the sampled angle, not standard acos(x). * Magnitudes at or above one leave the angle magnitude at zero. */ f32 sdfAcos(f32 x);

f32 sdfAtan2(f32 aY, f32 aX);

f32 sdfCos(f32 aRadius);

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Mult(void);

/* vu0 routine: the conjugate over the squared norm. */
void Inverse(void);

/* vu0 routine: normalize all four components of vf10. The IDB called this
 * SDF_QUAT_INNERPRODUCT, which belongs to the dot product below. */
void Normalize(void);

/* vu0 routine: retail reduces (x*x)*(y*y) + z*z + w*w, not squared length. */
f32 sdf_2C0510(void);

/* vu0 routine: return the four-lane dot product of vf10 and vf11. */
f32 SDF_QUAT_INNERPRODUCT(void);

/* vf10 initially holds the axis; write the sine-scaled axis and cosine W. */
void FromRotation(f32 aAngle);

/* As above, but write the second VU quaternion register (vf11). */
void FromRotation_B(f32 aAngle);

/* vu0 routine: shared quaternion registers are the input/output interface. */
void ToMatrix(void);

/* vu0 routine: the composed quaternion is left in the shared register. */
void FromEulerXYZ(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

/* vu0 routine: the composed quaternion is left in the shared register. */
void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

/* vu0 routine: compose pitch and offset-adjusted planar direction quaternions. */
void LookAt(f32 aFrontX, f32 aFrontZ);

/* vu0 routine: blend vf10/vf11 along the shorter quaternion arc into vf10.
 * blendAmount is used without clamping; the final step normalizes either blend. */
void Slerp(f32 aRatio);

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Lerp(f32 aRatio);

/* vu0 routine: shared quaternion registers are the input/output interface. */
f32 EulerAngleAxisY(void);

#endif
