#ifndef DDS_SDF_TYPES_H

#define DDS_SDF_TYPES_H
#include "../billboard.h"
#include "../sdf_types.h"
/* Native math and DMA types shared by renderer packet headers. Keeping these
 * independent of the engine owners lets packet constructors use the canonical
 * layouts without importing unrelated material, texture and queue definitions. */

#include "common.h"

/* A vector or matrix at a retail address that is not 16-aligned. Same type, so
 * copies to and from sdfVector4_t compile as before; a host build lowers the
 * alignment the typedef above raised, so the access is not a movaps. */

#ifndef UNALIGNED_QWORD

#endif

#endif /* DDS_SDF_TYPES_H */
