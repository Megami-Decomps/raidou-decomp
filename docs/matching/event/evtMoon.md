# Draft notes: event/evtMoon

## Process2_evtMoon (event/evtMoon, raidou1 0xB8C)

Raidou 1: 624 of 723 words differ, first at +0x34; emitted 2892 bytes, native 2956 bytes.

R1 only (R2 body 3268 bytes). Same size class, structure and calls; offset from register allocation: retail keeps 10 and 128 in s1/s6 with a stray div-by-10 trap in the sin block (dead /10 in source?), splits 0x80808000 into 0x80000000|0x808000, and treats w/2 as signed-odd. Shapes tried: fade min form (++ matches), colour expression x3, switch for the 1-3/5-7 tests (matches retail decision tree). Vectors reach only the fade prologue (zeros), checked against the asm.
