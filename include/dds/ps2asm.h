#ifndef DDS_PS2ASM_H

#define DDS_PS2ASM_H
#include "../ps2asm.h"
#include "common.h"

/* EE instructions ee-gcc 2.96 never selects from C: VU0 macro-mode COP2, the
 * MMI 128-bit integer ops, and the pipeline-1 integer multiply/divide.
 *
 * No macro parameter reaches the assembler. Opcodes, suffixes and every
 * vf/vi register are literal inside the macro body; parameters are C
 * expressions bound through operand constraints and substituted as %0, %1.
 * One macro per concrete operation, named after what the operation does.
 *
 * The engine's VU0 convention fixes the register roles: vf10 is the working
 * vector, vf11 the second operand, vf2 the broadcast scratch and vf28-vf31
 * the matrix bank. See include/README.md for what cc1 does with each
 * sequence. */

/* ---- quadword memory ---- */

/* lq/sq through $2. Plain C `*(u128 *)dst = *(u128 *)src` gives the same pair
 * but lets the sq fall into a jr delay slot. */

/* sq $0: EE GCC materialises a C 128-bit zero in a register first. */

/* sq from a GPR: stores all 128 physical bits of the register, so a 64-bit
 * value carries whatever sits in its upper half (zero here, left by a load).
 * cc1's own 64-bit stores (sd) only ever write the low 64 bits. */

/* ---- quadword between memory and the VU0 register file ---- */

/* The _TO/_FROM forms take a C lvalue and let cc1 pick the addressing mode;
 * the plain forms take an address already in a register. */

/* Explicit, volatile four-f32 scratch storage at a 16-byte-aligned address.
 * As with the copy helper, address expressions must have no side effects. */

/* vf0 is the constant (0,0,0,1), so this clears xyz and sets w to one. */

/* Four quadwords between memory and the matrix bank vf28-vf31. */

/* Preserve the current transform in the secondary matrix bank. */

/* ---- GPR <-> VU0 register ---- */

/* qmtc2/qmfc2 with the .ni (no interlock) suffix the SDK inlines use. */

/* ---- scaling by a scalar broadcast through vf2.x ---- */

/* Broadcast a C float into vf2.x and scale. The GPR the bits travel through is
 * the compiler's, not a fixed scratch: two consecutive scales in one function
 * use two different registers in retail code. */

/* The same scale from a GPR that already holds the binary32 bits. */

/* ---- VU0 macro-mode arithmetic on the fixed working pair ---- */

/* vf12 is the spare the effect code parks a vector in across a call. */

/* vf10.xyz = -vf10.xyz (vf0.xyz is zero), vf10.w = 0, vf10.w = 1. */

/* Float <-> integer lane conversion with 0 and 4 fractional bits. */

/* vf10 = matrix (vf28-vf31) * vf10: the core of libvu0 sceVu0ApplyMatrix.
 * _POINT takes the source w as 1; _ROTATE uses the 3x3 part only. */

/* vf10 = vf10*(1 - t) + vf11*t. vf2 holds t broadcast and vf3.w holds 1 - t;
 * $2 carries the bits, and is deliberately not a clobber: the engine's own
 * sequence does not list it, and cc1 then picks $2 for a nearby address the
 * way retail does. The _BITS form takes a
 * GPR that already holds them, which is what retail emits when one blend
 * factor drives several lerps. */

/* binary32 bits of a float in a GPR (mfc1), for the _BITS forms. */

/* vf10 = vf10 x vf11 over xyz: the outer-product pair. */

/* out = vf10 . vf11 over xyz as a C float; $2 is scratch. */

/* out = |vf10.xyz|, read back through the Q register and $2. */

/* vf10 /= |vf10.xyz| (w untouched). */

/* vf10 /= vf10.w, then w = 1: the perspective divide after a point transform. */

/* Accumulate the xyz clip tests of a transformed vector against its own w,
 * then read or reset the accumulated flags in vi18. */

/* ---- MMI 128-bit integer ops ---- */

/* cc1 rejects C operators on a 128-bit type ("unsupported wide integer
 * operation"), so every lane op is written out. Operands are u128/s128. */

/* Store the unit matrix represented by VU0's constant vf0 using the retail
 * MMI lane-shuffle sequence. */

/* Leading sign bits minus one of a word; cc1 has no clz builtin. */

/* SDK bit length of a non-negative word: highest set bit index plus one.
 * This shared four-instruction idiom uses its native $2 scratch (also present
 * in sdfTexSetPacket), not a target-local register constraint. Passed dim - 1
 * it gives log2 of a power-of-two dimension. */

/* RGBA8888 word -> four float lanes in vf10: bytes to halfwords to words,
 * then vitof0. */

