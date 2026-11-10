#ifndef DDS_KWLN_H

#define DDS_KWLN_H
#include "../kernel.h"
#include "common.h"

struct sdfDrawTag_t;

/* Shared fog/depth state consumed by sdf3DMakeFogPacket (0x14).
 * The viewer config serializes fogNear, depthNear, fogFar, depthFar, fogColor;
 * timeline payloads store fogFar before fogNear instead. */
typedef struct KwlnDrawVectorParams {
    f32 fogNear;    /* 0x00: minimum fog range. */
    f32 fogFar;     /* 0x04: maximum fog range. */
    f32 depthNear;  /* 0x08: near depth used in the projection coefficients. */
    f32 depthFar;   /* 0x0C: far depth used in the projection coefficients. */
    u32 fogColor;   /* 0x10: packed GS FOGCOL value. */
} KwlnDrawVectorParams;

typedef char KwlnDrawVectorParams_size[(sizeof(KwlnDrawVectorParams) == 0x14) ? 1 : -1];

/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 dds3TaskUpdateResult_t;

/* Called after linked children receive immediate kill requests and the task
 * leaves scheduler queues, while its allocation is still valid; the scheduler
 * frees it after the callback returns. */
typedef void (*dds3ProcessShutdownFunc_t)(dds3ProcessID_t *task);

#endif /* DDS_KWLN_H */
