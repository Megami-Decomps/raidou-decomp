#include "shared/kernel/dds3KernelDraw/decls.h"

/* The two live XYZ2 payload locals follow the matched DDS engine source.
 * Creating each before its RGBAQ pair preserves the native hoisted constants. */
void dds3InitZClearDrawTag(void)
{
    s32 state;
    s32 buffer;
    u64 topLeft;
    u64 bottomRight;

    for (state = 0; state < 3; state++) {
        for (buffer = 0; buffer < 2; buffer++) {
            dds3ZClearDrawTag[state][buffer].dma.qwc = 0xA;
            dds3ZClearDrawTag[state][buffer].dma.pad = 0;
            dds3ZClearDrawTag[state][buffer].dma.id = 0x70;
            dds3ZClearDrawTag[state][buffer].dma.addr = NULL;
            dds3ZClearDrawTag[state][buffer].dma.dat0 = 0;
            dds3ZClearDrawTag[state][buffer].dma.dat1 = 0x5000000A;
            dds3ZClearDrawTag[state][buffer].testGif.parm = 0x1000000000000001;
            dds3ZClearDrawTag[state][buffer].testGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].test.reg = 0x47;
            dds3ZClearDrawTag[state][buffer].test.data = 0x32001;
            dds3ZClearDrawTag[state][buffer].clearGif.parm = 0x1003400000000004;
            dds3ZClearDrawTag[state][buffer].clearGif.regs = 0xE;
            topLeft = 0x79007000;
            dds3ZClearDrawTag[state][buffer].colorA.reg = 1;
            dds3ZClearDrawTag[state][buffer].colorA.data = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].positionA.reg = 5;
            dds3ZClearDrawTag[state][buffer].positionA.data = topLeft;
            bottomRight = 0x87009000;
            dds3ZClearDrawTag[state][buffer].colorB.reg = 1;
            dds3ZClearDrawTag[state][buffer].colorB.data = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].positionB.reg = 5;
            dds3ZClearDrawTag[state][buffer].positionB.data = bottomRight;
            dds3ZClearDrawTag[state][buffer].blendGif.parm = 0x1000000000008002;
            dds3ZClearDrawTag[state][buffer].blendGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].blend[0].reg = 0x47;
            dds3ZClearDrawTag[state][buffer].blend[0].data = 0x5101B;
            dds3ZClearDrawTag[state][buffer].blend[1].reg = 0x42;
            dds3ZClearDrawTag[state][buffer].blend[1].data = 0x44;
        }
    }
}
