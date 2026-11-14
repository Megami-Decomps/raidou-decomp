#ifndef DDS_BTL_ENC_BG_ENTRY_H

#define DDS_BTL_ENC_BG_ENTRY_H
#include "common.h"

/* Conditional parameter entry read by the queued battle-scene resolver. */
typedef struct EncBgEntry {
    s32 fallbackParameter;
    s16 flagA;
    s16 parameterA;
    s16 flagB;
    s16 parameterB;
} EncBgEntry;

typedef char EncBgEntry_size_must_be_0xC[
    (sizeof(EncBgEntry) == 0xC) ? 1 : -1];

typedef struct EncBgRow {
    s32 mapIdBase;
    EncBgEntry entries[64];
} EncBgRow;

typedef char EncBgRow_size_must_be_0x304[
    (sizeof(EncBgRow) == 0x304) ? 1 : -1];

#endif /* DDS_BTL_ENC_BG_ENTRY_H */
