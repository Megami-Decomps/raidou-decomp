#ifndef SDF_DEV_CONS_H
#define SDF_DEV_CONS_H

#include "common.h"

typedef struct sdfDevConsCB_tag
{
    struct sdfDevConsCB_tag *prev;
    struct sdfDevConsCB_tag *next;
    u16 x;
    u16 y;
    s16 w;
    s16 h;
    s16 locX;
    s16 locY;
    u8 mode;
    u8 pad;
    u8 attr;
    u8 tabwidth;
    struct sdfMemHandle_t *bufferHandle;
    u8 *bufferPtr;
} sdfDevConsCB_tag;

/* The four print-control words are signed XYZ and packed RGBA when drawing. */
typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 attr;
} sdfDevPrintCB_t;

sdfDevConsCB_tag *sdfDevConsOpen(s32 aX, s32 aY, s32 aW, s32 aH);
void sdfDevConsClose(sdfDevConsCB_tag *aHndl);
void sdfDevConsAttribute(sdfDevConsCB_tag *aHndl, s32 aAttr);
void sdfDevConsSetMode(sdfDevConsCB_tag *aHndl, s32 aMode);
s32 sdfDevConsPrintf(sdfDevConsCB_tag *aHndl, const char *aFmt, ...);
void sdfDevSetPrintCB(sdfDevPrintCB_t *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr);
void *sdfDevVprintf(sdfDevPrintCB_t *aCB, const char *aStr, void *LegacyParamArray);
void *sdfDevPrintfEx(sdfDevPrintCB_t *aCB, const char *aStr, ...);

void *sdfDevPrintf(u32 x, u32 y, u32 z, s32 attr, const char *format, ...);

void sdfDevConsDrawAll(void);

#endif
