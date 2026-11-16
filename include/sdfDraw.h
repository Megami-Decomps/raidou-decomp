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
void sdfPreDrawChainInit(sdfPreDrawChain_t *aChain);
void sdfPreDrawChainLink(sdfPreDrawChain_t *aChain, sdfPreDrawTag_t *aTag);
void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);
void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, addr_t aPacket);
void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);
void sdfGrSetFrameMaskReset(const u8 *aEnv, sdfGrRefGifPacketHeader_t *aHdr, s32 aContext);
void sdfGrReqDraw(sdfDrawTag_t *aTag, sdfPreDrawChain_t *aPreDraw);
sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt);
void sdfGrFlushOtChain(u32 *source);
extern volatile u8 sdfDrawPublishFlag;
extern s8 GlobalParamFlag;
extern u16 sdfReqDrawDelaySleepCount;
extern u16 sdfDrawFrameCounter;
void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt);
void sdfPreDrawTagInit(sdfPreDrawTag_t *aTag);
void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, sdfPreDrawHook_t *aEnv);
void sdfGrReqDrawDelay(s32 count);

#endif
