#!/bin/sh
# land_sync.sh: after a raidou-decomp push, refresh the sync tables from this tree, sync both shared
# IDBs with --defs, reload the r5900 v2 plugin and save. Run between batches: it changes live pseudocode.
set -e
T=$(dirname "$(readlink -f "$0")")
cd "$T/sync"
python3 prep_stage1.py
python3 extract_types.py
python3 extract_protos.py
python3 extract_globals.py
python3 variant_protos.py
cd "$T"
export SHARED_APPROVED=1
python3 shared.py sync 13337 SLUS_214.31 r1 --defs
python3 shared.py sync 13338 SLUS_218.45 r2 --defs
python3 shared.py plugin 13337 SLUS_214.31
python3 shared.py plugin 13338 SLUS_218.45
python3 shared.py save 13337 SLUS_214.31
python3 shared.py save 13338 SLUS_218.45
