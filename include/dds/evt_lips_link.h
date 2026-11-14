#ifndef DDS_EVT_LIPS_LINK_H

#define DDS_EVT_LIPS_LINK_H
#include "common.h"

struct evtUnitHandle_t;

struct dds3ModelHandle_t;

/* Consumer view of the model owner reached from an event lip-sync link. */
typedef struct EvtLipsMh {
    u8 opaque00[0x0C];
    struct dds3ModelHandle_t *model; /* 0x0C */
} EvtLipsMh;

/* Consumer view of a lip-sync entry in the world's unit list. */
typedef struct EvtLipsLink {
    u8 opaque00[0x08];
    struct evtUnitHandle_t *unit; /* 0x08 */
    EvtLipsMh *mh; /* 0x0C */
} EvtLipsLink;

typedef char EvtLipsMhSizeCheck[(sizeof(EvtLipsMh) == 0x10) ? 1 : -1];

typedef char EvtLipsLinkSizeCheck[(sizeof(EvtLipsLink) == 0x10) ? 1 : -1];

#endif /* DDS_EVT_LIPS_LINK_H */
