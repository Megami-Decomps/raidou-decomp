#ifndef DDS3KERNELMISC_H
#define DDS3KERNELMISC_H

#include "common.h"
#include "sdf_resource.h"

void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor);
void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z);

void dds3ConsPanelTagLink(struct sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
void dds3ConsPanelTagLinkZ(struct sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z);

#endif
