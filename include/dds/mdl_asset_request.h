#ifndef DDS_MDL_ASSET_REQUEST_H

#define DDS_MDL_ASSET_REQUEST_H
#include "common.h"

/* The API returns a group address in the same word used for its two non-ready
 * outcomes, so retain the signed 32-bit representation at the ABI boundary. */
typedef s32 MdlAssetRequestResult;

/* Zero queues asset loading; nonzero waits for the requested group to load. */
typedef s32 MdlAssetRequestMode;

enum {
    MDL_ASSET_REQUEST_NONBLOCKING = 0,
    MDL_ASSET_REQUEST_BLOCKING = 1,
};

enum {
    MDL_ASSET_REQUEST_QUEUED = 0,
    MDL_ASSET_REQUEST_ALREADY_PENDING = -1,
};

#endif /* DDS_MDL_ASSET_REQUEST_H */
