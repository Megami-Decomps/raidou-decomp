#include "shared/model/mdlManager/decls.h"

saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode) {
    char requestPath[0x80];
    dds3ModelResrc_t *entity;
    dds3ModelFileIndex_t *selection;
    dds3ModelFileTable_t *paths;
    mdlManager_mdlTbMbAbLoadCb_t *request;
    char *extension;
    s32 packedBundle;

    WaitSema(mdlManagerWork);
    entity = mdlSearchResrc(aMajor, aMinor);
    if (entity != NULL) {
        SignalSema(mdlManagerWork);
        return EE_ADDR_S(entity);
    }
    if (mdlCheckLoading(aMajor, aMinor)) {
        SignalSema(mdlManagerWork);
        if (aBlockMode != 0) {
            return EE_ADDR_S(mdlWaitLoadEnd(aMajor, aMinor));
        }
        return -1;
    }
    selection = (dds3ModelFileIndex_t *)modelFileKindTable[aMajor].entries;
    selection += aMinor;
    paths = &((dds3ModelFileTable_t *)modelFileTable[selection->major].entries)[selection->minor];
    packedBundle = 0;
    extension = strrchr(paths->modelFile, '.');
    if (extension != NULL) {
        packedBundle = strcasecmp(extension, mdlPackedBundleExt) == 0;
    }
    mdlRegLoading(aMajor, aMinor);
    SignalSema(mdlManagerWork);
    if (packedBundle != 0) {
        mdlSetFileName(requestPath, paths->modelFile);
        mdlFileLoad(aMajor, aMinor, 0x101, requestPath,
                                  (void (*)(u32))sdfDummyFunc, 0);
    } else {
        request = sdfChipAllocZeroDirect(sizeof(*request));
        request->options = 0x101;
        request->group = aMajor;
        request->id = aMinor;
        if (paths->motionFile != NULL) {
            request->deferred = 1;
        }
        if (paths->texFile != NULL) {
            request->resourceListRequested = 1;
            mdlSetFileName(requestPath, paths->texFile);
            fileLoadEx(requestPath, 0, EE_ADDR(TbLoadCallback), EE_ADDR(request));
        }
        request->itemsRequested = 1;
        mdlSetFileName(requestPath, paths->modelFile);
        fileLoadEx(requestPath, 0, EE_ADDR(MbLoadCallback), EE_ADDR(request));
        if (paths->motionFile != NULL) {
            mdlSetFileName(requestPath, paths->motionFile);
            fileLoadEx(requestPath, 0, EE_ADDR(AbLoadCallback), EE_ADDR(request));
        }
    }
    if (aBlockMode != 0) {
        return EE_ADDR_S(mdlWaitLoadEnd(aMajor, aMinor));
    }
    return 0;
}
