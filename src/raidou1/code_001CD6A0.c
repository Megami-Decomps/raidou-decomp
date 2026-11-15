#include "common.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "kernel_draw.h"
#include "libsdf.h"
#include "sdk_libc.h"
#include "sdk_libgcc.h"
#include "dds/btl.h"
#include "dds/dds3_draw_buffer.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_append.h"
#include "dds/sdf_packet_builders.h"
#include "dds/sdf_packet_header.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_primitive_packet_api.h"
#include "dds/sdf_projection.h"
#include "dds/sdk_libc.h"

extern void *memcpy(void *, const void *, unsigned int);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

#include "dds/sdk_ps2.h"
#include "dds/sdf_gs_header.h"
#include "dds/sdf_gs_alpha_register_api.h"
#include "dds/sdf_gs_register_submit_api.h"
#include "dds/dds3_frame_draw_buffer.h"
#include "dds/ui_color.h"
#include "dds/sdf_cons_matrix_packet.h"
#include "dds/sdf_dma_packet_header.h"
#include "dds/eff_resource_records.h"
#include "dds/eff_slot_phase.h"
#include "dds/itf.h"
#include "dds/fpu.h"
#include "dds/itf_grid_text.h"
#include "dds/itf_grid_text_color_api.h"
#include "dds/sdf_grid_seek_api.h"
#include "dds/sdf.h"
#include "dds/mnu.h"
#include "dds/itf_draw_grid.h"
#include "dds/eff_resource_slots.h"
#include "dds/sdf_draw.h"
#include "dds/pcp_vu0.h"
#include "dds/grid_packed_vertex.h"
#include "dds/itf_grid_status_records.h"

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
