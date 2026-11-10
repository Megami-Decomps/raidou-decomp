#ifndef DDS_SDF_PAC_WORK_H

#define DDS_SDF_PAC_WORK_H
#include "common.h"

struct PacState;

struct SdfPacStreamPacketHeader;

/* Queue node allocated with an inline, variable-length PAC packet. */
typedef struct PacWork {
    struct PacWork *next; /* 0x00 */
    struct PacState *owner; /* 0x04 */
    void *resourceHandle; /* 0x08: type selected by the packet command */
    u8 *dataCursor; /* 0x0C */
    u8 packet[0]; /* 0x10: copied packet header, extensions, and data */
} PacWork;

typedef char PacWork_size_must_be_0x10[(sizeof(PacWork) == 0x10) ? 1 : -1];

/* Packet command selects the pointer type stored in resourceHandle. */

#define PAC_WORK_ALLOCATION(work) \
    ((struct sdfMemHandle_t *)(work)->resourceHandle)

#define PAC_WORK_TEXTURE(work) \
    ((struct sdfTexHandle_t *)(work)->resourceHandle)

#define PAC_WORK_TEXTURE_LIST(work) \
    ((struct DevRequest *)(work)->resourceHandle)

#endif /* DDS_SDF_PAC_WORK_H */
