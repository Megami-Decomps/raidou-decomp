#ifndef DDS_EVT_VIEWER_ROWS_H

#define DDS_EVT_VIEWER_ROWS_H
#include "evt_polygon_movie.h"

/* The viewer reads these fixed row strides from borrowed PMD section data.
 * Only the observed payload prefixes are described here. */
typedef struct EvtCompactRow {
    u16 value;
    u16 parameter;
    u16 flags;
    s16 variant;
    u8 payload[8];
} EvtCompactRow;

typedef struct EvtExtendedRow {
    u16 value;
    u16 parameter;
    u16 flags;
    s16 variant;
    u8 reserved08[4];
    u8 payload[0x20];
} EvtExtendedRow;

/* Timeline row value 2 stores the camera FOV at payload +0. The remaining
 * bytes shown here retain only the observed halfword access and opaque tail. */
typedef struct EvtCameraFovRowPayload {
    f32 cameraFieldOfView;
    u16 opaque04;
    u16 opaque06;
} EvtCameraFovRowPayload;

typedef char EvtCompactRow_size[(sizeof(EvtCompactRow) == 0x10) ? 1 : -1];

typedef char EvtExtendedRow_size[(sizeof(EvtExtendedRow) == 0x2C) ? 1 : -1];

typedef char EvtCameraFovRowPayload_size[(sizeof(EvtCameraFovRowPayload) == 8) ? 1 : -1];

u16 evtGetRowValue(const PolyMovieWork *work, s32 row);

s16 evtGetRowVariant(const PolyMovieWork *work, s32 row);

u16 evtGetRowParameter(const PolyMovieWork *work, s32 row);

u16 evtGetRowFlags(const PolyMovieWork *work, s32 row);

const u8 *evtGetRowPayloadAddress(const PolyMovieWork *work, s32 row);

#endif
