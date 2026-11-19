#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "effManager.h"
#include "effect.h"
#include "effPacket.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdf_packet_list.h"
#include "effPCPHelperBED.h"

INCLUDE_ASM(const s32, "code_002068E8", effPCPHelperBEDSetPosition);

INCLUDE_ASM(const s32, "code_002068E8", effPCPHelperBEDSetScale);

void effPCPHelperBEDSetColor(effPCPHelperBED_tag *hData, u32 color) {
    hData->color = color;
}

INCLUDE_ASM(const s32, "code_002068E8", effLeaf193);

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:25, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 effPcpKind88CreateDisabled(void) {
    return 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:30, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 effPcpKind88DuplicateDisabled(void) {
    return 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:35, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind88ReleaseNoOp(void) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:39, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind88DispatchNoOp(void) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:43, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind88ScaleNoOp(void) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:47, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind88ValueNoOp(void) {
}

INCLUDE_ASM(const s32, "code_002068E8", effLeaf194);

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:55, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 effPcpKind95CreateDisabled(void) {
    return 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:60, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 effPcpKind95DuplicateDisabled(void) {
    return 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:65, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind95ReleaseNoOp(void) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:69, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind95DispatchNoOp(void) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00184420.c:73, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPcpKind95ValueNoOp(void) {
}
