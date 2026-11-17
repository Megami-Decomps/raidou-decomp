# Draft notes: code_0013CDE0

## mapBgFScrollOn (code_0013CDE0, raidou1 0x80)

Raidou 1: 22 of 32 words differ, first at +0xC.

store order only: with each arm's retail-second store written last (sndSePlay_sndAt3 pattern) the arms no longer cross-jump (27 -> 22), but sched2 regroups the swc1 runs by source register (x, y, z stores pulled together) where retail keeps 68/65/69/66/70/67 interleaved; vectors do not reach, checked against the asm

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## fldBgZoomTest (code_0013CDE0, raidou1 0x2B4)

Matches.

Fakematch: three FAKEMATCH_ASM_LIVE(mode) after the mapZoomSetGoal calls that must stay jal. Without them the function is 14 words off; the same result with C dead stores matches but is not allowed.

## mapCutBgLoad (code_0013CDE0, raidou1 0x88)

Matches.

Fakematch: one FAKEMATCH_ASM_LIVE(cut) after mapBgFree2. Without it the function is 30 words off; the same result with a C dead store matches but is not allowed.

## mapBgSetScroll (code_0013CDE0, raidou1 0x1384)

Raidou 1: 1085 of 1248 words differ, first at +0x0; emitted 4992 bytes, native 4996 bytes.

Body complete, 1248 vs 1249 words. The switch is on fldDbg.camOn and needs an explicit case 1 label on default for the 5-entry jump table; cases 2 and 3 are separate bodies (512 vs 1024 width; case 3 copies mapData87/88 at zoom >= 1.001). Vectors reach the camId case 1 path and its stores agree in order.

Related: register allocation: retail keeps 1.0f in f20 for the whole function and saves an unused s0 (frame 64 vs 48), x/y/z in f22/f21/f23; the camera if-chain tests == 1 first.

## mapZoomSetGoal (code_0013CDE0, raidou1 0x13F4)

Raidou 1: 1223 of 1249 words differ, first at +0x18; emitted 4996 bytes, native 5108 bytes.

Complete body: an if-chain on fldGlobalBase.camId 1/2/3/else with per-case tails (cross-jumped in retail). Independent f32 scalar outputs give retail's addiu a1,sp,4 per call (f32 array outputs kept &pos[1] in s0). Case 1 vector matches retail's stores and values (opening).

Related: FP register allocation: retail saves f20-f27, the draft f20-f26, 27 words short; a mapData74 read-back form was tried with no gain.
