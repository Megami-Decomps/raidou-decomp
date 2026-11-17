# Draft notes: facility/fclMisc

## fclUpdateMessageEx (facility/fclMisc, raidou1 0xDC, raidou2 0xDC)

Raidou 1: 2 of 55 words differ, first at +0xC8.
Raidou 2: 2 of 55 words differ, first at +0xC8.

switch with empty case 0 gives retail's root-1 case tree (3-case switch splits at 2); last arm jal fclEndMessageEx where retail sibcalls with j; tried cached gMsgID, if/else chain (43 words); prototypes itfMesMngSetSelect/SetSelectDefKey moved to itfMesManager.h; R2 same 2 words; vectors do not reach, checked against the asm

Related: switch case tree shape and the last arm sibcall.
