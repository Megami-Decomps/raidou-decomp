#ifndef DDS_MODEL_H

#define DDS_MODEL_H
#include "../billboard.h"
#include "../model.h"
/* sdfModel: the node tree a loaded model file becomes, its per-frame draw
 * packet chains, and the four ordering tables they are filed into.
 * See include/README.md for the evidence per struct. */

#include "common.h"

#include "libsdf.h"

/* motion.h; a root owns a chain of controllers through motion. */
struct sdfMotionCtrlr_t;

/* Declared ahead so sdfModelNodeDef_t can point at it. */
struct sdfShapeHeader_t;

/* The five lists below are all sdfItemArray_t (libsdf.h); each repeats the
 * header so items carries its element type. elemSize is 4 for the three
 * pointer lists and 16 for the morph list. */

/* Shape headers. The engine's own name for these is not in either source, so
 * the three struct names below are this repo's; the field names follow the HD
 * metadata's sdfShapeClass, which merges the header and one packet. */

struct mdlEffectResrcItem_t;

struct mdlMarkPos_t;

struct mdlEffectResrc_IPU_t;

struct mdlTrackWork_t;

#endif /* DDS_MODEL_H */
