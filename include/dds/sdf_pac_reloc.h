#ifndef DDS_SDF_PAC_RELOC_H

#define DDS_SDF_PAC_RELOC_H
#include "common.h"

/* Prefix on relocated PAC payloads: tableBytes is cleared after fixups. */
typedef struct PacReloc {
    s32 tableOffset; /* 0x00: relocation table displacement from payload */
    s32 tableBytes; /* 0x04: relocation table byte count */
    u8 pad08[8];
    u8 payload[0]; /* 0x10: variable-length relocated payload */
} PacReloc;

#endif /* DDS_SDF_PAC_RELOC_H */
