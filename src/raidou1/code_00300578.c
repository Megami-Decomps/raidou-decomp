#include "common.h"
#include "include_asm.h"
#include "kernel.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfMemory.h"
#include "sdf_resource.h"
#include "dds3KernelCore.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "fldProcess.h"
#include "dds3PadManager.h"
#include "sdfSound.h"
#include "cmpMain.h"
#include "cmpFile.h"
#include "itf.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_002C8188.h"
#include "code_r1_002CAFD8.h"
#include "code_00300578.h"
#include "cmpModel.h"
#include "sdfTex.h"
#include "effManager.h"
#include "fclMisc.h"
#include "kernel_draw.h"
#include "dds3CameraObjectBasic.h"
#include "sdfVu0.h"
#include "ps2asm.h"
#include "dds3KernelMain.h"
#include "sdk_libc.h"
#include "sdfPacketBuff.h"
#include "mdlManager.h"
#include "code_00323280.h"

INCLUDE_ASM(const s32, "code_00300578", titleFunc4);

INCLUDE_ASM(const s32, "code_00300578", titleFunc5);

INCLUDE_ASM(const s32, "code_00300578", titleFunc6);

INCLUDE_ASM(const s32, "code_00300578", titleSeffViewDraw);

INCLUDE_ASM(const s32, "code_00300578", titleCallMemGetPtr);

INCLUDE_ASM(const s32, "code_00300578", titleCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_00300578", titleSeffViewProcess);

INCLUDE_ASM(const s32, "code_00300578", titleTestRelease);

INCLUDE_ASM(const s32, "code_00300578", titleTestDeleteWorld);

INCLUDE_RODATA(const s32, "code_00300578", titleSeffViewAreaTable);

INCLUDE_RODATA(const s32, "code_00300578", titleSeffViewAreaTable2);

INCLUDE_RODATA(const s32, "code_00300578", titleSeffViewAreaTable3);

INCLUDE_RODATA(const s32, "code_00300578", titleSeffViewAreaTable4);

INCLUDE_RODATA(const s32, "code_00300578", titleSeffViewAreaTable5);

INCLUDE_ASM(const s32, "code_00300578", titleSeffViewArea);

INCLUDE_ASM(const s32, "code_00300578", titleFunc7);

INCLUDE_ASM(const s32, "code_00300578", titleLeaf);

INCLUDE_ASM(const s32, "code_00300578", titleCallMatNew);

INCLUDE_ASM(const s32, "code_00300578", titleCallMatFreeDelayed);

INCLUDE_ASM(const s32, "code_00300578", titleCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_00300578", titleCallSetOt);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallInversMatrix);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallInversMatrix2);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallInversMatrix3);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallInversMatrix4);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallMemset22);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallMemset23);

INCLUDE_ASM(const s32, "code_00300578", sbtlSetDevilID_EnemyRot);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallMemset24);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallLinkTest);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallFptodp2);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallInversMatrixTable);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallInversMatrix2Table);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallInversMatrix3Table);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallInversMatrix4Table);

INCLUDE_RODATA(const s32, "code_00300578", sbtlStr72727272);

INCLUDE_ASM(const s32, "code_00300578", sbtlSeffActCam);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallBtlSetDevilIDEnemyRot);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallMemset25);

INCLUDE_ASM(const s32, "code_00300578", sbtlLeaf84);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallDrawTagLink);

INCLUDE_ASM(const s32, "code_00300578", sbtlLeaf85);

INCLUDE_ASM(const s32, "code_00300578", sbtlLeaf86);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallGetRandA);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallGrAllocPacketBuff4);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallDebugDrawWireModel);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallGrAllocPacketBuff5);

INCLUDE_ASM(const s32, "code_00300578", sbtlCallMemset26);

INCLUDE_ASM(const s32, "code_00300578", sbtlFunc14);

INCLUDE_ASM(const s32, "code_00300578", nbFunc33);

INCLUDE_ASM(const s32, "code_00300578", nbFunc34);

INCLUDE_ASM(const s32, "code_00300578", nbFunc35);

INCLUDE_ASM(const s32, "code_00300578", nbFunc36);

INCLUDE_ASM(const s32, "code_00300578", nbFunc37);

INCLUDE_ASM(const s32, "code_00300578", nbFunc38);

INCLUDE_ASM(const s32, "code_00300578", nbFunc39);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf64);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf65);

INCLUDE_ASM(const s32, "code_00300578", nbRefCameraTM);

INCLUDE_ASM(const s32, "code_00300578", nbCallMemset3);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallDebugDrawWireModelTable);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallGrAllocPacketBuff5Table);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallMemset26Table);

INCLUDE_RODATA(const s32, "code_00300578", sbtlCallMemset26Table2);

INCLUDE_RODATA(const s32, "code_00300578", nbCallMemset3Table);

INCLUDE_ASM(const s32, "code_00300578", nbFunc40);

INCLUDE_ASM(const s32, "code_00300578", nbFunc41);

INCLUDE_ASM(const s32, "code_00300578", nbFunc42);

INCLUDE_ASM(const s32, "code_00300578", nbFunc43);

INCLUDE_ASM(const s32, "code_00300578", nbCallMultMatrixRevNoArg);

INCLUDE_ASM(const s32, "code_00300578", nbCallIncResultCounter);

void nbIncResultCounter(void)
{
    nbData8++;
}

INCLUDE_ASM(const s32, "code_00300578", nbLeaf66);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf67);

INCLUDE_ASM(const s32, "code_00300578", nbCallLinkTest);

INCLUDE_ASM(const s32, "code_00300578", nbCallMemcpy);

INCLUDE_ASM(const s32, "code_00300578", nbFunc44);

INCLUDE_ASM(const s32, "code_00300578", nbFunc45);

INCLUDE_ASM(const s32, "code_00300578", nbFunc46);

INCLUDE_ASM(const s32, "code_00300578", nbFunc47);

INCLUDE_ASM(const s32, "code_00300578", nbCallMultMatrixRev);

INCLUDE_ASM(const s32, "code_00300578", nbCallGetRandFloatA);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf68);

INCLUDE_ASM(const s32, "code_00300578", nbCallToMatrix);

INCLUDE_ASM(const s32, "code_00300578", nbCallGetRandFloatA2);

INCLUDE_ASM(const s32, "code_00300578", nbCallGetRandFloatA3);

