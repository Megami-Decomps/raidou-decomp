#include "shared/interface/frFont/decls.h"

void frCreateDrawEnvPacket(frFontGsPacket_t *pDrawEnvPkt) {
    QUAD_ZERO(pDrawEnvPkt->dma.value);
    pDrawEnvPkt->dma.words[0] = 0x70000007;
    pDrawEnvPkt->dma.words[2] = 0;
    pDrawEnvPkt->dma.words[3] = 0x50000007;
    pDrawEnvPkt->gifTag = 0x1000000000008006ULL;
    pDrawEnvPkt->gifRegisters = 0xE;
    pDrawEnvPkt->writes[0].value = 0;
    pDrawEnvPkt->writes[0].reg = 0x3F;
    pDrawEnvPkt->writes[1].value = 0x44;
    pDrawEnvPkt->writes[1].reg = 0x42;
    pDrawEnvPkt->writes[2].value = 0;
    pDrawEnvPkt->writes[2].reg = 0x49;
    pDrawEnvPkt->writes[3].value = 0x8080;
    pDrawEnvPkt->writes[3].reg = 0x3B;
    pDrawEnvPkt->writes[4].value = 0;
    pDrawEnvPkt->writes[4].reg = 0x4A;
    pDrawEnvPkt->writes[5].value = 0x517ED;
    pDrawEnvPkt->writes[5].reg = 0x47;
}
