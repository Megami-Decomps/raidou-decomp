# Draft notes: sdf/sdfIpu

## sdfIpuDecode (sdf/sdfIpu, raidou1 0x1D0, raidou2 0x1D0)

Raidou 1 and Raidou 2: 96 of 114 words differ, first at +0x10; emitted 456 bytes, native 464 bytes.

First differing at +0x10. Ee-gcc schedules &cb->dma into the prologue exactly as retail does, then the delay-slot pass moves it into the bnez slot; retail's slot holds a `$4 = 0` that stands between that address and the test, which no argument of the calls inside the if-body reaches, and whose live range is what pushes retail's constants off $4.

Retail fills a delay slot with `$4 = 0`.

Related: [smt3 levers: store scheduling](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers/scheduling.md).