INCLUDE_ASM(const s32, "code_00300578", nbCallToMatrix2);

INCLUDE_ASM(const s32, "code_00300578", nbCallToMatrix3);

void nbLeaf69(s32 a, s32 b) {
    nbLeaf71(b, a);
}

INCLUDE_ASM(const s32, "code_00300578", nbFunc48);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf70);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf71);

INCLUDE_ASM(const s32, "code_00300578", nbCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf72);

INCLUDE_ASM(const s32, "code_00300578", nbFileOpen);

INCLUDE_ASM(const s32, "code_00300578", nbCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_00300578", nbCallChipAllocDirect2);

INCLUDE_ASM(const s32, "code_00300578", nbCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_00300578", nbCallTexFreeDelayed2);

INCLUDE_ASM(const s32, "code_00300578", nbFunc49);

INCLUDE_ASM(const s32, "code_00300578", nbFunc50);

INCLUDE_ASM(const s32, "code_00300578", nbFunc51);

INCLUDE_ASM(const s32, "code_00300578", nbCallTexFreeDelayed3);

INCLUDE_ASM(const s32, "code_00300578", nbCallMemFreeDirect3);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf73);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf74);

INCLUDE_ASM(const s32, "code_00300578", nbSeffresDumpResource);

INCLUDE_ASM(const s32, "code_00300578", nbCallLinkTest2);

INCLUDE_ASM(const s32, "code_00300578", nbCallMemGetPtr);

INCLUDE_ASM(const s32, "code_00300578", nbCallTexFreeDelayed4);

INCLUDE_ASM(const s32, "code_00300578", nbLeaf75);

INCLUDE_ASM(const s32, "code_00300578", sysCallMemGetPtr);

INCLUDE_ASM(const s32, "code_00300578", sysFunc29);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf47);

INCLUDE_ASM(const s32, "code_00300578", sysCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_00300578", sysFunc30);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf48);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf49);

INCLUDE_ASM(const s32, "code_00300578", sysFunc31);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf50);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf51);

INCLUDE_ASM(const s32, "code_00300578", sysCallChipAllocDirect);

INCLUDE_ASM(const s32, "code_00300578", sysFunc32);

INCLUDE_ASM(const s32, "code_00300578", sysCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_00300578", sysCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_00300578", sysCallBillBoardMakeN);

INCLUDE_ASM(const s32, "code_00300578", sysCallCos);

INCLUDE_ASM(const s32, "code_00300578", sysCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf52);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf53);

INCLUDE_ASM(const s32, "code_00300578", sysLeaf54);

INCLUDE_ASM(const s32, "code_00300578", sysCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_00300578", sysBILLNUM);

INCLUDE_ASM(const s32, "code_00300578", sysCallTexAlloc);

void sysLeaf55(sdfTexHandle_t *texture) {
    sdfTexReleaseReference(texture);
}

INCLUDE_ASM(const s32, "code_00300578", sysLeaf56);

INCLUDE_ASM(const s32, "code_00300578", sysCallImageDataSize);

INCLUDE_ASM(const s32, "code_00300578", sysSeffEditDraw);

INCLUDE_ASM(const s32, "code_00300578", sysCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_00300578", sysTYPETEXTURE);

INCLUDE_ASM(const s32, "code_00300578", frCallMemGetPtr);

INCLUDE_ASM(const s32, "code_00300578", frSeffEditProcess);

INCLUDE_ASM(const s32, "code_00300578", frFunc);

INCLUDE_ASM(const s32, "code_00300578", frCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_00300578", frFunc2);

INCLUDE_ASM(const s32, "code_00300578", frCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_00300578", frLeaf9);

INCLUDE_ASM(const s32, "code_00300578", frCallMemGetPtr4);

INCLUDE_ASM(const s32, "code_00300578", frCallMemGetPtr5);

INCLUDE_ASM(const s32, "code_00300578", frCallMemGetPtr6);

void frLeaf10(s32 arg) {
    sysLeaf50(arg);
}

INCLUDE_ASM(const s32, "code_00300578", frCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_00300578", frLeaf11);

INCLUDE_ASM(const s32, "code_00300578", frFunc3);

INCLUDE_ASM(const s32, "code_00300578", frFunc4);

INCLUDE_ASM(const s32, "code_00300578", frFunc5);

INCLUDE_ASM(const s32, "code_00300578", frFunc6);

INCLUDE_ASM(const s32, "code_00300578", frLeaf12);

INCLUDE_ASM(const s32, "code_00300578", frFunc7);

INCLUDE_ASM(const s32, "code_00300578", frLeaf13);

INCLUDE_ASM(const s32, "code_00300578", frLeaf14);

INCLUDE_ASM(const s32, "code_00300578", frFunc8);

INCLUDE_ASM(const s32, "code_00300578", frCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_00300578", frLeaf15);

INCLUDE_ASM(const s32, "code_00300578", frLeaf16);

INCLUDE_ASM(const s32, "code_00300578", frFunc9);

INCLUDE_ASM(const s32, "code_00300578", frMistTmx);

INCLUDE_ASM(const s32, "code_00300578", frCallMatFreeDelayed);

INCLUDE_ASM(const s32, "code_00300578", frLeaf17);

INCLUDE_ASM(const s32, "code_00300578", frCallMemset5);

INCLUDE_ASM(const s32, "code_00300578", frCallMemset6);

INCLUDE_ASM(const s32, "code_00300578", frCallLoad);

INCLUDE_ASM(const s32, "code_00300578", frCallSetPosition);

INCLUDE_ASM(const s32, "code_00300578", frCallLink);

INCLUDE_RODATA(const s32, "code_00300578", cmpDefaultLightColor);

INCLUDE_RODATA(const s32, "code_00300578", frCallMemset5Table);

INCLUDE_RODATA(const s32, "code_00300578", frStrLLL);

INCLUDE_RODATA(const s32, "code_00300578", frCallMemset5Table2);

INCLUDE_RODATA(const s32, "code_00300578", frCallMemset5Table3);

INCLUDE_ASM(const s32, "code_00300578", frWater);

INCLUDE_ASM(const s32, "code_00300578", frCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_00300578", frFunc10);

INCLUDE_ASM(const s32, "code_00300578", frSize);

INCLUDE_ASM(const s32, "code_00300578", frFunc11);

INCLUDE_ASM(const s32, "code_00300578", frTestFontInit);

INCLUDE_RODATA(const s32, "code_00300578", frStrWaterTestProcess);

INCLUDE_ASM(const s32, "code_00300578", frEndingWaterTest);

INCLUDE_ASM(const s32, "code_00300578", frCallMemAllocDirect3);

INCLUDE_ASM(const s32, "code_00300578", frCallDataRelease);

INCLUDE_ASM(const s32, "code_00300578", frCallRotateMatrixY);

INCLUDE_ASM(const s32, "code_00300578", pipeCallRotateMatrixY);

INCLUDE_ASM(const s32, "code_00300578", pipeWater);

INCLUDE_ASM(const s32, "code_00300578", pipeWater2);

INCLUDE_ASM(const s32, "code_00300578", pipeWater3);

INCLUDE_ASM(const s32, "code_00300578", pipeDataRelease);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf2);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf3);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf4);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf5);

INCLUDE_ASM(const s32, "code_00300578", pipeFunc);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf6);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf7);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf8);

