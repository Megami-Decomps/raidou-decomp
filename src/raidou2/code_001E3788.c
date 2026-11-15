#include "common.h"
#include "sdfGsMemIO.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "kernel_draw.h"
#include "libsdf.h"
#include "sdk_libc.h"
#include "sdk_libgcc.h"



/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10

/* Result bit 1: at least widget->rows successors remain from firstVisible. */
enum { GRID_LIST_HAS_COUNT_FOLLOWERS = 2 };

typedef struct GridDrawWork {
    u8 pad00[0xC];
    s16 width;               /* 0x0C */
    s16 height;              /* 0x0E */
    u32 packetHandle;        /* 0x10 */
    u32 overlayHandle;       /* 0x14 */
    u8 overlayEnabled;       /* 0x18 */
    u8 pad19;
    u8 overlayKind;          /* 0x1A */
    u8 pad1B[0x19];
    s32 overlayDataSize;     /* 0x34 */
} GridDrawWork;

void itfCreateGridPacketWithDefaultFlags(u32 packetHandle, s32 width, s32 height, u8 *data,
                  sdfDrawTag_t *context);

typedef struct ItfGridResource {
    u32 handle;
    u8 pad04[0x18];
    u32 entryCount;
} ItfGridResource;

/* The FLUSH_01 status payload stores the signed IN, WAIT and OUT durations. */

void itfCreateGridPacketWithDefaultFlags(u32 packetHandle, s32 width, s32 height, u8 *data,
                  sdfDrawTag_t *context) {
    sdfLinkLoadImage(context, (sdfGsMemSlice_t *)packetHandle,
                              0, 0, width, height, (s32)data, 0);
}
