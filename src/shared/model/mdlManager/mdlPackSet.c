#include "shared/model/mdlManager/decls.h"

sdfPacDecodeResrc_t *mdlPackSet(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags) {
    mdlSetDef_t groupSetup;
    mdlPartTagRecord_t partTagRecord;
    sdfPacStdBlockHeader_t *packetHeader;
    mdlPibRecord_t *record;
    mdlPibRecord_t *slotRecord;
    s32 partCount;
    sdfItemArray_t *partList;
    s32 slotCount;
    s32 objectSize;
    s32 remainingParts;
    s32 payloadValue;

    if (mdlPackCheck(resrc) == 0) {
        return mdlPackSetOld(resrc, major, minor, regFlags);
    }
    record = (mdlPibRecord_t *)resrc->data;
    payloadValue = mdlPibGetVersion(record);
    if (payloadValue <= 0) {
        return mdlPackSetOld(resrc, major, minor, regFlags);
    }

    memset(&groupSetup, 0, sizeof(groupSetup));
    groupSetup.effectData = record;
    groupSetup.effectMemHandle = (sdfMemHandle_t *)resrc->resrc;
    resrc = sdfPacFreeResrc(resrc);
    partCount = mdlPibGetEffectCount(record);
    remainingParts = partCount;
    if (partCount > 0) {
        partList = mdlEffectResrcListNew(partCount);
        groupSetup.effectResrcList = partList;
    }
    if (remainingParts > 0) {
        do {
            switch (((sdfPacStdBlockHeader_t *)resrc->header)->magicCode) {
            case PIB_TAG_P2A:
                mdlCreateEffectResrc_P2A(partList, resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            case PIB_TAG_D3P:
                mdlCreateEffectResrc_D3P(partList, (struct mdlEffectDescriptor_t *)resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            case PIB_TAG_IPU:
                objectSize = ((sdfPacStdBlockHeader_t *)resrc->header)->realSize;
                if (objectSize == 0) {
                    objectSize = ((sdfPacStdBlockHeader_t *)resrc->header)->size
                               + (((sdfPacStdBlockHeader_t *)resrc->header)->flags & 0xF0) - 0x10;
                }
                mdlCreateEffectResrc_IPU(partList, resrc->data,
                                    (sdfMemHandle_t *)resrc->resrc, objectSize);
                break;
            default:
                /* Retained metadata stores: SMT3 1DE078/1DE07C and
                 * DDS1 2188E0/2188E4 (same otherwise-unused local record). */
                packetHeader = (sdfPacStdBlockHeader_t *)resrc->header;
                partTagRecord.tag = packetHeader->magicCode;
                partTagRecord.state = 0;
                break;
            }
            resrc = sdfPacFreeResrc(resrc);
            remainingParts--;
        } while (remainingParts != 0);
    }

    if (((sdfPacStdBlockHeader_t *)resrc->header)->fileType == DDS3_FT_TEXLIST) {
        groupSetup.texList = (sdfModelTexList_t *)resrc->resrc;
        resrc = sdfPacFreeResrc(resrc);
    }
    groupSetup.modelDefHandle = (sdfMemHandle_t *)resrc->resrc;
    groupSetup.modelDefPtr = (sdfModelDef_t *)resrc->data;
    resrc = sdfPacFreeResrc(resrc);
    mdlRegistModelSet(major, minor, regFlags, &groupSetup);

    slotCount = mdlPibGetMotionCount(record);
    while (slotCount > 0) {
        slotRecord = (mdlPibRecord_t *)(resrc->data - (u32)&((mdlPibRecord_t *)0)->tail);
        mdlRegistMotion(major, minor, regFlags, (s16)slotRecord->parameter.word,
                                      slotRecord->payload.stream.selectorA,
                                      slotRecord->payload.stream.selectorB,
                                      (sdfMotionPlayBook_t *)resrc->data,
                                      (sdfMemHandle_t *)resrc->resrc);
        resrc = sdfPacFreeResrc(resrc);
        slotCount--;
    }
    return resrc;
}
