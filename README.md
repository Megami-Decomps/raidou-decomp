# Devil Summoner: Raidou Kuzunoha 1 and 2

A work-in-progress matching decompilation of *Shin Megami Tensei: Devil
Summoner* (Raidou Kuzunoha vs. the Soulless Army) and *Shin Megami Tensei:
Devil Summoner 2* (Raidou Kuzunoha vs. King Abaddon) for PlayStation 2. Both
games run on the SMT3 engine, so this repository is set up from
[smt3-decomp](https://github.com/Megami-Decomps/smt3-decomp) and carries two
versions in one tree the way
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp) does.

Building requires your own copy of the games. Disc images and retail
executables are not included, and nothing derived from them is committed.

## Status and versions

All code is still assembly split from the retail executables. `ninja` relinks
the split objects and checks the result against the retail SHA-1.

| Version | Game | Serial | ELF SHA-1 |
|---|---|---|---|
| `raidou1` | Shin Megami Tensei: Devil Summoner - Raidou Kuzunoha vs. the Soulless Army (USA) | `SLUS_214.31` | `a005ff18213eb72017119c7d666312507ad3b5ea` |
| `raidou2` | Shin Megami Tensei: Devil Summoner 2 - Raidou Kuzunoha vs. King Abaddon (USA) | `SLUS_218.45` | `bc0d30b35a40324db1f6d43bb652c6ca1dab3629` |

Both ELFs have entry `0x00100008` and keep their section headers. `gp` is
`0x00471770` in `raidou1` and `0x004E4170` in `raidou2`. `.vutext` and
`.vudata` are byte-identical to SMT3's; the VU0 and VU1 microcode is assembled
from the listings in `src/<version>/sdf/vu/`, taken from smt3-decomp.

## Toolchain

The build uses smt3-decomp's compilers: ee-gcc 2.96 (`2.96-ee-001003-1`) for
game code and ee-gcc 2.9 (`991111-01`) for the Sony libraries. Evidence that
this is right for both games, so far:

- Both link the same Sony EE libraries as SMT3, all tagged `PsII... 2500`
  (SDK 2.5.0): libgraph, libdma, libpad, libcdvd, libmrpc, libipu, libmc,
  libkernl, plus libsdr.
- About 2,300 functions in each game are word-identical to SMT3's (strict
  matches in smt3-port's cross-game map), so their compiler and flags are
  SMT3's.
- `raidou2` contains C++ with exceptions (a 0x904-byte `.eh_frame` and a
  `.gcc_except_table`); its CIE has an empty augmentation string, which is
  GCC 2.x output (GCC 3 writes `zR`). `raidou1`'s `.eh_frame` is only the
  4-byte terminator, as in SMT3.

No C is compiled yet, so none of this is tested by the build.

## Building

Requirements: Linux x86-64 or WSL2 with 32-bit i386 support, Python 3.10+,
`ninja`, `cpp` and `git`.

```sh
python -m venv .venv && . .venv/bin/activate
python -m pip install -r requirements.txt
python tools/download_tools.py

# Copy the disc images into the repository root or orig/, then:
python tools/extract.py        # or copy each ELF to orig/<version>/<serial>
python configure.py            # every version with an ELF in orig/, or name one
ninja                          # or: ninja raidou1
```

## Acknowledgements

- [smt3-decomp](https://github.com/Megami-Decomps/smt3-decomp), whose build
  (`configure.py`, the tools under `tools/`, the VU listings) this repository
  starts from, and [dds-decomp](https://github.com/Megami-Decomps/dds-decomp)
  and its author **redthing1**, whose machinery smt3-decomp in turn vendors.
- [splat](https://github.com/ethteck/splat),
  [spimdisasm](https://github.com/Decompollaborate/spimdisasm) and
  [objdiff](https://github.com/encounter/objdiff).
- [decomp.me](https://decomp.me) for the ee-gcc toolchain packaging.
