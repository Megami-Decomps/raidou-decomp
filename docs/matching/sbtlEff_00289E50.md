# Draft notes: sbtlEff_00289E50

## sbtlFunc (sbtlEff_00289E50, raidou1 0x144)

Raidou 1: 58 of 79 words differ, first at +0x8; emitted 316 bytes, native 324 bytes.

The table copy picks v1/a3 for source and destination where retail uses a3/t0, which shifts every later register.

Batch notes: Table copy picks v1/a3 for src/dst where retail uses a3/t0, shifting every later register (same issue as sbtlBtlLoadPlayerOnMemDataSub); logic same. Menu call records: id 4..18, ret follows the call. SbtlCallBtlSearchEffectData2 and sbtlBtlLoadPlayerOnMemDataSub third/second param now void *pData (call records pass a data pointer)
