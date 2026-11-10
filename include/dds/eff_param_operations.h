#ifndef DDS_EFF_PARAM_OPERATIONS_H

#define DDS_EFF_PARAM_OPERATIONS_H
#include "common.h"

/* Native 0x28-byte operation row, indexed by effect kind.
 * The scale operation takes a float in addition to its payload.
 * In the extended table, a duplicate callback selects raw-source creation
 * and cloning; its absence selects the kind/tableIndex fallback table. */
typedef struct EffDispatchEntry {
    void *(*create)(void *);               /* 0x00 */
    void (*dispatch)(void *);              /* 0x04 */
    void (*release)(void *);               /* 0x08 */
    void *(*duplicate)(void *);            /* 0x0C */
    void (*setPosition)(void *payload, void *positionSource); /* 0x10 */
    void (*setScale)(void *, f32);         /* 0x14 */
    void (*setRotationMatrix)(void *payload, void *matrix); /* 0x18 */
    void (*setColor)(void *payload, u32 color); /* 0x1C */
    void (*setDirection)(void *payload, void *direction); /* 0x20 */
    void (*setArea)(void *payload, f32 area); /* 0x24 */
} EffDispatchEntry; /* 0x28 */

#endif
