#ifndef DDS_SDF_TEXTURE_SUBMIT_H

#define DDS_SDF_TEXTURE_SUBMIT_H
#include "sdf.h"

/* Select whether an upload copies its pixels or borrows them until transfer. */
typedef enum SdfTextureUploadOwnership {
    SDF_TEXTURE_UPLOAD_COPY = 0,
    SDF_TEXTURE_UPLOAD_BORROW = 1
} SdfTextureUploadOwnership;

#endif
