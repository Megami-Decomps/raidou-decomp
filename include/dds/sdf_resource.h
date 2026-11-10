#ifndef DDS_SDF_RESOURCE_H

#define DDS_SDF_RESOURCE_H
#include "../sdf_resource.h"
/* Engine resource records the packet builders need but the engine owners in
 * libsdf.h do not have to drag in with them. Same intent as sdf_types.h one
 * layer down: a unit that builds a VIF packet from a material or a TMX header
 * includes this and gets the canonical layout, without the queue, file and
 * motion definitions it has no use for.
 *
 * libsdf.h includes this, so there is exactly one definition of each record.
 * The pointed-to types stay forward declarations on purpose: a unit that also
 * carries its own copy of one of them keeps compiling. */

#include "common.h"

struct sdfMatPacket_t;

struct sdfTexPacket_t;

/* DDS counts a texture's references; a duplicate's origin owns the pixels. */
typedef struct sdfTexRef_t
{
    struct sdfTexHandle_t *origin;
    s32 refCount;
} sdfTexRef_t;

#endif /* DDS_SDF_RESOURCE_H */
