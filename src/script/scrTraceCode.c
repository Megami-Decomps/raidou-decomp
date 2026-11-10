#include "common.h"

/* Declarations from smt3-decomp src/smt3/script/scrTraceCode.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "newdata.h"

#include "scr.h"

enum code_operation
{
    CODE_OPERATION_ADD = 0,
    CODE_OPERATION_SUB = 1,
    CODE_OPERATION_MUL = 2,
    CODE_OPERATION_DIV = 3,
    CODE_OPERATION_OR = 4,
    CODE_OPERATION_AND = 5,
    CODE_OPERATION_EQ = 6,
    CODE_OPERATION_NEQ = 7,
    CODE_OPERATION_S = 8,
    CODE_OPERATION_L = 9,
    CODE_OPERATION_SE = 10,
    CODE_OPERATION_LE = 11,
    CODE_OPERATION_MAX = 12
};

extern scrProcessWork_s *nowpwork;

extern const scrCommandTable_t scrCommandTable[];

extern s32 (*codefunc[])(scrProcessWork_s *pw);

extern s32 PopInt(scrProcessWork_s *pwork);

extern f32 PopFloat(scrProcessWork_s *pwork);

extern void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type);

void PushInt(scrProcessWork_s *pwork, s32 data);

void PushFloat(scrProcessWork_s *pw, f32 value);

void PushStr(scrProcessWork_s *pwork, u8 *data);

void PushRet(scrProcessWork_s *pwork, s32 cp);

s32 PopInt(scrProcessWork_s *pwork);

f32 PopFloat(scrProcessWork_s *pwork);

s32 CodeFunc_PushI(scrProcessWork_s *pwork);

s32 CodeFunc_PushIS(scrProcessWork_s *pwork);

s32 CodeFunc_PushF(scrProcessWork_s *pwork);

s32 CodeFunc_PushIX(scrProcessWork_s *pwork);

s32 CodeFunc_PushFX(scrProcessWork_s *pwork);

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork);

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork);

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork);

s32 CodeFunc_PushReg(scrProcessWork_s *pwork);

s32 CodeFunc_PopIX(scrProcessWork_s *pwork);

s32 CodeFunc_PopFX(scrProcessWork_s *pwork);

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork);

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork);

s32 CodeFunc_Proc(scrProcessWork_s *pwork);

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork);

s32 CodeFunc_End(scrProcessWork_s *pwork);

s32 CodeFunc_Jump(scrProcessWork_s *pwork);

s32 CodeFunc_Call(scrProcessWork_s *pwork);

s32 CodeFunc_Run(scrProcessWork_s *pwork);

s32 CodeFunc_Goto(scrProcessWork_s *pwork);

extern s32 CodeFunc_2Operation_i1;

extern s32 CodeFunc_2Operation_i2;

extern s32 CodeFunc_2Operation_ri;

extern f32 CodeFunc_2Operation_f1;

extern f32 CodeFunc_2Operation_f2;

extern f32 CodeFunc_2Operation_rf;

extern s32 CodeFunc_2Operation_t1;

extern s32 CodeFunc_2Operation_t2;

void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type);

s32 CodeFunc_Add(scrProcessWork_s *pwork);

s32 CodeFunc_Sub(scrProcessWork_s *pwork);

s32 CodeFunc_Mul(scrProcessWork_s *pwork);

s32 CodeFunc_Div(scrProcessWork_s *pwork);

s32 CodeFunc_Minus(scrProcessWork_s *pwork);

s32 CodeFunc_Not(scrProcessWork_s *pwork);

s32 CodeFunc_Or(scrProcessWork_s *pwork);

s32 CodeFunc_And(scrProcessWork_s *pwork);

s32 CodeFunc_EQ(scrProcessWork_s *pwork);

s32 CodeFunc_NEQ(scrProcessWork_s *pwork);

s32 CodeFunc_S(scrProcessWork_s *pwork);

s32 CodeFunc_L(scrProcessWork_s *pwork);

s32 CodeFunc_SE(scrProcessWork_s *pwork);

s32 CodeFunc_LE(scrProcessWork_s *pwork);

s32 CodeFunc_IF(scrProcessWork_s *pwork);

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork);

s32 scrGetIntPara(s32 n);

f32 scrGetFloatPara(s32 n);

u8 *scrGetStrPara(s32 n);

void scrSetIntReturnValue(s32 i);

void scrSetFloatReturnValue(f32 f);

s32 scrGetProcedureCodeAddress(s32 procindex);

s32 scrGetLabelCodeAddress(s32 labelindex);

s32 scrGetCodePointer(void);

void scrSetCodePointer(s32 cp);

s32 scrGetScriptTimer(void);

s32 scrGetCommandTimer(void);

s32 scrGetMessageHandle(void);

scrProcessWork_s *scrGetScriptHandle(void);

void *scrGetData(void);
/* End of declarations from src/smt3/script/scrTraceCode.c */

