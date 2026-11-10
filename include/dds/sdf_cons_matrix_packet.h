#ifndef DDS_SDF_CONS_MATRIX_PACKET_H

#define DDS_SDF_CONS_MATRIX_PACKET_H
#include "common.h"

/* Shared 0xD0 VIF packet: two stored matrices, three vector slots, and trailer. */
typedef struct ConsMatrixPacket {
    u16 quadwords;        /* 0x00 */
    u8 unknown02[6];      /* 0x02 */
    u32 reservedWord;     /* 0x08 */
    u32 command;          /* 0x0C */
    u8 matrixA[0x40];     /* 0x10 */
    u8 matrixB[0x40];     /* 0x50 */
    u8 vecA[0x10];        /* 0x90 */
    u8 vecB[0x10];        /* 0xA0 */
    u8 vecC[0x10];        /* 0xB0 */
    u32 stmodCommand;     /* 0xC0 */
    u32 mscalCommand;     /* 0xC4 */
    u32 unknownC8;        /* 0xC8 */
    u32 unknownCC;        /* 0xCC */
} QWORD_ALIGNED ConsMatrixPacket;

typedef ConsMatrixPacket SdfVuBonePacket;

typedef char ConsMatrixPacket_size_must_be_0xD0[
    (sizeof(ConsMatrixPacket) == 0xD0) ? 1 : -1];

#endif /* DDS_SDF_CONS_MATRIX_PACKET_H */
