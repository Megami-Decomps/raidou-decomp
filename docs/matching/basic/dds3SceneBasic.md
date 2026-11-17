# Draft notes: basic/dds3SceneBasic

## dds3SceneBasicSearchSerialID_Used (basic/dds3SceneBasic, raidou1 0xC0, raidou2 0xC0)

Raidou 1: 33 of 48 words differ, first at +0x4.
Raidou 2: 33 of 48 words differ, first at +0x4.

control flow right (type loop, two scene searches, restart on hit); registers rotate (retail s0=serialID s1=type s2=flag). Tried for+break with post-loop test, single for with type=0 reset (37/41), local order + if/else tail (32/47, 4 short). vector checked boot (id 3 free -> 3)

Related: register-allocation.
