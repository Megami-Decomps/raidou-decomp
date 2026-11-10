/* Declarations of smt3-decomp src/smt3/script/scrTraceCode.c, for its bodies in this directory. */
#ifndef SHARED_SCRIPT_SCRTRACECODE_H
#define SHARED_SCRIPT_SCRTRACECODE_H

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

#endif