INCLUDE_ASM(const s32, "code_00300578", pipeFunc2);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf9);

INCLUDE_ASM(const s32, "code_00300578", pipeFunc3);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf10);

INCLUDE_ASM(const s32, "code_00300578", pipeLeaf11);

INCLUDE_ASM(const s32, "code_00300578", pipeCallNormalizeVec);

INCLUDE_ASM(const s32, "code_00300578", pipeCallGetRotate);

INCLUDE_ASM(const s32, "code_00300578", jmsFunc);

INCLUDE_ASM(const s32, "code_00300578", jmsFunc2);

INCLUDE_ASM(const s32, "code_00300578", jmsCallGetPosition);

INCLUDE_ASM(const s32, "code_00300578", jmsLeaf);

INCLUDE_ASM(const s32, "code_00300578", jmsCallGetRotate);

INCLUDE_ASM(const s32, "code_00300578", jmsFunc3);

INCLUDE_ASM(const s32, "code_00300578", jmsLeaf2);

INCLUDE_ASM(const s32, "code_00300578", jmsLeaf3);

INCLUDE_ASM(const s32, "code_00300578", jmsCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_00300578", jmsCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_00300578", jmsCallGrAllocPacketBuff3);

INCLUDE_RODATA(const s32, "code_00300578", pipeStrGs);

INCLUDE_RODATA(const s32, "code_00300578", pipeStrGs2);

INCLUDE_RODATA(const s32, "code_00300578", pipeStr);

INCLUDE_RODATA(const s32, "code_00300578", pipeStr2);

INCLUDE_RODATA(const s32, "code_00300578", pipeStrUv);

INCLUDE_RODATA(const s32, "code_00300578", pipeStr3);

INCLUDE_ASM(const s32, "code_00300578", jmsCallFptodp);

INCLUDE_ASM(const s32, "code_00300578", jmsCallSetOt);

INCLUDE_ASM(const s32, "code_00300578", jmsWaterReflectTmx);

INCLUDE_ASM(const s32, "code_00300578", jmsModelMakeLightPacket);

INCLUDE_ASM(const s32, "code_00300578", jmsLeaf4);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf);

f32 *sdfVec4Add(f32 *dst, f32 *lhs, f32 *rhs) {
    dst[0] = lhs[0] + rhs[0];
    dst[1] = lhs[1] + rhs[1];
    dst[2] = lhs[2] + rhs[2];
    return dst;
}

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf2);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf3);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf4);

INCLUDE_ASM(const s32, "code_00300578", sdfFunc);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf5);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf6);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf7);

INCLUDE_ASM(const s32, "code_00300578", sdfCallCosf);

INCLUDE_ASM(const s32, "code_00300578", sdfLeaf8);

INCLUDE_ASM(const s32, "code_00300578", sdfCallFptodp);

INCLUDE_ASM(const s32, "code_00300578", sdfCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_00300578", sdfCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_00300578", sdfCallDGetPrimPacketSize);

INCLUDE_ASM(const s32, "code_00300578", sdfCallDGetPrimPacketSize2);

INCLUDE_ASM(const s32, "code_00300578", sdfCallMultMatrixRev);

INCLUDE_ASM(const s32, "code_00300578", sdfWater);

INCLUDE_ASM(const s32, "code_00300578", sdfFunc2);

INCLUDE_ASM(const s32, "code_00300578", sdfCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "code_00300578", sdfCallPrintf);

INCLUDE_ASM(const s32, "code_00300578", sdfFunc4);

INCLUDE_ASM(const s32, "code_00300578", evtCallGetRandFloatA);

INCLUDE_ASM(const s32, "code_00300578", evtLeaf);

INCLUDE_ASM(const s32, "code_00300578", evtFunc12);

INCLUDE_ASM(const s32, "code_00300578", evtCallSetOt);

INCLUDE_ASM(const s32, "code_00300578", evtFunc13);

INCLUDE_ASM(const s32, "code_00300578", evtCallGetRandFloatA2);

INCLUDE_ASM(const s32, "code_00300578", evtLeaf2);

INCLUDE_ASM(const s32, "code_00300578", evtFunc14);

INCLUDE_ASM(const s32, "code_00300578", evtCallGetRandFloatA3);

INCLUDE_ASM(const s32, "code_00300578", evtCallSetOt2);

INCLUDE_ASM(const s32, "code_00300578", evtFunc15);

INCLUDE_ASM(const s32, "code_00300578", evtCallGrAllocPacketBuff3);

