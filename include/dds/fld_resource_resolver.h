#ifndef DDS_FLD_RESOURCE_RESOLVER_H

#define DDS_FLD_RESOURCE_RESOLVER_H
#include "common.h"

struct dds3Basic_t;

/* Return the first matching record's cached node, or NULL if no binding is found. */
struct dds3Basic_t *fldFindWorldNodeForRecordId(s32 recordId);

/* Bind the node to every collision record with this ID. */
void fldBindWorldNodeToRecordId(s32 recordId, struct dds3Basic_t *worldNode);

#endif
