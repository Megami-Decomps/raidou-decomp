#ifndef DDS_BTL_EFFECT_TABLE_H

#define DDS_BTL_EFFECT_TABLE_H
#include "common.h"

/* Eight-byte rows from /battle/EFFECT.TBL. */
typedef struct BtlEffectActionRecord {
    u8 unknown00[2];
    u16 kind; /* 0x02 */
    s8 participantSelectors[3]; /* 0x04: -1 omits the corresponding participant. */
    u8 unknown07;
} BtlEffectActionRecord;

typedef char BtlEffectActionRecordSizeCheck[
    sizeof(BtlEffectActionRecord) == 8 ? 1 : -1];

extern BtlEffectActionRecord *D_00435E34;

#endif /* DDS_BTL_EFFECT_TABLE_H */
