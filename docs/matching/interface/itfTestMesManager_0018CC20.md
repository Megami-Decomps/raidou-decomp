# Draft notes: interface/itfTestMesManager_0018CC20

## TestMsgMngD_Process (interface/itfTestMesManager_0018CC20, raidou1 0x9C, raidou2 0x9C)

Raidou 1: 6 of 39 words differ, first at +0x20.
Raidou 2: 7 of 39 words differ, first at +0x20.

smt3 body with evtPictureSprite* (evtPicture.h) and sdfGrOtTable[103] (evtData2); scheduling only: retail reloads testMsgTexture before the three lui and sets v0=0 before the epilogue restores; tried nested return (6, worse head), sized/unsized extern arrays; R2 7 words; vectors do not reach, checked against the asm

Related: scheduling of the testMsgTexture reload and v0=0.
