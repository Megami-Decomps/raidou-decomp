#ifndef CODE_R1_0019_D3_A8_H
#define CODE_R1_0019_D3_A8_H

#include "common.h"

#define SDF_GS_CONTEXT_1 0

/* Complete command header passed to sdfDevSetPrintCB and the SIF formatter. */
typedef struct SifCommand {
    s32 source;   /* 0x00 */
    s32 end;      /* 0x04 */
    s32 argument; /* 0x08 */
    u32 command;  /* 0x0C */
} SifCommand;

typedef char SifCommandSizeCheck[sizeof(SifCommand) == 0x10 ? 1 : -1];

#endif