/* The SDK form, which reads the colour word back from its own 16-byte carrier
 * and scales the lanes by `unit`, binary32 bits in a GPR (0x3C000000 = 1/128).
 * $2 is the SDK's scratch and has to stay fixed: letting cc1 allocate it picks
 * a different register from retail. */

/* vf10 (floats) -> RGBA8888 word: scale through vf2x, vftoi0, then words to
 * halfwords to bytes. $3 carries the scale bits, as in the SDK sequence. */

/* The same pack with the scale already in a GPR. */

/* Three unaligned floats at p -> vf10 (w undefined), and the reverse. cc1's
 * own unaligned accesses emit ldl before ldr, and cannot extract a quadword's
 * upper half with a C shift. $2/$3 are scratch. */

/* ---- pipeline-1 integer multiply and divide ---- */

/* Use these for native pipeline-1 forms that ordinary C does not reproduce.
 * Some ordinary expressions already emit mult1; inspect compiler output first. */

/* ---- COP1 square root ---- */

/* The engine's own names. The HD sources keep both as real functions in
 * sdfMath.c, sdfSqrt as `return sqrtf(aValue);` and sdfFabs as
 * `return fabsf(c);`, so a call site here reads the way the original did.
 * Value forms, and not volatile: they have an output and no side effect. */

/* ---- barriers ---- */

/* The body of a busy-wait loop that reads no hardware: without a side effect
 * cc1 deletes the countdown. The loop's remaining nops are cc1's own
 * load-use padding, which it emits whether or not this one is there. */

/* Additional working-register operations used by the trail interpolator. */

/* SDK input-address-only store: audited VU pipelines only. Unlike the
 * normal store it does not invalidate values cached from unrelated memory. */

/* The load, the store and the quadword copy with the address as a read-write
 * operand, so cc1 gives each asm its own copy of it instead of sharing one
 * pseudo. Needed where several VU0 accesses to one frame vector sit inside a
 * loop: the shared pseudo becomes a loop invariant in a callee-saved register,
 * where retail rematerialises `addiu rd, $29, N` at every access. */

/* Cell gradient vector operations. */

/* Ring geometry matrix and working-register operations. */

/* Store XYZ from an aligned quadword into the packed VIF stream. */

/* ---- operations taken on a register named at the call site ---- */

/* The vf register is a literal token in the macro body, so one definition
 * covers every register the units use. Units that need a different operand
 * constraint for the same operation keep their own definition and say why. */

/* vf10 = vf0, so xyz go to zero and w to one. */

/* Same stores with the address as a read-write operand, so cc1 gives the asm
 * its own copy of it instead of sharing one pseudo with a later use. Needed
 * for a frame address whose other use is a call argument in the same loop:
 * the shared form becomes a loop invariant and takes a callee-saved register,
 * where this one is rematerialised at each use. */

/* Memory to memory through the matrix bank, which it leaves loaded. */

/* An identity matrix built in GPRs from vf0 and stored, with no VU0
 * arithmetic: pextuw walks the one in vf0.w down the four rows. */

/* The same unpack as MMI_RGBA_UNPACK with the operands spelled the way the
 * event and model units write them. */

/* ---- lane reads and writes through $2, and the single-register stores ---- */

/* vf10.w and vf10.z set from a C float, vf0 supplying the one. */

/* An identity matrix in the bank: vmr32 rotates the one in vf0.w into
 * place for each row. */

/* ---- fixed-register forms, one name per register pair ---- */

/* The extended form: an empty operand list, not a basic asm with no colons.
 * cc1 2.96 schedules the two differently, and mdlSetBlur only matches with
 * this spelling. */

/* Extended register-only forms used by the audited Bezier ribbon pipeline.
 * As in the matched DDS engine, empty operands retain ASM_OPERANDS boundaries
 * without adding instructions, artificial C dependencies or memory clobbers.
 * The existing basic forms and their consumers are unchanged. */

/* ---- operations that were written out at a call site ---- */

/* The extended form, as the trail builder spells it. */

/* An sq offset is a literal, so the unrolled clear needs one name per step. */

/* Pack an RGBA8888 word down to RGBA5551. */

/* The barrier forms: same instruction, but cc1 may not keep memory values
 * cached across them. */

/* vf0 is the hardware (0, 0, 0, 1) constant. Broadcasting its w into vf1.xyz
 * gives ones there, and subtracting it from itself zeros vf1.w. */

/* ---- quaternion operations shared by the blend routines ---- */

/* vf10 . vf11 over all four lanes, read back as a C float. vf1.xyz is set to
 * one first because vf0.x is a hardwired zero and cannot carry the y and z
 * terms. */

/* vf12 = -vf11, taking the long way: the accumulator is zeroed by vf0.x and
 * vf11 is then subtracted from it. */

