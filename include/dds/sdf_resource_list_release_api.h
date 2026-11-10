#ifndef DDS_SDF_RESOURCE_LIST_RELEASE_API_H

#define DDS_SDF_RESOURCE_LIST_RELEASE_API_H
#include "common.h"

struct DevRequest;

typedef s32 SdfTexListItemReferencePolicy;

enum {
    SDF_TEX_LIST_KEEP_REFERENCES = 0,
    SDF_TEX_LIST_RELEASE_REFERENCES = 1
};

#endif /* DDS_SDF_RESOURCE_LIST_RELEASE_API_H */
