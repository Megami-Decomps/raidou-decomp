#include "common.h"

INCLUDE_ASM(const s32, "event/evtBitEditor", dds3BitEditorProcess);
#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorRowBase);
#else
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData2);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorTable);
#else
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData3);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorTable2);

INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorGlyphTemplate);
#else
INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorProcessData4);
#endif

INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorColumnFormat);

INCLUDE_SDATA(const s32, "event/evtBitEditor", dds3BitEditorRowFormat);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtBitEditor", evtPendingEventSelection);

#endif
