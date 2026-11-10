#ifndef DDS_EFF_FILE_JOB_REQUEST_H

#define DDS_EFF_FILE_JOB_REQUEST_H
#include "common.h"

struct sdfMemHandle_t;

typedef struct EffFileJobRequest {
    char *name;                       /* 0x00 */
    u16 fileKind;                     /* 0x04 */
    u8 pad06[2];
    u16 transferMode;                /* 0x08 */
    u8 pad0A[2];
    u8 *outputBuffer;                 /* 0x0C */
    u32 size;                         /* 0x10 */
    struct sdfMemHandle_t *allocation;   /* 0x14 */
    u16 resourceMode;                 /* 0x18 */
    u8 pad1A[2];
    const char *secondaryCommandPath; /* 0x1C */
} EffFileJobRequest;

typedef char EffFileJobRequestSizeCheck[(sizeof(EffFileJobRequest) == 0x20) ? 1 : -1];

#endif
