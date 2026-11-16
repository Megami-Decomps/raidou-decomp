# Draft notes: basic/dds3SceneBasic

## dds3SceneBasicSearchSerialID_Used (basic/dds3SceneBasic, raidou1 0xC0, raidou2 0xC0)

Raidou 1: 33 of 48 words differ, first at +0x4.
Raidou 2: 33 of 48 words differ, first at +0x4.

control flow right (type loop, two scene searches, restart on hit); registers rotate (retail s0=serialID s1=type s2=flag). Tried for+break with post-loop test, single for with type=0 reset (37/41), local order + if/else tail (32/47, 4 short). vector checked boot (id 3 free -> 3)

Related: register-allocation.

## dds3SceneBasicStartScript (basic/dds3SceneBasic, raidou1 0x84, raidou2 0x84)

Raidou 1: 12 of 34 words differ, first at +0x20; emitted 152 bytes, native 132 bytes.
Raidou 2: 12 of 34 words differ, first at +0x20; emitted 152 bytes, native 132 bytes.

smt3 body without printfs, Raidou priority 1002, returns dds3ProcessID_t *. Retail keeps jal+epilogue on scrStartScript2; ee-gcc sibcalls return f() and a result local alike; smt3 matched with a do{}while(0) blocker (not used: dummy shape). Nested-if single exit: 25 words. vectors do not reach, checked against the asm

Related: sibcall.
