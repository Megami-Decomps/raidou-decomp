#ifndef SDFPACDECODE_H
#define SDFPACDECODE_H

#include "common.h"
#include "libsdf.h"

s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length);
void sdfPacFree(sdfPacDecodeCb_t *cb);
sdfPacDecodeResrc_t *sdfPacFreeResrc(sdfPacDecodeResrc_t *resrc);
void sdfPacInit(sdfPacDecodeCb_t *cb, sdfPacCallbackFunc callback);
void sdfPacSetFromTail(sdfPacDecodeCb_t *cb);
void sdfPacSetStay(sdfPacDecodeCb_t *cb);

#endif
