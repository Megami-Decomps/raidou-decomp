#include "shared/script/scrTraceCode/decls.h"

void PushStr(scrProcessWork_s *pwork, u8 *data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_STRING;
    pwork->StackData[pwork->SP].i = EE_ADDR_S(data);
    pwork->SP++;
}