void PushInt(scrProcessWork_s *pwork, s32 data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_INT;
    pwork->StackData[pwork->SP].i = data;
    pwork->SP++;
}

void PushFloat(scrProcessWork_s *pw, f32 value)
{
    pw->StackType[pw->SP] = SCR_STACK_TYPE_FLOAT;
    pw->StackData[pw->SP].f = value;
    pw->SP++;
}

void PushStr(scrProcessWork_s *pwork, u8 *data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_STRING;
    pwork->StackData[pwork->SP].i = EE_ADDR_S(data);
    pwork->SP++;
}

void PushRet(scrProcessWork_s *pwork, s32 cp)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_RET;
    pwork->StackData[pwork->SP].i = cp;
    pwork->SP++;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", PopInt);

INCLUDE_ASM(const s32, "script/scrTraceCode", PopFloat);

s32 CodeFunc_PushI(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushInt(pwork, pwork->Code[pwork->CP].i);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushIS(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushF(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushFloat(pwork, pwork->Code[pwork->CP].f);
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PushIX);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PushFX);

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->LocalInt[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, pwork->LocalFloat[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork)
{
    PushStr(pwork, pwork->StringData + pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushReg(scrProcessWork_s *pwork)
{
    pwork->StackType[pwork->SP] = pwork->StackType[27];
    pwork->StackData[pwork->SP].i = pwork->StackData[27].i;
    pwork->SP++;
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PopIX);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PopFX);

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork)
{
    pwork->LocalInt[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork)
{
    pwork->LocalFloat[pwork->Code[pwork->CP].s.s1] = PopFloat(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Proc(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork)
{
    s32 commandCP = pwork->CP;
    s32 commandIndex = pwork->Code[commandCP].s.s1;

    nowpwork = pwork;
    if (scrCommandTable[commandIndex].comm() == 0) {
        return CODE_RET_SYNC;
    }
    pwork->SP -= scrCommandTable[commandIndex].paranum;
    if (commandCP == pwork->CP) {
        pwork->CP = commandCP + 1;
    }
    return CODE_RET_NEXT;
}

s32 CodeFunc_End(scrProcessWork_s *pwork)
{
    if (pwork->SP == 0) {
        return CODE_RET_END;
    }

    pwork->CP = PopInt(pwork) + 1;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Jump(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Call(scrProcessWork_s *pwork)
{
    PushRet(pwork, pwork->CP);
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Run(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Goto(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_2Operation);

s32 CodeFunc_Add(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_ADD);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Sub(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SUB);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Mul(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_MUL);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Div(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_DIV);
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_Minus);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_Not);

s32 CodeFunc_Or(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_OR);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_And(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_AND);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_EQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_EQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_NEQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_NEQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_S(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_S);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_L(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_L);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_SE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_LE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_LE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_IF);

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork)
{
    s32 result;

    while (1) {
        result = codefunc[pwork->Code[pwork->CP].s.s0](pwork);
        if (result == CODE_RET_END) {
            pwork->CommandTimer = 0;
            return SCR_RESULT_END;
        }
        if (result == CODE_RET_SYNC) {
            break;
        }
        if (result == CODE_RET_NEXT) {
            pwork->CommandTimer = 0;
        }
    }
    pwork->CommandTimer++;
    pwork->ScriptTimer++;
    return SCR_RESULT_CONTINUE;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", scrGetIntPara);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrGetFloatPara);

u8 *scrGetStrPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;
    s32 stackType = (s8)pw->StackType[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return EE_ADDR_AS(u8 *, pw->StackData[stackIndex].i);
            }
        }
    }
    return NULL;
}

void scrSetIntReturnValue(s32 i)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_INT;
    nowpwork->StackData[27].i = i;
}

void scrSetFloatReturnValue(f32 f)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_FLOAT;
    nowpwork->StackData[27].f = f;
}

s32 scrGetProcedureCodeAddress(s32 procindex)
{
    return nowpwork->ProcTable[procindex].CodeAddress;
}

s32 scrGetLabelCodeAddress(s32 labelindex)
{
    return nowpwork->LabelTable[labelindex].CodeAddress;
}