/* The two blend weights into vf2.x and vf3.x. */

/* vf10 = vf10 * vf2.x + vf12 * vf3.x. */

/* ---- operations recovered in one unit each ---- */

/* from basic/dds3EffectObjectBasic.c */

/* Translate with an implicit homogeneous coordinate of one. */

/* from basic/dds3PathBasic.c */

/* from code_001AC0A8.c */

/* from code_001DEA58.c */

/* from newbattle/nbStageProcess.c */

/* from code_002B5CD0.c */

/* The three rotated basis rows saved across a nested packet build. */

/* Same triple, one quadword further in: src's first quadword is skipped. */

/* from code_002C6F38.c */
/* The wire-model clipper keeps the segment in vf10/vf11, its colours in
 * vf8/vf9, the viewport scale and offset in vf14/vf15 and the plane in vf2. */

/* The unpack with the colour word read through an input operand instead of a
 * memory clobber, so a surrounding loop can still hoist unrelated loads, and
 * with the source operand first: the hoisted address and the hoisted unit
 * constant come out in operand order. */

/* from effect/effPCPScatter.c */
/* Load-normalize-store of one packed vector in a single block ("+m" operand
 * with a memory clobber). */

/* from effect/parManager.c */

/* from kernel/dds3KernelDraw.c */
/* Concrete SDK register transfers used by the quaternion and projection paths.
 * VF2 is the scalar/swap scratch; C float transfers use the SDK's $2 scratch. */

/* Light and ambient colors scale by 128, and their scratch register is $3. */

/* The same pack by 128 through the $2 scratch, as event/evtCommand uses. */

/* from script/scrCommonCommand.c */

/* from sdf/sdfModel.c */

/* from sdf/sdfMotion.c */
/* SDK packed-key and VU interpolation sequences used by model channels. */

/* Retail repeats the first pointer and transfers the raw z word to vf3;
 * preserve these two SDK sequence defects (SMT3 0x002B4258, DDS1 twin). */

/* out = a + (b - a) * t per RGBA byte lane, rounded through vf3x. `half` is
 * 0.5f in a general register. */

/* The scalar reaches vf2.x through $2 rather than a register cc1 picks,
 * which is what these callers' retail allocation needs. */

/* Packed-palette tinting used by sdfTexSetClutColor and the DDS counterpart.
 * vf2x holds the blend amount, vf3.xyz the tint normalized by 255; the loop
 * primitives preserve source alpha in vf6w. Scalar C cannot emit the packed
 * byte expansion, VU accumulator blend, or PPAC5. Loops and palette addressing
 * remain C; these primitives cover only the native COP2/MMI conversion.
 */

/* ---- sequences that units wrote out in place ---- */

/* ---- sequences that units wrote out in place ---- */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* From src/smt3/code_002BD340.c. */

/* ---- sequences that units wrote out in place ---- */

/* From src/smt3/model/mdlManager.c. */

/* sdfVu1.c lit-vertex transform pipeline. vf28-vf31 hold the screen matrix
 * and vf24-vf26 the world light row that corePktLoadWorldLightMtx and the
 * matrix multiply leave behind; nothing in C writes them. vf16-vf18 hold a
 * UV transform's three rows. */

/* corePktNormalizeNorms: vf2.xyz /= |vf2.xyz|, vrsqrt rather than dividing
 * by zero. vf1.x is the engine's resident 1.0. */

/* corePktLoadLight: vf19-vf21 = the light; each light-row basis scales by
 * the light's W (vf19x). */

/* corePktLoadUvTransform: vf16-vf18 = a texture transform's three 64-bit
 * rows. Each ld/qmtc2.ni pair must stay fused: the GPR carries the loaded
 * row between the two instructions. */

/* corePktLightVertex: shared lighting and screen transform. vf6 is the
 * normal (or its negation), vf2 the position. */

/* cfc2 through the last sqc2/sb must stay one statement: the GPR carries
 * the clip byte across the stores that follow it. */

/* corePktXfmLightClip/UV: norm -> vf6, vert -> vf2, both through the
 * unaligned load CORE_VU_LOAD_XYZ builds; its GPR must not be split from
 * the qmtc2.ni that consumes it. */

/* corePktXfmLightClipPB/UVPB: position and normal already sit in the
 * point buffer record. */

/* corePktRelightBackFaces: the normal is flipped in place and the shade
 * and clip bytes recomputed; the position and its clip flags are
 * untouched. */

/* VU0 scratchpad unpack and reference-map lighting. */

/* The EE's load/store ordering barrier. */

/* FBRST, COP2 control register 28: read it, set bits, write it back.
 * aTmp is an output scratch register; its prior C value is not read. */

/* VPU_STAT, COP2 control register 29. */

#endif /* DDS_PS2ASM_H */
