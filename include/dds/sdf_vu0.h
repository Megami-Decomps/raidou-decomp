#ifndef DDS_SDF_VU0_H

#define DDS_SDF_VU0_H
#include "../ps2asm.h"
#include "common.h"

/* VU0 and MMI operations under the names SMT3HD keeps for them: the
 * transpiler could not inline the engine's macros, so its metadata has
 * each as a function (SDF_VU0__NORMALIZE) and smt3-decomp's
 * include/ps2asm.h adopted those names. One macro per concrete operation,
 * every register literal in the body, which is the form the engine used.
 * Bodies here are byte-identical to the register-parameterised macros in
 * pcp_vu0.h they replaced. */

/* An sq offset is a literal, so the unrolled clear needs one name per step. */

/* Additional working-register operations used by the trail interpolator. */

/* The extended form, as the trail builder spells it. */

/* vf10 = matrix (vf28-vf31) * vf10: the core of libvu0 sceVu0ApplyMatrix.
 * _POINT takes the source w as 1; _ROTATE uses the 3x3 part only. */

/* Ring geometry matrix and working-register operations. */

/* Accumulate the xyz clip tests of a transformed vector against its own w,
 * then read or reset the accumulated flags in vi18. */

/* ---- VU0 macro-mode arithmetic on the fixed working pair ---- */

/* The extended form: an empty operand list, not a basic asm with no colons.
 * cc1 2.96 schedules the two differently, and mdlSetBlur only matches with
 * this spelling. */

/* vf12 is the spare the effect code parks a vector in across a call. */

/* out = vf10 . vf11 over xyz as a C float; $2 is scratch. */

/* An identity matrix in the bank: vmr32 rotates the one in vf0.w into
 * place for each row. */

/* vf10 = vf0, so xyz go to zero and w to one. */

/* The _TO/_FROM forms take a C lvalue and let cc1 pick the addressing mode;
 * the plain forms take an address already in a register. */

/* Cell gradient vector operations. */

/* ---- fixed-register forms, one name per register pair ---- */

/* vf10.xyz = -vf10.xyz (vf0.xyz is zero), vf10.w = 0, vf10.w = 1. */

/* vf10 = vf10 x vf11 over xyz: the outer-product pair. */

/* ---- operations that were written out at a call site ---- */

/* from effect/parManager.c */

/* SDK input-address-only store: audited VU pipelines only. Unlike the
 * normal store it does not invalidate values cached from unrelated memory. */

/* from code_001DEA58.c */

/* from basic/dds3EffectObjectBasic.c */

/* vf12 = -vf11, taking the long way: the accumulator is zeroed by vf0.x and
 * vf11 is then subtracted from it. */

/* vf10 . vf11 over all four lanes, read back as a C float. vf1.xyz is set to
 * one first because vf0.x is a hardwired zero and cannot carry the y and z
 * terms. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* vf10 = vf10 * vf2.x + vf12 * vf3.x. */

#endif
