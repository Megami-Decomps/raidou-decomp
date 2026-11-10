#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetDrawDefParmPacket(sdfGrRefGifPacketHeader_t *aDef) {
    aDef->dmatag.data = 1;
    aDef->dmatag.reg = SDF_GS_PRMODECONT;
    aDef->giftag.data = 1;
    aDef->giftag.reg = SDF_GS_COLCLAMP;
    aDef->reftag.data = 0;
    aDef->reftag.reg = SDF_GS_DTHE;
    aDef->linktag.data = SDF_GS_DEFAULT_TEXA;
    aDef->linktag.reg = SDF_GS_TEXA;
}
