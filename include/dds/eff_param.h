#ifndef DDS_EFF_PARAM_H

#define DDS_EFF_PARAM_H
#include "common.h"

#if !defined(SMT3_DEFINES_effPartHandle_t)
/* Compact effect owner: its halfword kind selects the opaque payload's operations. */
typedef struct effPartHandle_t {
    u16 kind;
    u8 pad02[2];
    void *payload;
} effPartHandle_t;
#endif

/* Extended adapter owner: a full-word kind and retained fallback-table index. */
typedef struct EffParamWorkEx {
    u32 kind;
    u32 tableIndex;
    void *payload;
} EffParamWorkEx;

typedef char EffParamWorkEx_size_must_be_0xC[(sizeof(EffParamWorkEx) == 0xC) ? 1 : -1];

typedef char EffParamWork_size_must_be_8[(sizeof(effPartHandle_t) == 8) ? 1 : -1];

/* The table starts with 0x10 bytes of header, followed by 0x10-byte records.
 * Each blockOffset is signed and relative to the start of the whole table. */
typedef struct EffParamTableRecord {
    u32 word0;
    s32 blockOffset;
    u32 kindAndFlags;
    u32 wordC;
} EffParamTableRecord;

typedef struct EffParamTable {
    u8 header[0x10];
    EffParamTableRecord records[1];
} EffParamTable;

typedef char EffParamTableRecord_size_must_be_0x10[
    (sizeof(EffParamTableRecord) == 0x10) ? 1 : -1];

#endif