/* body: matched in raidou @ 61f09dc: src/shared/effect/effPacket.c:180, rework, same function 1.00 (words 252/252, calls 1.00, shape 1.00) */
void *effPkt3DPrimMakePoly2(sdf3DPrimPolyParm_t *aParm) {
    s32 parameterCount = aParm->faceCount;
    s32 vertexCount = aParm->pointCount;
    u32 primitive = aParm->attr | 0x40;
    u32 mode = 0x60;
    s32 stride = 3;
    s32 overhead = 16;
    s32 allocationSize;
    s32 i;
    s32 offset;
    void *packet;
    void *(*allocate)(s32);
    u32 *cursor;
    u32 *stream;
    const u128 *vector;
    u32 countCode;
    u32 *stream2;

    if (aParm->normal != NULL) {
        mode = 0xE0;
        overhead = 17;
        stride = 6;
    }
    if (aParm->uv != NULL) {
        mode |= 0x100;
        primitive |= 0x10;
        overhead++;
        stride += 2;
        if (aParm->multiUv != NULL) {
            primitive |= 0x1000;
            stride += 2;
        }
    }
    if (aParm->vertexColor != NULL) {
        mode |= 0x200;
        primitive |= 0x800;
        overhead++;
        stride++;
    }
    allocationSize = (overhead + parameterCount + stride * vertexCount) * 4;
    allocationSize = (allocationSize + 15) & ~15;
    allocate = (void *(*)(s32))aParm->allocator;
    if (allocate == NULL) {
        allocate = (void *(*)(s32))sdfGrAllocPacketBuff;
    }
    packet = allocate(allocationSize);

    *(u64 *)packet = (u64)(u16)((allocationSize >> 4) - 1) | 0x20000000ULL;
    ((u32 *)packet)[2] = 0x6102C000;
    ((u32 *)packet)[3] = (~aParm->attrInhibit) & 0xFFFF;
    ((u32 *)packet)[4] = 0x6E01C002;
    ((u32 *)packet)[5] = aParm->color;
    ((u32 *)packet)[6] = 0x6001C003;
    cursor = (u32 *)packet + 7;
    *cursor++ = (u32)aParm->zBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)parameterCount | ((u32)(u16)vertexCount << 16);
    *cursor++ = (primitive & 0xFFFF) | (mode << 16);
    *cursor++ = 0x6E00C001 | ((u32)parameterCount << 16);

    if (aParm->faceIndex == NULL) {
        memcpy(cursor, effPktDefaultFaceIndex2, parameterCount * 4);
    } else {
        memcpy(cursor, aParm->faceIndex, parameterCount * 4);
    }
    cursor += parameterCount;
    offset = parameterCount + 1;
    countCode = vertexCount << 16;
    *cursor++ = countCode | offset | 0x6800C000;
    vector = (u128 *)aParm->vertex;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, *vector);
        i++;
        cursor += 3;
        vector++;
    } while (i != vertexCount);
    offset += vertexCount;

    vector = (u128 *)aParm->normal;
    if (vector != NULL) {
        *cursor++ = countCode | offset | 0x6800C000;
        i = 0;
        do {
            MMI_STORE_VEC3_VALUE(cursor, *vector);
            i++;
            cursor += 3;
            vector++;
        } while (i != vertexCount);
        offset += vertexCount;
    }
    stream = (u32 *)aParm->uv;
    if (stream != NULL) {
        stream2 = (u32 *)aParm->multiUv;
        if (stream2 == NULL) {
            *cursor++ = countCode | offset | 0x6400C000;
            memcpy(cursor, stream, vertexCount * 8);
            cursor += vertexCount * 2;
        } else {
            *cursor++ = countCode | offset | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *stream++;
                *cursor++ = *stream++;
                *cursor++ = *stream2++;
                *cursor++ = *stream2++;
                i++;
            } while (i != vertexCount);
        }
        offset += vertexCount;
    }
    stream = aParm->vertexColor;
    if (stream != NULL) {
        *cursor++ = countCode | offset | 0x6E00C000;
        memcpy(cursor, stream, vertexCount * 4);
        cursor += vertexCount;
    }
    *cursor = 0x1400000C;
    while (EE_ADDR(++cursor) & 15) {
        *cursor = 0;
    }
    return packet;
}

INCLUDE_ASM(const s32, "code_00300578", evtFunc16);

INCLUDE_ASM(const s32, "code_00300578", evtFunc17);

s32 evtCreateLinkSub(CMP_SPR a_no) {
    s32 mot;

    if (a_no >= 4) {
        return 0;
    }
    mot = evtData9[a_no];
    if (mot < 0) {
        return 0;
    }
    return cmpLinkMotFunc[mot]();
}

INCLUDE_ASM(const s32, "code_00300578", evtFunc18);

s32 evtFunc19(CMP_SPR a_no) {
    s32 mot;

    if (a_no >= 4) {
        return 0;
    }
    mot = evtData9[a_no];
    if (mot < 0) {
        return 0;
    }
    return evtFunc18(mot);
}

INCLUDE_ASM(const s32, "code_00300578", evtCallGetSpriteHnd);

INCLUDE_ASM(const s32, "code_00300578", evtFunc20);

INCLUDE_ASM(const s32, "code_00300578", evtStartTerminalMenuMessage);

INCLUDE_ASM(const s32, "code_00300578", cmpResetMotion);

INCLUDE_ASM(const s32, "code_00300578", cmpFreeSpriteMem);

INCLUDE_ASM(const s32, "code_00300578", cmpLoadSpriteMem);

s32 cmpFreeSpriteHnd(CMP_SPR a_no) {
    struct sprHandle_t **set = &evtWork6[a_no];

    if (a_no >= 4) {
        return 0;
    }
    if (*set == NULL) {
        return 0;
    }
    effDestroyResourceSlotSet(*set);
    *set = NULL;
    evtFunc19(a_no);
    return 1;
}

s32 pipeCreateLink(CMP_SPR a_no) {
    if (a_no >= 4) {
        return 0;
    }
    cmpFreeSpriteHnd(a_no);
    evtWork6[a_no] = effCreateResourceSlotSetFromAllocation(cmpWork3[a_no], 0);
    evtCreateLinkSub(a_no);
    return 1;
}

/* NON_MATCHING: 83 of 86 words match. Only the order of the two loop preheader addiu (evtWork6, cmpWork3 bases) differs. */
#ifdef NON_MATCHING
s32 pipeCampCAMPPACKLB(void) {
    s32 i = 0;
    s32 ret = 1;
    fileRequest_t *pac = pacLoad(pipeStrCampCamppackLb);
    sdfPacDecodeResrc_t *resrc;

    pacSync(pac);
    for (resrc = pac->pac.resrcHead; resrc != NULL; resrc = resrc->next, i++) {
        if ((u32)i >= 4) {
            ret = 0;
            sdfMemFreeDirect(resrc->resrc);
        } else {
            switch (i) {
            case 0:
                cmpWork3[0] = resrc->resrc;
                evtWork6[0] = NULL;
                break;
            case 1:
                cmpWork3[1] = resrc->resrc;
                evtWork6[1] = NULL;
                break;
            case 2:
                cmpWork3[2] = resrc->resrc;
                evtWork6[2] = NULL;
                break;
            case 3:
                cmpWork3[3] = resrc->resrc;
                evtWork6[3] = NULL;
                break;
            default:
                sdfMemFreeDirect(resrc->resrc);
                break;
            }
        }
    }
    pacFileHandleFree_(pac);
    return ret;
}
#else
INCLUDE_RODATA(const s32, "code_00300578", sdfCallMultMatrixRevTable);

