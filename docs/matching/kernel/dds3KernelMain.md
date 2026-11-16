# Draft notes: kernel/dds3KernelMain

## dds3KernelMain (kernel/dds3KernelMain, raidou1 0x114, raidou2 0x114)

Raidou 1: 0 of 69 words differ.
Raidou 2: 0 of 69 words differ.

smt3 goto body minus printf; dds3SoftResetFlag s8 (lb). Last 2: lb FileCmdLoPrio in v1+beq vs retail v0+beql. while-loop with sign local gives beql but swaps sign/time regs (11 words); decl order no effect. vectors do not reach, checked against the asm

Related: delay-slot.
