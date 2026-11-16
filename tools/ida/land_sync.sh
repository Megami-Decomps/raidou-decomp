#!/bin/sh
# land_sync.sh: after a raidou-decomp push, refresh the sync tables from this tree, sync the shared IDBs with
# --defs, reload the r5900 v2 plugin and save. Run between batches: it changes live pseudocode.
set -e
usage() {
    echo "usage: land_sync.sh [--game r1|r2]
Refresh the sync tables, then sync each shared IDB, reload its plugin and save it (both by default).
  --game r1|r2  only that game's IDB (r1: port 13337 SLUS_214.31, r2: port 13338 SLUS_218.45)
  -h, --help    print this and exit"
}
GAMES="r1 r2"
while [ $# -gt 0 ]; do
    case "$1" in
    -h | --help)
        usage
        exit 0
        ;;
    --game)
        case "$2" in
        r1 | r2) GAMES=$2 ;;
        *)
            usage >&2
            exit 2
            ;;
        esac
        shift 2
        ;;
    *)
        usage >&2
        exit 2
        ;;
    esac
done
idb() {
    case "$1" in
    r1) echo "13337 SLUS_214.31" ;;
    r2) echo "13338 SLUS_218.45" ;;
    esac
}
T=$(dirname "$(readlink -f "$0")")
cd "$T/sync"
python3 prep_stage1.py
python3 extract_types.py
python3 extract_protos.py
python3 extract_globals.py
python3 variant_protos.py
cd "$T"
export SHARED_APPROVED=1
for g in $GAMES; do python3 shared.py sync $(idb $g) $g --defs; done
for g in $GAMES; do python3 shared.py plugin $(idb $g); done
for g in $GAMES; do python3 shared.py save $(idb $g); done