INCLUDE_RODATA(const s32, "code_00300578", sdfStr3);

INCLUDE_RODATA(const s32, "code_00300578", sdfStrGs);

INCLUDE_RODATA(const s32, "code_00300578", sdfStrGs2);

INCLUDE_RODATA(const s32, "code_00300578", sdfStr4);

INCLUDE_RODATA(const s32, "code_00300578", sdfStr5);

INCLUDE_RODATA(const s32, "code_00300578", fldStrCampTizuSpr);

INCLUDE_RODATA(const s32, "code_00300578", fldStrCampJoutaiSpr);

INCLUDE_RODATA(const s32, "code_00300578", fldStrCampTdnSpr);

INCLUDE_RODATA(const s32, "code_00300578", fldStrCampSubSpr);

INCLUDE_ASM(const s32, "code_00300578", pipeCampCAMPPACKLB);
#endif

INCLUDE_ASM(const s32, "code_00300578", cmpFreeSpriteToMem);

INCLUDE_ASM(const s32, "code_00300578", cmpFreeSpriteToGS);

struct sdfMemHandle_t *cmpLoadSpriteHnd(CMP_SPR a_no) {
    struct sdfMemHandle_t *handle = NULL;

    if (a_no < 4) {
        handle = cmpWork3[a_no];
    }
    return handle;
}

struct sprHandle_t *cmpGetSpriteHnd(CMP_SPR a_no) {
    struct sprHandle_t *hSpr = NULL;

    if (a_no < 4) {
        hSpr = evtWork6[a_no];
    }
    return hSpr;
}

sdfTexHandle_t *cmpCallDevLoadFileEx(const char *filename)
{
    u32 resource;
    sdfMemHandle_t *fileAllocation = sdfDevLoadFileEx(filename, &resource, 0);
    sdfTexHandle_t *texture = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, resource));

    sdfMemFreeDelayedDirect(fileAllocation);
    return texture;
}

INCLUDE_ASM(const s32, "code_00300578", cmpCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "code_00300578", pipeRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00300578", pipeRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_00300578", pipeMessDraw);

INCLUDE_ASM(const s32, "code_00300578", pipeSetModelLightSub);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateBulletNum);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetSpriteHnd);

INCLUDE_ASM(const s32, "code_00300578", datCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_00300578", datDlcNoneCheck);

INCLUDE_ASM(const s32, "code_00300578", datLeaf82);

INCLUDE_ASM(const s32, "code_00300578", datLeaf83);

INCLUDE_ASM(const s32, "code_00300578", datLeaf84);

INCLUDE_ASM(const s32, "code_00300578", datCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_00300578", datCallGetRandIntA3);

INCLUDE_ASM(const s32, "code_00300578", datCallGetRandIntA4);

INCLUDE_ASM(const s32, "code_00300578", datFunc26);

INCLUDE_ASM(const s32, "code_00300578", cmpLeaf42);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetRandIntA);

INCLUDE_ASM(const s32, "code_00300578", cmpFunc5);

INCLUDE_ASM(const s32, "code_00300578", cmpLeaf43);

INCLUDE_ASM(const s32, "code_00300578", cmpFunc6);

INCLUDE_ASM(const s32, "code_00300578", cmpCampTestMomiji);

INCLUDE_ASM(const s32, "code_00300578", cmpCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_00300578", cmpSetDraw);

INCLUDE_ASM(const s32, "code_00300578", cmpGetDraw);

INCLUDE_ASM(const s32, "code_00300578", cmpSetType);

INCLUDE_ASM(const s32, "code_00300578", cmpSetId);

INCLUDE_ASM(const s32, "code_00300578", cmpSetNew);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawSpriteDef);

INCLUDE_ASM(const s32, "code_00300578", cmpSetAlphaRate);

INCLUDE_ASM(const s32, "code_00300578", fldCallSetFontColor);

/* body: written from the asm when landing; the found body was a different function */
void fldCamUpdateDefault(void)
{
    fldCallSetFontColor(255.0f);
}

s32 fldDrawProcessSub(f32 arg) {
    return fldCallSetFontColor(arg);
}

INCLUDE_ASM(const s32, "code_00300578", fldShopSetumeiSpr);

INCLUDE_ASM(const s32, "code_00300578", cmpCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_00300578", cmpGetFlagScreenShot);

INCLUDE_ASM(const s32, "code_00300578", cmpCallBitCheck);

void cmpInitCmpModelSub(void) {
    cmpInitCore2();
}

INCLUDE_ASM(const s32, "code_00300578", cmpSetLight);

INCLUDE_ASM(const s32, "code_00300578", cmpInitCore2);

INCLUDE_ASM(const s32, "code_00300578", cmpSetLightUnit);

INCLUDE_ASM(const s32, "code_00300578", cmpInitDstEff2);

INCLUDE_RODATA(const s32, "code_00300578", cmpSetLightTable);

INCLUDE_RODATA(const s32, "code_00300578", cmpSetLightTable2);

INCLUDE_RODATA(const s32, "code_00300578", cmpStrL);

INCLUDE_RODATA(const s32, "code_00300578", cmpDefaultFogColor);

INCLUDE_ASM(const s32, "code_00300578", cmpInitDstEff);

INCLUDE_ASM(const s32, "code_00300578", cmpCreateBstEff);

INCLUDE_ASM(const s32, "code_00300578", cmpDeleteBstEff);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawBstEff);

INCLUDE_ASM(const s32, "code_00300578", Init_cmpModel);

INCLUDE_ASM(const s32, "code_00300578", End_cmpModel);

INCLUDE_ASM(const s32, "code_00300578", cmpFreeResource);

dds3ModelResrc_t *cmpCallPrepare(MODELTYPE MajorNo, s32 MinorNo, s32 BlockMode)
{
    return cmpCallPrepareData = EE_ADDR_AS(dds3ModelResrc_t *, mdlPrepare(MajorNo, MinorNo, BlockMode));
}

dds3ModelResrc_t *Setup(MODELTYPE MajorNo, s32 MinorNo, s32 BlockMode) {
    return cmpCallPrepare(MajorNo, MinorNo, BlockMode);
}

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot);

