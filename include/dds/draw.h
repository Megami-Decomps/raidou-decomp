#ifndef DDS_DRAW_H

#define DDS_DRAW_H
#include "../billboard.h"
#include "../draw.h"
/* The GS side of the engine: the frame buffers, the three-slot draw queue, the
 * draw environment, and the A+D register packets the sdfPrim and sdf2D
 * builders file into an ordering table.
 *
 * sdfGrPacketHeader_t (libsdf.h) is every packet's first 32 bytes:
 * sdfGrSetRegsPacketHeader writes the DMA tag's qwc and VIF DIRECT code and
 * then the GIF tag, with NLOOP as the register count and REGS as A+D.
 * See include/README.md for the evidence per struct. */

#include "common.h"

#include "libsdf.h"

#endif /* DDS_DRAW_H */
