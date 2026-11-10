#ifndef DDS_COMMON_H

#define DDS_COMMON_H
#include "../common.h"
/* Marks a DMA tag or packet type: the DMAC reads it at a quadword (16-byte)
 * boundary. The PS2 layouts already place these types there, so this build
 * adds nothing. A build whose layouts differ (8-byte pointers, say) defines
 * QWORD_ALIGNED as __attribute__((aligned(16))). */

#ifndef QWORD_ALIGNED

#endif

/* Stores a zero quadword. ee-gcc materialises the 128-bit zero in a
 * register and stores that, which is what retail does, so the operation stays
 * written as an assignment here. A build with no 128-bit integer type defines
 * this as a 16-byte clear instead. */

#ifndef QUAD_ZERO

#endif

/* A pointer in the EE's 32-bit address representation, which is how this
 * engine passes a packet or buffer address through a u32 field or argument.
 * It lives here rather than in eeregs.h because 60 units need it and almost
 * every unit already includes this header. On target the cast is the whole
 * of it; a build with wider pointers redefines both of these and must keep
 * them inverse to each other. */

#ifndef EE_ADDR

#endif

/* The same address back as a pointer. The (u32) is kept because the sites
 * spell it: dropping it would change the target conversion, not only the
 * host one. */

#ifndef EE_ADDR_PTR

#endif

/* The same narrowing where the site spells the signed type. 422 sites do, so
 * the signedness is kept rather than folded into EE_ADDR: on the EE the two
 * differ in later arithmetic and comparisons, not in the address. */

#ifndef EE_ADDR_S

#endif

/* An EE address widened back to a typed pointer. Unlike EE_ADDR_PTR there is
 * no (u32) in the expansion, because these sites spell none: the cast to the
 * pointer type is the whole of the target conversion and adding a step could
 * change it. */

#ifndef EE_ADDR_AS

#endif

/* A guest address held in an integer, which is how this engine passes a
 * packet, buffer or callback address through a field, parameter or return
 * type rather than through a pointer. EE_ADDR produces one and EE_ADDR_PTR
 * takes one back. On this target it is exactly u32, so every layout, every
 * promotion and every byte of output is what it was when these declarations
 * spelled u32; a build with wider pointers defines ADDR_TYPE to an
 * integer that holds its own pointers.
 *
 * It is deliberately not spelled on a hardware packet word. A DMA tag, VIF
 * code or GIF register field is 32 bits because the hardware reads 32 bits
 * there, and widening one would describe the packet wrongly. Those stay u32
 * and a host truncates into them on purpose. */

#ifndef ADDR_TYPE

#endif

/* The same, where the declaration spells the signed type. The signedness is
 * kept rather than folded into addr_t for the reason EE_ADDR_S exists:
 * unsigned is not a free substitution, it changes comparisons, division and
 * promotion, which on this target is codegen. */

#ifndef SADDR_TYPE

#endif

/* Asserts a structure's retail size, by the negative-array-size trick ee-gcc
 * 2.96 accepts. The typedef emits nothing, so this costs no code. The check
 * applies only where a pointer is four bytes: a wider pointer moves every
 * member after one, so the retail size is the wrong question there and the
 * members' own types stay the record of the layout. */

#ifndef SIZE_CHECK

#endif

/* Binds NAME as an assembler alias for BASE + OFFSET, for a view into
 * another object that has no symbol of its own. The assembler does the
 * arithmetic, so nothing is allocated and the reference stays gp-relative.
 * A build whose assembler cannot resolve the expression, which is every
 * target but the EE, defines this differently. */

#ifndef SYMBOL_AT

#endif

/* The same for a symbol naming a region rather than an object, so the
 * declaration is an array and a difference against another such symbol stays
 * pointer arithmetic. SYMBOL_AT cannot serve: it puts the type before the
 * name and an array's brackets go after it.
 *
 * Structured rather than a raw asm string, for a reason measured on a
 * supported compiler. LLVM's integrated assembler refuses symbol arithmetic
 * against an undefined extern, with "expected relocatable expression", as
 * soon as the symbol is referenced; GNU as defers the relocation and accepts
 * it. A site that spells the alias as a raw string gives a host no way to
 * neutralise that one alias without neutralising every raw asm string in the
 * tree, so the alias belongs behind its own name. A build whose assembler
 * cannot resolve the expression redefines this to the declaration alone and
 * supplies the symbol itself. */

#ifndef SYMBOL_AT_REGION

#endif

/* Localized compiler constraints on a real live value. None of them emits an
 * instruction: the binding only fixes which register cc1 keeps the value in,
 * and the empty template only forces it into a register, or into its storage,
 * at that point. docs/CONTRIBUTING.md says when these are permitted. */
/* Declares NAME while the symbol it binds to keeps its retail name, for a
 * second view of one object or a definition the linker must see under another
 * name. It emits nothing: the label only names the symbol. */

#ifndef FAKEMATCH_ASM_NAME

#endif

#ifndef FAKEMATCH_ASM_REG

#endif

#ifndef FAKEMATCH_ASM_LIVE

#endif

#ifndef FAKEMATCH_ASM_LIVE_PAIR

#endif

#ifndef FAKEMATCH_ASM_LIVE_F32

#endif

/* The _VOLATILE forms hold the constraint where cc1 would otherwise drop it. */

#ifndef FAKEMATCH_ASM_LIVE_VOLATILE

#endif

#ifndef FAKEMATCH_ASM_LIVE_F32_VOLATILE

#endif

/* Input-only extended asm is implicitly volatile; it emits no instruction. */

#ifndef FAKEMATCH_ASM_READ

#endif

#ifndef FAKEMATCH_ASM_READ_PAIR

#endif

#ifndef FAKEMATCH_ASM_LIVE_MEM

#endif

#ifndef FAKEMATCH_ASM_READ_MEM

#endif

#ifndef NULL

#endif

#include "include_asm.h"

#endif /* DDS_COMMON_H */
