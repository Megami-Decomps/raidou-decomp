#include "common.h"

#include "shared/script/scrTraceCode/PushInt.c"

#include "shared/script/scrTraceCode/PushFloat.c"

#include "shared/script/scrTraceCode/PushStr.c"

#include "shared/script/scrTraceCode/PushRet.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", PopInt);

INCLUDE_ASM(const s32, "script/scrTraceCode", PopFloat);

#include "shared/script/scrTraceCode/CodeFunc_PushI.c"

#include "shared/script/scrTraceCode/CodeFunc_PushIS.c"

#include "shared/script/scrTraceCode/CodeFunc_PushF.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PushIX);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PushFX);

#include "shared/script/scrTraceCode/CodeFunc_PushLIX.c"

#include "shared/script/scrTraceCode/CodeFunc_PushLFX.c"

#include "shared/script/scrTraceCode/CodeFunc_PushSTR.c"

#include "shared/script/scrTraceCode/CodeFunc_PushReg.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PopIX);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PopFX);

#include "shared/script/scrTraceCode/CodeFunc_PopLIX.c"

#include "shared/script/scrTraceCode/CodeFunc_PopLFX.c"

#include "shared/script/scrTraceCode/CodeFunc_Proc.c"

#include "shared/script/scrTraceCode/CodeFunc_Comm.c"

#include "shared/script/scrTraceCode/CodeFunc_End.c"

#include "shared/script/scrTraceCode/CodeFunc_Jump.c"

#include "shared/script/scrTraceCode/CodeFunc_Call.c"

#include "shared/script/scrTraceCode/CodeFunc_Run.c"

#include "shared/script/scrTraceCode/CodeFunc_Goto.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_2Operation);

#include "shared/script/scrTraceCode/CodeFunc_Add.c"

#include "shared/script/scrTraceCode/CodeFunc_Sub.c"

#include "shared/script/scrTraceCode/CodeFunc_Mul.c"

#include "shared/script/scrTraceCode/CodeFunc_Div.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_Minus);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_Not);

#include "shared/script/scrTraceCode/CodeFunc_Or.c"

#include "shared/script/scrTraceCode/CodeFunc_And.c"

#include "shared/script/scrTraceCode/CodeFunc_EQ.c"

#include "shared/script/scrTraceCode/CodeFunc_NEQ.c"

#include "shared/script/scrTraceCode/CodeFunc_S.c"

#include "shared/script/scrTraceCode/CodeFunc_L.c"

#include "shared/script/scrTraceCode/CodeFunc_SE.c"

#include "shared/script/scrTraceCode/CodeFunc_LE.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_IF);

#include "shared/script/scrTraceCode/scrTraceCode.c"

INCLUDE_ASM(const s32, "script/scrTraceCode", scrGetIntPara);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrGetFloatPara);

#include "shared/script/scrTraceCode/scrGetStrPara.c"

#include "shared/script/scrTraceCode/scrSetIntReturnValue.c"

#include "shared/script/scrTraceCode/scrSetFloatReturnValue.c"

#include "shared/script/scrTraceCode/scrGetProcedureCodeAddress.c"

#include "shared/script/scrTraceCode/scrGetLabelCodeAddress.c"
