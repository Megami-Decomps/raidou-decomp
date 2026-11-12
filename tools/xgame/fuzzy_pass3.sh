#!/bin/sh
# Usage: fuzzy_pass3.sh TARGET SOURCE  (raidou1 raidou2): match TARGET against SOURCE's current names
set -e
W=${FUZZY_WORK:-$HOME/.local/state/raidou-decomp-work/fuzzy}
mkdir -p "$W/src"
case $2 in raidou1) S=SLUS_214.31;; raidou2) S=SLUS_218.45;; esac
python3 tools/xgame/elf_export_funcs.py orig/$2/$S config/$2/symbol_addrs.txt "$W/src/$2.json"
python3 tools/xgame/fuzzy_match.py "$1" --sources "$2"
python3 tools/xgame/fuzzy_apply.py "$1" --eval
python3 tools/xgame/fuzzy_apply.py "$1"