INCLUDE_ASM(const s32, "code_00300578", cmpLoadStatusModel);

INCLUDE_ASM(const s32, "code_00300578", cmpWaitLoadModel);

INCLUDE_ASM(const s32, "code_00300578", cmpSetModelMotion);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot2);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot3);

INCLUDE_ASM(const s32, "code_00300578", cmpSetSmoothLoopMotion);

INCLUDE_ASM(const s32, "code_00300578", cmpGetMotionNum);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot4);

INCLUDE_ASM(const s32, "code_00300578", cmpGetMotionSpeed);

INCLUDE_ASM(const s32, "code_00300578", cmpSetModelScale);

INCLUDE_ASM(const s32, "code_00300578", cmpCallMemset2);

INCLUDE_ASM(const s32, "code_00300578", cmpCallFromEuler);

void cmpSetStatusModelParam(void)
{
    sdfVector4_t target;
    sdfVector4_t eye;
    u32 frame = dds3GetFrameID();

    SDF_VU0_COPYVECTOR(&aViewEye, &cmpStatusModelBasePos);
    SDF_VU0_COPYVECTOR(&aViewTarget, &cmpStatusModelPos);
    SDF_VU0_COPYVECTOR(&aViewDown, &cmpData2);
    dds3ScreenParam.fovy = 0.43633229f;
    sdf3MakeScreenParam(&dds3ScreenParam);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(gMapScrollReset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&target);
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&eye);
    LookAtMatrix(&target, &eye, &aViewDown);
    SDF_VU0_STOREMATRIX_NOMEM(&sdfViewMatrix);
    sdf3DMakeViewportPacket(&dds3DrawEnv[frame].viewport, &dds3ScreenParam, &sdfViewMatrix);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
}

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot5);

INCLUDE_ASM(const s32, "code_00300578", cmpLeaf46);

INCLUDE_ASM(const s32, "code_00300578", cmpLeaf47);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot6);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetFlagScreenShot7);

INCLUDE_ASM(const s32, "code_00300578", cmpAddPos);

INCLUDE_ASM(const s32, "code_00300578", cmpAddRot);

INCLUDE_ASM(const s32, "code_00300578", cmpAddScl);

void itfMesMngDebugShutdown(dds3ProcessID_t *proc_id)
{
    cmpData6[0] = 0.0f;
    cmpData6[1] = 0.0f;
    cmpData6[2] = 0.0f;
}

INCLUDE_ASM(const s32, "code_00300578", cmpClearDat);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawStatusModel);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawStatusModel_cmpModel);

INCLUDE_ASM(const s32, "code_00300578", cmpGetViewerLimit);

INCLUDE_ASM(const s32, "code_00300578", cmpSetViewerLimit);

INCLUDE_ASM(const s32, "code_00300578", cmpGetPosLimit);

INCLUDE_ASM(const s32, "code_00300578", cmpSetPosLimit);

INCLUDE_ASM(const s32, "code_00300578", cmpSetLightDef);

INCLUDE_ASM(const s32, "code_00300578", ModelLoadBuffEnd);

INCLUDE_RODATA(const s32, "code_00300578", cmpSetModelScaleTable);

void cmpCampEFFTES(void)
{
    f32 position[4] = { 128.0f, -100.0f, 64.0f, 0.0f };

    if (fldTitleMiniWork5 == NULL) {
        fldTitleMiniWork5 = sdfDevLoadFileEx("/camp/EFF_TES.EPL", &cmpCampEFFTESData, NULL);
        fldTitleMiniTex5 = dds3EffectLoadEPL((void *)cmpCampEFFTESData);
        fclInitDstEff2Sub((struct plistHandle_t *)fldTitleMiniTex5->instance);
        dds3EffectReset(fldTitleMiniTex5);
        dds3EffectSetPosition(fldTitleMiniTex5, position);
        dds3EffectSetLoop_dds3Effect(fldTitleMiniTex5, 1);
        dds3EffectSetScale(fldTitleMiniTex5, 1.5f);
    }
}

INCLUDE_ASM(const s32, "code_00300578", cmpDeleteEffect);

void cmpEffectSetRGBA(u32 a_rgba)
{
    if (fldTitleMiniTex5 != NULL) {
        effSetEffectColorSubSub(fldTitleMiniTex5, a_rgba);
    }
}

INCLUDE_ASM(const s32, "code_00300578", cmpDrawEffect);

INCLUDE_ASM(const s32, "code_00300578", cmpAddEffScl);

INCLUDE_ASM(const s32, "code_00300578", cmpGetMotionParam);

INCLUDE_ASM(const s32, "code_00300578", cmpSetModelLight);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdatemodel);

INCLUDE_ASM(const s32, "code_00300578", cmpViewerModel);

INCLUDE_ASM(const s32, "code_00300578", cmpCalcRestParamPly);

INCLUDE_ASM(const s32, "code_00300578", cmpCalcRestParamDvl);

INCLUDE_ASM(const s32, "code_00300578", cmpCalcRestParam);

INCLUDE_ASM(const s32, "code_00300578", cmpCalcParam);

INCLUDE_ASM(const s32, "code_00300578", cmpResetParam);

INCLUDE_ASM(const s32, "code_00300578", cmpCtrlSetParam);

INCLUDE_ASM(const s32, "code_00300578", cmpCallCreateLink);

INCLUDE_ASM(const s32, "code_00300578", cmpLvUpSeqFadeInStepUpdate);

INCLUDE_ASM(const s32, "code_00300578", cmpFuncEnd);

INCLUDE_RODATA(const s32, "code_00300578", cmpStrShichisei);

INCLUDE_RODATA(const s32, "code_00300578", cmpStrNichirin);

INCLUDE_ASM(const s32, "code_00300578", cmpCallClearDat);

INCLUDE_ASM(const s32, "code_00300578", CmpStatusSklItemSequence);

INCLUDE_ASM(const s32, "code_00300578", cmpSeqRankUp);

INCLUDE_ASM(const s32, "code_00300578", Release_cmpStatus);

INCLUDE_ASM(const s32, "code_00300578", CallProcEnd_cmpStatus);

INCLUDE_ASM(const s32, "code_00300578", Process_cmpStatus);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00300578", cmpCallDrawFont2);

