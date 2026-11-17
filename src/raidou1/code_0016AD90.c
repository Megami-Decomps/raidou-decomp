#include "common.h"
#include "fldProcess.h"
#include "mapData.h"
#include "code_0016A7D8.h"
#include "sdfMath.h"
#include "dds3KernelDraw.h"
#include "code_0016AD90.h"
#include "fld.h"
#include "itf.h"
#include "sdfVu0.h"
#include "ps2asm.h"

INCLUDE_ASM(const s32, "code_0016AD90", fldLeaf25);

/* NON_MATCHING: 74 of 94 words match.  */
#ifdef NON_MATCHING
void mapFunc(void) {
    s32 down;
    s32 up;
    s32 i;
    s32 dx;
    s32 dz;

    if (pFldMap == NULL) {
        return;
    }
    down = -1;
    up = -1;
    draw_flags &= ~1;
    draw_flags &= ~2;
    for (i = 1; i < 44; i++) {
        if (!(draw_flags & 1)) {
            down = fldCallBitCheck(-i);
            if (down >= 0 && mapAutoMapUpdateLimit(down) > 0) {
                draw_flags |= 1;
            }
        }
        if (!(draw_flags & 2)) {
            up = fldCallBitCheck(i);
            if (up >= 0 && mapAutoMapUpdateLimit(up) > 0) {
                draw_flags |= 2;
            }
        }
    }
    if (dds3PadButtonState[0x28] < 0 && (draw_flags & 1)) {
        am_floor = down;
        fldLeaf25();
    }
    if (dds3PadButtonState[0x2A] < 0 && (draw_flags & 2)) {
        am_floor = up;
        fldLeaf25();
    }
    dx = 0;
    dz = 0;
    if (dds3PadButtonState[0x24]) {
        dx = -1;
    }
    if (dds3PadButtonState[0x26]) {
        dz = -1;
    }
    if (dds3PadButtonState[0x27]) {
        dz = 1;
    }
    if (dds3PadButtonState[0x25]) {
        dx = 1;
    }
    mapAutoMapCalcPosLimit(dx, dz);
}
#else
INCLUDE_ASM(const s32, "code_0016AD90", mapFunc);
#endif

/* NON_MATCHING: 18 of 62 words match.  */
#ifdef NON_MATCHING
void mapCallSin11(s32 x, s32 y) {
    f32 px = x;
    f32 py;

    mapCallSin11Data += 0.09999999404f;
    py = (f32)y - 8.0f + sdfSin(mapCallSin11Data);
    eglSetOt(0x67);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglCallDLinkTexPacket(px - 8.0f, py, 16.0f, 16.0f, 0x6C, 0xD, 0x10, 0x10, 0x80808080, 0x80808080, 0x80808080, 0x80808080, effQueuedResourceNameRecord4);
}
#else
INCLUDE_ASM(const s32, "code_0016AD90", mapCallSin11);
#endif

INCLUDE_ASM(const s32, "code_0016AD90", mapCallSin12);

void mapCallCutGetCam(s32 x0, s32 y0, s32 x1, s32 y1) {
    sdfVector4_t a;
    sdfVector4_t b;
    sdfVector4_t dir;
    u32 col[3];
    f32 angle;
    f32 vx;
    f32 vy;
    f32 fx0;
    f32 fy0;
    f32 fx1;
    f32 fy1;

    fx0 = x0;
    fy0 = y0;
    fx1 = x1;
    fy1 = y1;
    if (mapCallCutGetCamData + 0.1f > 6.28f) {
        mapCallCutGetCamData = 0.0f;
    } else {
        mapCallCutGetCamData += 0.1f;
    }
    mapCutGetCam(&a, &b, &angle);
    a.x = fx0;
    b.x = fx1;
    b.z = fy1;
    a.y = 0.0f;
    a.z = fy0;
    angle *= 0.75f;
    a.w = 0.0f;
    b.y = 0.0f;
    b.w = 0.0f;
    SDF_VU0_LOADVECTOR(&b);
    SDF_VU0_LOADVECTOR_B(&a);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_BITS(224.0f);
    SDF_VU0_STOREVECTOR(&dir);
    fx1 = fx0 + dir.x;
    fy1 = fy0 + dir.z;
    SDF_VU0_LOADVECTOR_B(&a);
    SDF_VU0_LOADIDENTITY();
    RotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&a);
    SDF_VU0_LOADIDENTITY();
    RotateMatrixY(-angle);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&b);
    itfGetVectorPosBezierPt3(&vx, &vy, a.x, a.z, b.x, b.z, fx1, fy1, 0.5f);
    col[0] = 0x235A8296;
    col[1] = 0x0A5A8296;
    col[2] = 0x0A5A8296;
    itfDrawFanBezierPt3(fx0, fy0, a.x, a.z, b.x, b.z, vx, vy, col[0], col[1], col[2], 102);
}
