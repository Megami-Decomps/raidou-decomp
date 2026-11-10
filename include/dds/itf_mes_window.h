#ifndef DDS_ITF_MES_WINDOW_H

#define DDS_ITF_MES_WINDOW_H
#include "common.h"

#include "itf_mes_entry_index.h"

#include "itf_mes_entry_items_result.h"

#include "itf_option_bit_index_api.h"

#include "itf_mes_window_index.h"

struct ItfMesSub;

/* The stored word mixes low-half state with high-half flag bits. */
typedef u32 ItfMesStatusWord;

/* Mutators accept only the flag domain stored in the status word's high half. */
typedef u32 ItfMesHighStatusBits;

/* Return the entry's item-list address, NONE, or the BRANCH sentinel. */
ItfMesEntryItemsResult itfMesGetWindowEntryItems(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

u32 itfMesGetEntryCount(ItfMesWindowIndex window);

void evtLipsExecFunction(s32 unitUniqueId, s32 motionId);

struct ItfMesBlk40;

struct ItfMesBlkA4;

struct ItfMesTextSlots;

/* Initialize the cursor state used by a newly reset message window. */
void itfInitializeCursorResetState(struct ItfMesBlk40 *cursor);

/* Release the window's three sprites and restore its default bounds/fade limit. */
void itfResetWindowResourceBlock(struct ItfMesBlkA4 *block);

struct sdfTexHandle_t;

struct sdfTexHandle_t *itfMesGetWindowTexture(void);

#endif /* DDS_ITF_MES_WINDOW_H */