INCLUDE_ASM(const s32, "code_00300578", cmpCallDrawFont3);

INCLUDE_ASM(const s32, "code_00300578", cmpCallDrawFont4);

INCLUDE_ASM(const s32, "code_00300578", cmpCallSetFontSpace);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetSpriteHnd3);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawSprList);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetSpriteHnd4);

INCLUDE_ASM(const s32, "code_00300578", cmpCallSprintf);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawParamCursor);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawParameter);

INCLUDE_ASM(const s32, "code_00300578", cmpGetSummonerRank2SprNo);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetSpriteHnd2);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawPartsName);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawPartsExp);

INCLUDE_ASM(const s32, "code_00300578", cmpGetSwordName);

INCLUDE_ASM(const s32, "code_00300578", cmpGetSwordParam);

INCLUDE_ASM(const s32, "code_00300578", cmpCallSprintf2);

INCLUDE_ASM(const s32, "code_00300578", cmpGetSwordNameEx);

INCLUDE_ASM(const s32, "code_00300578", cmpCallDrawSprList);

/* body: written from the asm when landing; the found body was a different function */
void cmpLeaf48(dds3ProcessID_t *target, u32 flag, s32 type)
{
    cmpCallDrawSprList(0, 2, target, flag, type);
}

INCLUDE_ASM(const s32, "code_00300578", cmpCallSprintf3);

INCLUDE_ASM(const s32, "code_00300578", cmpFunc);

INCLUDE_ASM(const s32, "code_00300578", cmpLeaf);

INCLUDE_ASM(const s32, "code_00300578", cmpCallCheckSkillEnable);

INCLUDE_ASM(const s32, "code_00300578", cmpCallIconDrawSprite);

INCLUDE_ASM(const s32, "code_00300578", cmpCallChkMessageDraw);

INCLUDE_ASM(const s32, "code_00300578", cmpCallMoverGetPosition);

INCLUDE_ASM(const s32, "code_00300578", DrawProcess_cmpStatus);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00300578", End);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00300578", cmpStartLevelUp);

INCLUDE_ASM(const s32, "code_00300578", cmpChkEnd);

INCLUDE_ASM(const s32, "code_00300578", cmpStartRankUp);

INCLUDE_ASM(const s32, "code_00300578", cmpChkWait2);

INCLUDE_ASM(const s32, "code_00300578", cmpChkEnd2);

INCLUDE_ASM(const s32, "code_00300578", cmpStartLearnSkillMessage);

INCLUDE_ASM(const s32, "code_00300578", cmpSetRecover);

INCLUDE_ASM(const s32, "code_00300578", cmpSetFlagDraw);

INCLUDE_ASM(const s32, "code_00300578", cmpChkDraw);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetProcessData35);

INCLUDE_ASM(const s32, "code_00300578", cmpSetFlagCamp);

INCLUDE_ASM(const s32, "code_00300578", StatusGoSword);

/* NON_MATCHING: 9 of 10 words match.  */
#ifdef NON_MATCHING
s32 cmpGetFlagCallDebug(void)
{
    return ((cmpStatusWork_t *)dds3GetProcessData(g_mainPID))->flags < 0;
}
#else
INCLUDE_ASM(const s32, "code_00300578", cmpGetFlagCallDebug);
#endif

INCLUDE_ASM(const s32, "code_00300578", Init_cmpStatus);

INCLUDE_ASM(const s32, "code_00300578", SetType);

INCLUDE_ASM(const s32, "code_00300578", cmpCallInitCmpModel);

void Shutdown_cmpStatus(dds3ProcessID_t *id) {
    cmpStatusWork_t *wk = dds3GetProcessData(id);

    Release_cmpStatus(wk);
    if (dds3ExistProcess(wk->drawPID)) {
        dds3KillProcess(wk->drawPID, 0);
        wk->drawPID = NULL;
    }
    sdfMemFreeDirect(wk->hMem);
    g_mainPID = NULL;
}

INCLUDE_RODATA(const s32, "code_00300578", cmpCallSprintfTable);

dds3ProcessID_t *ProcessStart_cmpStatus(void)
{
    struct sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(cmpStatusWork_t));
    cmpStatusWork_t *wk = sdfMemGetPtr(hMem);

    memset(wk, 0, sizeof(cmpStatusWork_t));
    wk->hMem = hMem;
    g_mainPID = dds3StartProcess("camp_status_main", 0x3EE, 0, 0, cmpCallGetProcessData2, (dds3ProcessFunc_t)Shutdown_cmpStatus, wk);
    wk->drawPID = dds3StartProcess("camp_status_draw", 0x2B03, 0, 0, cmpCallGetProcessData3, NULL, wk);
    cmpCallInitCmpModel(wk);
    return g_mainPID;
}

void ProcessEnd_cmpStatus(void) {
    cmpStatusWork_t *wk;

    if (g_mainPID != NULL && dds3ExistProcess(g_mainPID)) {
        wk = dds3GetProcessData(g_mainPID);
        if (!(wk->flags & 1)) {
            wk->flags |= 1;
            wk->step2 = 3;
            wk->count = 0;
            wk->step = 0;
        }
    }
}

s32 ProcessCheck_cmpStatus(void) {
    if (g_mainPID == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID) == 3 || dds3GetProcessStat(g_mainPID) == 0) {
        g_mainPID = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00300578", cmpCampScriptDevilguide);

INCLUDE_ASM(const s32, "code_00300578", cmpLeaf2);

INCLUDE_ASM(const s32, "code_00300578", cmpSetModeSq);

INCLUDE_ASM(const s32, "code_00300578", cmpSetStatus);

INCLUDE_ASM(const s32, "code_00300578", cmpRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00300578", cmpMenuDecFilter);

INCLUDE_ASM(const s32, "code_00300578", cmpMenuEmptyFilter);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID2);

INCLUDE_ASM(const s32, "code_00300578", cmpSkillIDStock);

INCLUDE_ASM(const s32, "code_00300578", CallFuncStatus);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID3);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID4);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID5);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetSpriteHnd5);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID6);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetsubMenuPID);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateUse);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateSkillList);

INCLUDE_ASM(const s32, "code_00300578", cmpCallMenuGetSelectID);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID7);

INCLUDE_ASM(const s32, "code_00300578", cmpPartSetParticle);

