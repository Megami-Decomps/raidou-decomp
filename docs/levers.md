# Matching levers found here

Shapes found while matching Raidou functions, in the format of
[smt3-decomp's lever pages](https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/matching-levers.md),
which list the rest.

## Control flow

| Retail shows | Change |
|---|---|
| `bnez x, body` then `b exit` with `daddu $2, $0, $0` in its slot, a return-0 block of its own right after a guard chain, where your C hoists `v0 = 0` above the tests and threads every guard to the epilogue | write the guards that share the zero as one `||` with one `return 0`, keeping a guard that must come before a load on its own: `if (lwk == NULL) return 0; dwk = lwk->dwk; if (dwk == NULL || lwk->num == 0) return 0;`. Separate early returns let cc1 hoist the zero and branch each test straight to the shared epilogue; the `||` gives the last test its own `b` plus zero. Nine Raidou list walkers turned on this alone (`sbtlReleaseSM_List`, `sbtlReleaseDelList`, `sbtlCallMemFreeDirect6` in `code_002A38D8`), from 3 to 20 differing words each to exact. A mechanical sweep of the same rewrite over 31 further drafts found no other match: where the residual is register allocation or block layout it moves 0 to 7 words |
