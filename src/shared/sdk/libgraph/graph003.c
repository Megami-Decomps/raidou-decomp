#include "common.h"
#include "ps2asm.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"

extern s32 sceClose(s32 a);
extern s32 atoi(const char *s);

/* Reference implementation; see docs/matching/sdk/libgraph.md. */

s32 checkModelVersion(void)
{
    s8 buf[0x100];
    s32 fd;
    u32 i;

    fd = sceOpen("rom0:ROMVER", 1);
    if (fd < 0) {
        return -1;
    }
    for (i = 0; i < 0x100; i++) {
        sceRead(fd, &buf[i], 1);
        if (buf[i] == 0) {
            break;
        }
    }
    sceClose(fd);
    return 0x1315670 < atoi(&buf[i - 9]);
}

INCLUDE_ASM(const s32, "sdk/libgraph/graph003", _GetGsDxDyOffset);

INCLUDE_ASM(const s32, "sdk/libgraph/graph003", sceGsSetDefDispEnv);