INCLUDE_ASM(const s32, "code_00300578", cmpDrawParticle);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID8);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID9);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetCharPID10);

s32 CmpOnMenu(s32 a_menu)
{
    return CmpOnMenuData[a_menu >> 16]();
}

INCLUDE_ASM(const s32, "code_00300578", cmpGetNoneItemText);

INCLUDE_ASM(const s32, "code_00300578", cmpRefTbldata);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetNoneItemText);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateUseItemEnebleDecide);

INCLUDE_ASM(const s32, "code_00300578", cmpCallMenuSetFlagMenu);

INCLUDE_ASM(const s32, "code_00300578", cmpCheckUseCharEnableSelect);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateSkillHD);

INCLUDE_ASM(const s32, "code_00300578", cmpCallFuncUse);

INCLUDE_ASM(const s32, "code_00300578", cmpCallFuncBullet);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateSword);

INCLUDE_ASM(const s32, "code_00300578", cmpCallGetSpriteHnd6);

INCLUDE_ASM(const s32, "code_00300578", cmpCallBitCheck2);

INCLUDE_ASM(const s32, "code_00300578", cmpSetup);

INCLUDE_ASM(const s32, "code_00300578", awdMenuProcess);

INCLUDE_ASM(const s32, "code_00300578", awdCallMenuGetWaitType);

INCLUDE_ASM(const s32, "code_00300578", cmpEndMesChk);

INCLUDE_ASM(const s32, "code_00300578", cmpUpdateMaterial);

s32 Update_cmpItem(void)
{
    return Update_cmpItemData[cmpData26 >> 16]();
}

void cmpLoadScript(void) {
    cmpFreeScriptData = sdfDevLoadFileEx(cmpStrCampScriptCmpUseScrBf, &effScalyTextureHandle2, NULL);
}

INCLUDE_ASM(const s32, "code_00300578", cmpFreeScript);

INCLUDE_ASM(const s32, "code_00300578", cmpStart);

INCLUDE_ASM(const s32, "code_00300578", cmpCallKillProcess);

INCLUDE_ASM(const s32, "code_00300578", cmpChkMessageDraw);

INCLUDE_ASM(const s32, "code_00300578", fldCommand_FLD_WARP_RUN_START);

INCLUDE_RODATA(const s32, "code_00300578", cmpStrCampScriptCmpUseScrBf);

s32 Process_cmpMap(dds3ProcessID_t *id) {
    tagCMP_MAP_WORK *work = dds3GetProcessData(id);

    switch (work->step) {
    case 0:
        mapFunc();
        work->flag |= 8;
        work->step = 1;
        dds3FadeOut(0, 0, 0, 1);
        return 0;
    case 1:
        if (work->count == 0) {
            dds3FadeIn(0, 0, 0, 10);
        } else if (work->count >= 10) {
            work->count = 0;
            work->step = 2;
            break;
        }
        work->count++;
        break;
    case 2:
        mapFunc();
        if (dds3PadButtonState[0x21] >= 0) {
            return 0;
        }
        work->step = 3;
        break;
    case 3:
        if (work->count == 0) {
            dds3FadeOut(0, 0, 0, 10);
            if (cmpProcessCheck()) {
                p2sdPlaySE(1, 127, 63);
            } else {
                p2sdPlaySE(18, 127, 63);
            }
        } else if (work->count >= 10) {
            work->count = 0;
            work->step = 4;
            break;
        }
        work->count++;
        break;
    case 4:
        if (work->count == 0) {
            work->flag &= ~8;
        } else if (work->count >= 2) {
            fldCallTexFreeDelayed();
            work->count = 0;
            work->step = 5;
            break;
        }
        work->count++;
        break;
    case 5:
        return -1;
    default:
        return 0;
    }
    return 0;
}

void cmpCallSetOt2(s32 x, s32 y, s32 frame) {
    u32 color[4];
    s32 px;

    eglSetOt(44);
    eglLinkTest(0, 0, 0, 0, 0, 0, 1, 2);
    eglLinkAlpha(0);
    color[0] = 0x80182328;
    color[1] = 0x80282A37;
    color[2] = 0x80282A37;
    color[3] = 0x80182328;
    if (frame < 70) {
        px = x + (s32)((f32)frame * 108.0f / 70.0f);
        eglLinkBoxZ(px - 108, y, 108, 173, 0, color[1], color[0], color[3], color[2]);
        eglLinkBoxZ(px, y, 108, 173, 0, color[0], color[1], color[2], color[3]);
        eglLinkBoxZ(px + 108, y, 108, 173, 0, color[1], color[0], color[3], color[2]);
    } else {
        px = x + (s32)((f32)(frame - 70) * 108.0f / 70.0f);
        eglLinkBoxZ(px - 108, y, 108, 173, 0, color[0], color[1], color[2], color[3]);
        eglLinkBoxZ(px, y, 108, 173, 0, color[1], color[0], color[3], color[2]);
        eglLinkBoxZ(px + 108, y, 108, 173, 0, color[0], color[1], color[2], color[3]);
    }
}

void cmpDrawBack(tagCMP_MAP_WORK *wk) {
    if (cmpGetSpriteHnd(CMP_SPR_MAP) == NULL) {
        return;
    }
    eglSetOt(44);
    eglLinkTest(1, 2, 128, 0, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkBoxZ(0, 0, 512, 58, 4095, 0, 0, 0, 0);
    eglLinkBoxZ(0, 405, 512, 43, 4095, 0, 0, 0, 0);
    eglLinkBoxZ(0, 58, 22, 347, 4095, 0, 0, 0, 0);
    cmpCallSetOt2(-39, 4, cmpDrawBackData);
    cmpCallSetOt2(311, -53, cmpDrawBackData);
    cmpCallSetOt2(222, 284, cmpDrawBackData);
    cmpDrawBackData++;
    if (cmpDrawBackData >= 141) {
        cmpDrawBackData = 0;
    }
    eglSetOt(44);
    eglLinkTest(1, 2, 128, 0, 0, 0, 1, 1);
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(-624, 32, 4095, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 0, 44);
    itfDrawGridWithResolvedSlot(4976, -424, 4095, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 32, 44);
    itfDrawGridWithResolvedSlot(3552, 2272, 4095, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 0, 44);
    eglSetOt(44);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(320, 448, 0, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 1, 44);
}
