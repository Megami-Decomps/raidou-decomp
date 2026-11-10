#ifndef DDS_EEREGS_H

#define DDS_EEREGS_H
#include "../eeregs.h"
#include "common.h"

/*
 * Subset of the Sony SDK eeregs.h: the EE and GS registers retail code
 * touches, by their SDK names.
 */

/* A register as a pointer to its address. A build with no register space at
 * these addresses defines this to point into storage of its own. */

#ifndef SDF_HW_REG

#endif

/* D0 and D1 are the VIF DMA channels, D3 fromIPU and D4 toIPU. */

/* Channel 2 is the GIF's. sdk/libgraph.c spells this register itself, as a
 * dereference rather than a pointer, because it includes no eeregs.h; a
 * consumer transcribing sceGsSyncPath reads it through HW_READ and needs the
 * pointer form here. */

/* GS privileged registers. */

/* Scratchpad RAM: 16KB of on-chip memory with no cache, mapped at a fixed
 * physical address. sdfVu1.c's point buffer uses one fixed region of it as
 * a second ring alongside corePktPointBuff. A build with no scratchpad
 * defines this as a host buffer's address instead. */

#ifndef SDF_SCRATCHPAD_POINT_BUFFER

#endif

/* VU0's data memory, reached from the EE side as ordinary memory while VU0
 * is idle. sdfCoreDraw.c writes transform, light and texture data here
 * directly ahead of a VU0 microprogram run, bypassing the vf-register
 * macros. A build with no VU0 data memory mapped this way defines this to
 * wherever that data lands instead. */

#ifndef SDF_VU0_DATA_MEMORY

#endif

/* A DMA tag: the address in the high word, the control word in the low one.
 * Absorbs the physical conversion, so a consumer that redefines EE_PHYSICAL
 * gets every tag with it and never meets the mask at a tag site.
 *
 * word is widened without narrowing first, which preserves what the
 * open-coded sites did including sign extension. Three callers pass a
 * computed word rather than a literal (sdfModel.c's qwc, count and packed,
 * each OR-ed with 0x30000000), so if one of those ever had bit 31 set the
 * widening would reach into the address half. That is the existing
 * behaviour, not a change, and it is named here because the macro hides a
 * shift the open-coded form showed. */

#ifndef SDF_DMA_TAG

#endif

/* The same tag with the operands in the other order, for the two sites whose
 * source order the compiler reproduces: both pass a (u32) control word, and
 * writing the address first moves the zero extension and changes the
 * instructions. Same value, same layout, order preserved. The word keeps
 * whatever type the caller gives it rather than being widened here, because
 * that is what those two sites do. */

#ifndef SDF_DMA_TAG_WORD_FIRST

#endif

/* A cross-thread flag another thread writes, polled for lack of a scheduler.
 * addr keys a host wait and wake table; cast to void so the target still
 * type-checks it against the flag SDF_FLAG_SET writes. A host redefinition
 * must wake under the wait's lock and publish the value first; SET does both.
 * Writes that run before any thread exists stay plain assignments, because
 * there is no consumer to signal and a wake there would be meaningless rather
 * than wrong. A consumer must not assume SDF_FLAG_SET can only run once its
 * own wait table exists: nothing here guarantees that ordering. */

#ifndef SDF_FLAG_WAIT

#endif

#ifndef SDF_FLAG_SET

#endif

/* The EE's plain physical addressing, which is what a DMA engine reads: the
 * low 28 bits name RAM and the top nibble is the cache mode. Off target there
 * is no cache mode and no 28-bit RAM, so the address passes through. */

#ifndef EE_PHYSICAL

#endif

/* Forces an address into the EE's uncached mirror by setting one of the top
 * two address bits, for a DMA engine or coprocessor reading memory the CPU
 * just wrote: EE_UNCACHED (bit 29) for an ordinary buffer or struct,
 * EE_UNCACHED_RING (bit 28) for a DMA ring address. On a host build there is
 * no cache to bypass and no mirror to land in, so both default to the plain
 * address. */

#ifndef EE_UNCACHED

#endif

#ifndef EE_UNCACHED_RING

#endif

/* The EE hardware register space and the GS privileged register space, as
 * address ranges rather than individual registers: MovieViewer.c's debug
 * watcher classifies an arbitrary address by whether it falls in one of
 * them. A build with no mapped register range defines these so the test
 * never matches. */

#ifndef EE_REG_RANGE_BASE

#endif

/* Register access: a plain volatile access, unless the build supplies its
 * own. */

#ifndef HW_READ

#endif

/* A register reached as a struct's member (a DMA channel's chcr through
 * sceDmaGetChan) is named as the member itself, since taking its address
 * changes ee-gcc's code: read, its bits set, its bits cleared. A plain
 * access, unless the build supplies its own. */

#ifndef HW_FIELD_READ

#endif

#endif /* DDS_EEREGS_H */
