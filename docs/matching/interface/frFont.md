# Draft notes: interface/frFont

## MessageRequestNS (interface/frFont, raidou1 0x40, raidou2 0x40)

Raidou 1 and Raidou 2: 7 of 15 words differ, first at +0x20; emitted 60 bytes, native 64 bytes.

Nothing is recorded about this draft.

Raidou 1, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x20 | `jal 0x12e0` | `sw zero,0(sp)` |
| +0x24 | `sw zero,0(sp)` | `jal 0x181090` |
| +0x28 | `ld ra,24(sp)` | `sb zero,-21960(gp)` |
| +0x2C | `sb s0,0(gp)` | `ld ra,24(sp)` |

Raidou 2, first differing words:

| Offset | Draft | Retail |
|---|---|---|
| +0x20 | `jal 0x1320` | `sw zero,0(sp)` |
| +0x24 | `sw zero,0(sp)` | `jal 0x196080` |
| +0x28 | `ld ra,24(sp)` | `sb zero,-19728(gp)` |
| +0x2C | `sb s0,0(gp)` | `ld ra,24(sp)` |

## frCallGetFontWidth2 (interface/frFont, raidou1 0x260)

Raidou 1: 80 of 151 words differ, first at +0x5C; emitted 604 bytes, native 608 bytes.

glyph metrics from gFT prop pairs; FontNo doubled in place (fixed s2/s3 roles), char test as a switch keeps the +32 (retail compares 33/49); remaining: retail re-tests flags&0x40 instead of keeping it, and a0/a1 roles for base width and fontHeight; tried u16 locals, cell store order; vectors reach (opening rows, FontNo 0x33 gives advance 12, x -16), not replayed; R2 body is 128 words, r1only; frCallGetFontWidth2/frSetFontSub2 made void (no caller uses a result)

Related: register-allocation.

## frSetFont (interface/frFont, raidou1 0x148, raidou2 0x148)

Raidou 1: 4 of 82 words differ, first at +0x54.
Raidou 2: 4 of 82 words differ, first at +0x54.

4-arg Raidou body: no 0x20 floor, out-of-range index clamps to 0x147, frSetFontSub takes s8 (prototype fixed); left: frCallMemset a0/a1 setup order and the entry address in a1 vs s1; tried an entry pointer local (77 off); title-ng vectors reach it (callees and stores agree with the C), checked against the asm

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## frSetFontString (interface/frFont, raidou1 0x198)

Raidou 1: 3 of 102 words differ, first at +0x114.

retail loads chain->advance into v1 and childCount into v0, mine swaps them; tried +=, explicit sum, count first (23), previous store last; R2 body differs (472 bytes, r1only); global named frSetFontString_style (renames.tsv), tests used R1/R2 names; vector checked (menu)

N1 retry: frData2 + (glyph->advance + chain->advance) takes it from 7 to 3 (childCount now in v0); left: retail loads glyph->advance first into a1, sums chain + glyph in v1, then sum + frData2 into frData2's register. Tried an s32 advance local holding frData2 (=, +=, both sum orders: 3 to 6), frData2 + (chain + glyph) (6). frFontAppendIndexedTextToGlyphChain closes to 1 word once this is C.

Related: register-allocation.

## frDrawEffect (interface/frFont, raidou1 0x348)

Raidou 1: 64 of 208 words differ, first at +0x240; emitted 832 bytes, native 840 bytes.

For the CCN_QUAKE_OFF test (last child fade != 0x80) retail emits xori 0x80 and beq with a nop in the bne delay slot; the draft loads 128 into a0 in that delay slot and compares with beq, two words short, so the rest of the function shifts (64 of 208 words differ). A u32 local for the fade, or comparing the byte inline, does not change it. Checked against retail call records (ingame, step 0x80 path) and the asm.

Related: [smt3 levers: callee visibility and branch-likely](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/callee-visibility.md).

## frDrawFontOt (interface/frFont, raidou1 0x34C)

Raidou 1: 21 of 211 words differ, first at +0x0.

Prologue only: retail copies pFRQ into s1 before the NULL test and stores the stack quake flag from s7; the draft tests a0 and stores the flag from v1, 22 of 211 words differ. Reusing the parameter as the chain cursor grows the function (206 words off) and declaring the quake flag next to canStopLips gives 50. Checked against retail call records (ingame) and the asm.

Related: prologue.

## frFontAppendIndexedTextToGlyphChain (interface/frFont, raidou1 0x170, raidou2 0x170)

Raidou 1: 47 of 90 words differ alone, first at +0x48; 0 once frSetFontString is C (the -DNON_MATCHING build).
Raidou 2: 47 of 90 words differ, first at +0x48; emitted 360 bytes, native 368 bytes.

two frCallSetFontString sites: retail keeps them apart with beql/lbu Spd in the firstChild==NULL delay slot and stores v0, mine is 2 words short from block layout; tried local reuse, assignment chains on both paths; DDS body with Raidou text/var names; vector checked (menu)

N1 retry: with frSetFontString compiled as C the body is exact after ctx->glyphChain = frCallSetFontString(...); context = ctx->glyphChain; in the firstChild arm (the assignment chain stored the copy). Alone it is 47 off because the block layout follows frSetFontString's visibility, so it waits on frSetFontString.

Related: cross-jump.

## frAnalyzeMessage (interface/frFont, raidou1 0x1F0)

Raidou 1: 1 of 124 words differ, first at +0x1A8.

SMT3 frAnalyzeMessage loop on frFontCtx_t; one word: retail bne with addiu v0,10 in the slot, mine bnel; tried continue/else-if chain, inverted ModeFlag test, IDA if(code!=10) (61); lever addressing-and-types 526 says not reachable from the branch shape; R2 body differs (568 bytes, r1only); frMsgInfo_t merged into frFontCtx_t in frFont.c; vector checked (menu)

Related: branch-likely.
