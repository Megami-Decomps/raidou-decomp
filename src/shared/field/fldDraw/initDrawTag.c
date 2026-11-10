#include "shared/field/fldDraw/decls.h"

void initDrawTag(dbgConsCtl_tag *cons) {
    s32 packet;
    sdfDrawTag_t *packetList;

    packetList = dds3FontReqAllocDrawTag();
    cons->tag = packetList;
    packet = sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(EE_ADDR_AS(sdfPrimTransPacket_t *, packet));
    sdfDrawTagLink(cons->tag, packet);
}
