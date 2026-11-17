#include "common.h"
#include "effect.h"
#include "effPacket.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdf_packet_list.h"

INCLUDE_ASM(const s32, "code_0020EE08", effEctBlurDrawAlphaMultiSub);

void effDrawBlurRectangle(const dds3Effect_Blur_t *source) {
    sdfDrawTag_t *list;
    sdfGrRefPacketHeader_t *tag;
    u64 *samplingPacket;
    u64 *textureAlphaPacket;
    u64 *blendPacket;
    u64 *clampPacket;
    sdfGrPacketHeader_t *drawPacket;

    if (dds3NullDrawProc() == 0) {
        list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(list);
        tag = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20));
        sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), tag);
        samplingPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        samplingPacket[0] = 2;
        samplingPacket[1] = 0x5000000210000000ULL;
        samplingPacket[2] = 0x1000000000008001ULL;
        samplingPacket[3] = 0xE;
        samplingPacket[4] = 0x61;
        samplingPacket[5] = 0x14;
        sdfDrawTagLink(list, EE_ADDR(samplingPacket));
        textureAlphaPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        textureAlphaPacket[0] = 3;
        textureAlphaPacket[1] = 0x5000000310000000ULL;
        textureAlphaPacket[2] = 0x1000000000008002ULL;
        textureAlphaPacket[3] = 0xE;
        textureAlphaPacket[4] = 0x8000000080ULL;
        textureAlphaPacket[5] = 0x3B;
        textureAlphaPacket[6] = 0;
        textureAlphaPacket[7] = 0x3F;
        sdfDrawTagLink(list, EE_ADDR(textureAlphaPacket));
        blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = source->blend;
        blendPacket[7] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(blendPacket));
        clampPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        clampPacket[0] = 2;
        clampPacket[1] = 0x5000000210000000ULL;
        clampPacket[2] = 0x1000000000008001ULL;
        clampPacket[3] = 0xE;
        if (dds3BistaCheck() != 0) {
            clampPacket[4] = ((s64)(((447 - dds3Data33) >> 1) - 1) << 34) |
                             (((s64)(dds3DrawBistaData2 + 1) >> 1) << 24) | 9;
        } else {
            clampPacket[4] = 0x37C00000009ULL;
        }
        clampPacket[5] = 8;
        sdfDrawTagLink(list, EE_ADDR(clampPacket));
        drawPacket = effPkt2DAllocTriStrip(1, 0);
        dds3CallCos(source, effPkt2DGetPrimPtr(drawPacket), 0);
        sdfDrawTagLink(list, EE_ADDR(drawPacket));
        effData3.regFunc(&effData3, list);
    }
}

INCLUDE_ASM(const s32, "code_0020EE08", effCallGrAllocPacketBuff);
