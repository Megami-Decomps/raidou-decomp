#ifndef CMPVIEW_H
#define CMPVIEW_H

#include "common.h"
#include "sdf_resource.h"
#include "draw3d.h"

/* Camp menu 3D view chains, one per frame buffer. */
extern sdfDrawTag_t cmpViewTag[2];
extern sdf3DInitPacket_t cmpViewInitPacket[2];
extern sdfDrawTag_t cmpLightTag[2];
extern sdf3DLightParallelPacket_t cmpLightPacket[2];
extern sdf3DViewportPacket_t cmpViewportPacket[2];

void cmpDrawSetup3DView(void);

#endif
