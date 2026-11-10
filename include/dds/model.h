#ifndef DDS_MODEL_H

#define DDS_MODEL_H
#include "../billboard.h"
#include "../model.h"
/* sdfModel: the node tree a loaded model file becomes, its per-frame draw
 * packet chains, and the four ordering tables they are filed into.
 * See include/README.md for the evidence per struct. */

#include "common.h"

#include "libsdf.h"

typedef struct dds_sdfModelRootNode_t dds_sdfModelRootNode_t;

/* motion.h; a root owns a chain of controllers through motion. */
struct sdfMotionCtrlr_t;

/* Declared ahead so sdfModelNodeDef_t can point at it. */
struct sdfShapeHeader_t;

/* The five lists below are all sdfItemArray_t (libsdf.h); each repeats the
 * header so items carries its element type. elemSize is 4 for the three
 * pointer lists and 16 for the morph list. */

/* One loaded model. sdfModelRootNew allocates 152 bytes; the HD build's
 * modelobj, oData, effpos_type and reversePB past zBias do not exist here. */
struct dds_sdfModelRootNode_t
{
    sdfModelNodeList_t *nodeList;
    sdfModelNode_t *headNode;
    sdfModelTexList_t *texList;
    sdfMatList_t *matList;
    sdfModelMorphList_t *morphList;
    struct sdfMotionCtrlr_t *motion;
    u8 complexNodeId; /* unconfirmed; the mode itself is flags bit 0 */
    u8 flags;
    u16 drawAttr;
    u32 color;
    sdfMatrix4_t tm;
    sdfVector4_t rotate;
    sdfVector4_t scale;
    struct sdf3DLightParallelPacket_t *light;
    struct sdf3DFogPacket_t *fog;
    f32 faceDivSoften;
    f32 faceDivBias;
    void *extraData; /* the file's material def block, per sdfModelMakeRoot */
    f32 zBias;
    u8 lod; /* sdfModelLinkOt draws only packets whose otIndex high nibble equals it */
    u8 pad99[3];
};

/* Shape headers. The engine's own name for these is not in either source, so
 * the three struct names below are this repo's; the field names follow the HD
 * metadata's sdfShapeClass, which merges the header and one packet. */

struct mdlEffectResrcItem_t;

struct mdlMarkPos_t;

struct mdlEffectResrc_IPU_t;

struct mdlTrackWork_t;

#endif /* DDS_MODEL_H */
