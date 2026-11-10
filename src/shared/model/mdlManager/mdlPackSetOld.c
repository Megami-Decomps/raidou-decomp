#include "shared/model/mdlManager/decls.h"

sdfPacDecodeResrc_t *mdlPackSetOld(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags) {
    mdlSetDef_t setup;
    sdfItemArray_t *parts;
    s32 count;

    memset(&setup, 0, sizeof(setup));
    parts = NULL;
    while (resrc != NULL) {
        switch (((sdfPacStdBlockHeader_t *)resrc->header)->fileType) {
        case DDS3_FT_EFFECT:
            switch (((sdfPacStdBlockHeader_t *)resrc->header)->magicCode) {
            case PIB_TAG_PIB0:
                setup.effectData = resrc->data;
                setup.effectMemHandle = (sdfMemHandle_t *)resrc->resrc;
                count = mdlPibGetEffectCount((mdlPibRecord_t *)resrc->data);
                if (count > 0) {
                    parts = mdlEffectResrcListNew(count);
                    setup.effectResrcList = parts;
                }
                break;
            case PIB_TAG_P2A:
                mdlCreateEffectResrc_P2A(parts, resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            case PIB_TAG_D3P:
                mdlCreateEffectResrc_D3P(parts, (struct mdlEffectDescriptor_t *)resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            }
            break;
        case DDS3_FT_TEXLIST:
            setup.texList = (sdfModelTexList_t *)resrc->resrc;
            break;
        case DDS3_FT_MODEL:
            if (setup.modelDefHandle == NULL) {
                setup.modelDefHandle = (sdfMemHandle_t *)resrc->resrc;
                setup.modelDefPtr = (sdfModelDef_t *)resrc->data;
            }
            break;
        case DDS3_FT_MOTION:
            if (setup.playbookHandle == NULL) {
                setup.playbookHandle = (sdfMemHandle_t *)resrc->resrc;
                setup.playbookPtr = (sdfMotionPlayBook_t *)resrc->data;
            }
            break;
        }
        resrc = sdfPacFreeResrc(resrc);
    }
    mdlRegistModelSet(major, minor, regFlags, &setup);
    return resrc;
}
