# Draft notes: sdf/sdfFileIO

## sdfFileCdImageCmdThread (sdf/sdfFileIO, raidou1 0x26C, raidou2 0x26C)

Raidou 1 and Raidou 2: 0 of 155 words differ.

`check_unit.py` reports no differing word in Raidou 1 and Raidou 2: the draft now compiles to retail's text there. Promote it after confirming the unit's data and the full build.

Note from when the draft was written: Worker slot 3 services disc-backed requests through serialized disc operations. +0x218: the fd = -1 after the read-file close takes $3 where retail takes $2.

Tried: reusing locals for fd; no change.

## sdfFileCmdAnalyzeFileName (sdf/sdfFileIO, raidou1 0x168, raidou2 0x168)

Raidou 1 and Raidou 2: 4 of 90 words differ, first at +0x54.

Two instructions of the /smovie/ and movie path memcmp calls are scheduled differently (4 words)

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
