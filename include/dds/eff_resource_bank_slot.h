#ifndef DDS_EFF_RESOURCE_BANK_SLOT_H

#define DDS_EFF_RESOURCE_BANK_SLOT_H
#include "common.h"

typedef struct EffResourceBankSlot {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    char name[0x34];     /* Selected resource name without its extension. */
    s32 type;            /* Resource category returned by the formatter. */
    s32 selectionStatus; /* Browser selection status. */
    s32 pathVariant;     /* Selected path variant returned by the browser. */
    u8 opaque108[8];     /* In caller-sized buffers; not written by this provider. */
} EffResourceBankSlot;

typedef char EffResourceBankSlot_size_must_be_0x110[
    sizeof(EffResourceBankSlot) == 0x110 ? 1 : -1];

typedef char EffResourceBankSlot_fullName_must_be_0xC8[
    sizeof(((EffResourceBankSlot *)0)->fullName) == 0xC8 ? 1 : -1];

#endif
