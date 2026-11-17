# Draft notes: movie/mviManager

## mnuUpdateMovieDrawShutdownCountdown (movie/mviManager, raidou1 0xA8, raidou2 0xA8)

Raidou 1: 1 of 42 words differ, first at +0x68.
Raidou 2: 1 of 42 words differ, first at +0x68.

1 words off: IDA pseudocode on the smt3 shape; bne where retail has bnel with the increment in the slot (+0x68); tried inverted !=30 arm (worse), a count local, one shared scrSetIntReturnValue via goto; vectors do not reach, checked against the asm
