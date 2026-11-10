#ifndef DDS_SDF_DEV_STATE_H

#define DDS_SDF_DEV_STATE_H
#include "common.h"

#if !defined(SMT3_DEFINES_SdfDevOperationCode)
/* Protocol IDs use distinct byte-sized storage codes in sdfFileCmdHandle_t. */
typedef u8 SdfDevOperationCode;
#endif

#if !defined(SMT3_DEFINES_SdfDevStateCode)
typedef s8 SdfDevStateCode;
#endif

/* Queue acceptance is immediate; read, write, and size results arrive later. */
typedef s32 SdfFileCommandQueueResult;

#define SDF_FILE_COMMAND_QUEUE_RESULT_NOT_QUEUED -1

#define SDF_FILE_COMMAND_QUEUE_RESULT_QUEUED 0

#if !defined(SMT3_DEFINES_sdfFileCmdHandle_t)
/* Complete 0x40-byte owner allocated by sdfFileCmdNewHandle. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next; /* 0x00 */
    struct sdfFileCmdHandle_t *previous; /* 0x04 */
    struct sdfFileCmdHandle_t *workerNext; /* 0x08 */
    struct sdfFileCmdHandle_t *workerPrev; /* 0x0C */
    char *resolvedPath; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    SdfDevOperationCode operation; /* 0x15 */
    SdfDevStateCode state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 seekOffset; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    void *requestData; /* 0x20 */
    s32 options; /* 0x24 */
    s32 fileHandle; /* 0x28: SCE descriptor, or -1 when no file is open */
    s32 result; /* 0x2C */
    s32 transferred; /* 0x30 */
    u8 pad34[4]; /* 0x34 */
    void (*callback)(struct sdfFileCmdHandle_t *, s32, s32, s32, s32); /* 0x38 */
    void *callbackContext; /* 0x3C: caller-owned callback context */
} sdfFileCmdHandle_t;
#endif

typedef char DevState_size_must_be_0x40[(sizeof(sdfFileCmdHandle_t) == 0x40) ? 1 : -1];

typedef char DevState_operation_size_must_be_1[
    (sizeof(((sdfFileCmdHandle_t *)0)->operation) == 1) ? 1 : -1];

typedef char DevState_state_size_must_be_1[
    (sizeof(((sdfFileCmdHandle_t *)0)->state) == 1) ? 1 : -1];

#endif /* DDS_SDF_DEV_STATE_H */
