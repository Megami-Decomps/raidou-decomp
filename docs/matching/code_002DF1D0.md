# Draft notes: code_002DF1D0

## sbtlGetSpecialExp (code_002DF1D0, raidou2 0x78)

Raidou 2: 23 of 23 words differ, first at +0x0; emitted 92 bytes, native 120 bytes.

Scans sbtlData130 to stageNo -1 and returns the matching row's exp. The original recomputes the 3*i index every pass and reloads wk_sbtl->stageNo inside the loop, as if the loop optimiser never ran; the draft's loop is strength-reduced. Tried a for loop with direct return, a ret local with break, and a post-incremented index.

## sbtlGetSpecialMoney (code_002DF1D0, raidou2 0x78)

Raidou 2: 23 of 23 words differ, first at +0x0; emitted 92 bytes, native 120 bytes.

Same scan as sbtlGetSpecialExp, returning the money column; same cause: the original's loop is not strength-reduced and reloads wk_sbtl->stageNo every pass. Same shapes tried.

## sbtlGetChuusei_ChkMemSkl (code_002DF1D0, raidou2 0x84)

Raidou 2: 21 of 33 words differ, first at +0x4.

The rate lives in callee-saved f20 while both constants are set after the sbtlChkHaveMemSkl call, with the multiply duplicated into a beql slot. An if/else after the call drops f20; rate = 0 before the call keeps f20 but puts its mtc1 before the call; an early return changes nothing else.
