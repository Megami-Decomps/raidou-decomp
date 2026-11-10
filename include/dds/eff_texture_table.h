#ifndef DDS_EFF_TEXTURE_TABLE_H

#define DDS_EFF_TEXTURE_TABLE_H
struct sdfTexHandle_t;

/* Model asset-data view shared by the DDS1 and DDS2 texture upload paths. */
typedef struct EffTexTable {
    u8 opaquePrefix[0x0C];
    struct sdfTexHandle_t **entries;
} EffTexTable;

#endif
