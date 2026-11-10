#ifndef DDS_BILL_OBJECT_API_H

#define DDS_BILL_OBJECT_API_H
#include "common.h"

struct billHandle_t;

struct BillData;

struct BillOut;

struct BillChildPayload;

struct BillSnapshot;

struct sdfTexHandle_t;

struct SdfDrawPacket;

struct SdfTextureFileHeader;

struct BlurPacketQuad;

void billReleaseSharedEntryBlock(struct BillData *data);

void effReleaseSharedTextureRecord(struct BillChildPayload *child);

void billCopyCurrentRecordToSnapshot(
    struct billHandle_t *billboard, struct BillSnapshot *snapshot);

#endif
