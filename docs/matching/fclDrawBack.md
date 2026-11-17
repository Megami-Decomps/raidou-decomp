# Draft notes: fclDrawBack

## fldGbInit_System (fclDrawBack, raidou1 0x42C)

Raidou 1: 20 of 267 words differ, first at +0x164.

store scheduling only: same store set, retail emits single stores (word158/170/178, word1C4..21C, word1D4 and 2EC..308, fldDbg menuCur/hitOn/bgDrawOn/skyCngOn) a few slots away from mine; ordsearch over the three store windows took it from 55 to 20 and bottomed; unk21 written after transition (one DDS3_GBWK load for 0x8..0x21) and p assigned after the 2C70/2C72 halves fixed the mode block; both byte loops and the 15-word loop write one element past their arrays as retail does; vector checked (boot, mode 0)

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

## fldGbInit_Field (fclDrawBack, raidou1 0x94)

Raidou 1: 12 of 37 words differ, first at +0x34.

store scheduling only: zero stores to menuCur/floatE4/word318 issue early in retail, hitX/hitZ early in mine; same store set; ordsearch single-line moves bottomed at 7 from retail order (25); vector checked (boot)

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).

Fragile: retyping fldAreaWork_t 0x120..0x128 (hitZ/camAngle/camAngle2 to the s32 nowTalkModelID/nowTakaraTblIdx/nowHealTblIdx, or the same as f32/s32 unions) moved it from 7 to 12 differing words with the same stores.
