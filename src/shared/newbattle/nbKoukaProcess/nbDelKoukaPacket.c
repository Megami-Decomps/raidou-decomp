#include "shared/newbattle/nbKoukaProcess/decls.h"

void nbDelKoukaPacket(nbKoukaPacket_t *p)
{
    DelKoukaPacket(nbGetKoukaProcessData(), p);
}
