#ifndef SDFDRAW_H
#define SDFDRAW_H

#include "common.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdf_types.h"

void sdfDrawTagInit(sdfDrawTag_t *a);
void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);
void sdfDrawTagLink2(sdfDrawTag_t *aTag, addr_t aHeadPacket, addr_t aTailPacket);
void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, addr_t aPacket);
void sdfGrInitDraw(void);
void sdfGrJoinDrawTag(sdfDrawTag_t *aFore, sdfDrawTag_t *aBack);
void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);
void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

#endif
