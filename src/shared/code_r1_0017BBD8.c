#include "common.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdf2D.h"
#include "sdk_libkernl.h"
#include "sdfTex.h"

#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "newdata.h"
#include "ps2asm.h"
#include "fpu.h"
#include "code_0027A1D0.h"
#include "code_r1_0019D3A8.h"
#include "code_r1_0021CE60.h"
#include "code_r1_0017BBD8.h"
#include "eff_editor_menu.h"
#include "dds3GlobalWork.h"
#include "dds3PadManager.h"
#include "billManager.h"

/* Project spellings for the SCE iox layout, with retail's ordinary word alignment. */
typedef struct sdfSceIoStat_t {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} sdfSceIoStat_t;

typedef struct fsDirectoryEntry_t {
    sdfSceIoStat_t stat;
    char name[0x100];
    void *privateData;
} fsDirectoryEntry_t;

typedef char sdfSceIoStat_size_must_be_0x40[
    (sizeof(sdfSceIoStat_t) == 0x40) ? 1 : -1];

typedef char fsDirectoryEntry_size_must_be_0x144[
    (sizeof(fsDirectoryEntry_t) == 0x144) ? 1 : -1];

/* Allocate a variable-length sprite column packet with this draw header. */

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */

/* Sprite vertex record of a column packet: colour, then two corners of the quad. */
typedef struct dds3EffectSpriteCorner_t {
    s32 u, v;
    u8 pad08[8];
    s32 x, y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} dds3EffectSpriteCorner_t;

typedef struct dds3EffectSpriteVertex_t {
    s32 r, g, b, a;
    dds3EffectSpriteCorner_t corner[2];
} dds3EffectSpriteVertex_t;

typedef char dds3EffectSpriteCorner_size[(sizeof(dds3EffectSpriteCorner_t) == 0x20) ? 1 : -1];

typedef char dds3EffectSpriteVertex_size[(sizeof(dds3EffectSpriteVertex_t) == 0x50) ? 1 : -1];

enum {
    BTL_RESOURCE_DESCRIPTOR_BYTES = 0x48,
    BTL_RESOURCE_NAME_RECORD_BYTES = 0x38,
    BTL_PACKED_SCREEN_X_BIAS = 0x7000,
    BTL_PACKED_SCREEN_Y_BIAS = 0x7900,
    BTL_HP_PERCENT_SCALE = 100,
    BTL_DIRECTORY_PATH_BYTES = 0x70,
    BTL_BUILTIN_NAME_COUNT = 62,
    BTL_DIRECTORY_FLAG_CLEAR = 0x1000,
    BTL_RESOURCE_ENTRY_BYTES = sizeof(SBTLRESOURCE_ENTRY),
    /* High-bit selectors are game-specific callback-table positions. */
    BTL_PACKED_ENEMY_COUNT_LIMIT = 0x1400000,
    BTL_PACKED_ENEMY_QUEUED_QUERY = 0x0F800000,
    BTL_PACKED_PARTY_QUEUED_QUERY = 0x0FC00000,
    BTL_PACKED_PARTY_ELEMENT_BLOCK = 0x0F400000,
    BTL_PACKED_PARTY_EMPTY_MP = 0x10000000
};

extern u8 D_003BB818[];
extern u8 sdfHddValid;
extern s32 btlBuiltinResourceNameIndex;
extern u8 D_003BB820[];

struct BtlRuntimeTask;

typedef struct SBTL_ACTOR {
    u8 pad00[0x18];
    SBTL_UNIT *unit;
} SBTL_ACTOR;

typedef struct SBTL_LIST {
    u8 pad00[0x20];
    s32 count;
} SBTL_LIST;

typedef struct SBTLCMD_CTX {
    u8 pad00[0xC];
    u32 flags; /* 0x0C: command-context flags */
    u8 pad10[8];
    SBTL_UNIT *unit;
    u8 pad1C[4];
    s32 commandMode;
    s32 commandValue;
    u8 pad28[4];
    SBTL_UNIT *actionProbeFirst;
    SBTL_UNIT *actionProbeSecond;
    u8 pad34[4];
    u32 parameter38; /* 0x38 */
    u8 pad3C[0x50];
    u32 parameter8C; /* 0x8C */
} SBTLCMD_CTX;

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

struct BtlRuntimeTask;

typedef struct SBTLCONTROL_OBJECT {
    u8 enabled; /* 0x00 */
    u8 pad01[0x0F];
    u8 state; /* 0x10 */
    u8 pad11[0x0F];
    u16 type; /* 0x20 */
    u8 pad22[0x26];
    s32 status; /* 0x48 */
    void (*update)(void); /* 0x4C */
} SBTLCONTROL_OBJECT;

/* Select the smallest X for reference flag 0x200, otherwise the largest.
 * An empty indexed list returns NULL but still loads the native scratch vector. */

struct effRefObj_t;

typedef struct SBTLBTN_UV {
    s32 u;
    s32 v;
} SBTLBTN_UV;

/* Script command handlers set the command state at +0x20 and its
 * argument at +0x24; their numeric opcodes are still unidentified. */
u32 btlScriptSelectEmptyCommand(void);

/* Select the fixed mode-6/value-0xC2 command and continue the script VM. */
u32 btlScriptSelectFixedCommand(void);
u32 btlScriptSelectModeDValueOne(void);
u32 btlScriptSelectModeAValueOne(void);

/* Resolve a command-table entry through its probe, or store the direct command id. */
u32 btlScriptSelectActionEntry(void);
u32 btlScriptSetCommandValueAndParameter(void);
u32 btlCmdSetContextFlagOne(void);
u32 btlScriptSetActorUnitParameter(void);
u32 btlScriptSetBattleWorkParameter(void);

/* Select targets for the current command's action; return VM success. */
u32 btlScriptChooseActionTargets(void);

/* Parameter 0 supplies the group element-check input; return VM success. */
u32 btlScriptPickElementCheckedTarget(void);

/* Select the current command's minimum nonzero-HP candidate; return VM success. */
u32 btlScriptPickLowestHpTarget(void);

/* Select the current command's minimum HP-percentage candidate; return VM success. */
u32 btlScriptPickLowestHpRatioTarget(void);

/* Parameter 0 feeds lowest-HP element-block selection; return VM success. */
u32 btlScriptPickLowestHpBlockTarget(void);

/* Pick the current command's highest-MP target and return VM completion. */
u32 btlScriptPickHighestMpTarget(void);

/* Run the current command's minimum-value target selector; return VM success. */
u32 btlScriptPickLowestLevelTarget(void);

/* Parameter 0 is a status-presence mask for command target selection; return VM success. */
u32 btlScriptPickTargetWithStatusMask(void);

/* Parameter 0 is a status-absence mask for command target selection; return VM success. */
u32 btlScriptPickTargetWithoutStatusMask(void);

/* Apply script parameter 0 as the command target-selection mode; return VM success. */
u32 btlScriptCmdSelectTargetsByMode(void);

/* Separate builtin entry with the same action-target selection body; return VM success. */
u32 btlScriptChooseActionTargetsDuplicate(void);

/* Run the current script command's unlinked-party target selection; return script success. */
u32 btlScriptPickUnlinkedPartyTarget(void);

/* Run the current script command's linked-party target selection; return script success. */
u32 btlScriptPickLinkedPartyTarget(void);

/* Append this command's own actor after target scan; return VM success. */
u32 btlScriptAppendActorToTargets(void);

/* Append the global effect actor to this command's targets; return VM success. */
u32 btlScriptAddEffectActorTarget(void);

/* Parameter 0 feeds element-block target selection; return VM success. */
u32 btlScriptPickElementBlockTarget(void);

/* Prefer a non-actor target, retaining native self fallback; return VM success. */
u32 btlScriptPickTargetExcludingActor(void);

/* Script command selection: low/high flag words gate the stored choices. */
typedef struct SBTLCOMMAND_CONTEXT {
    u8 pad00[0x18];
    SBTL_UNIT *actor; /* 0x18: the command actor's unit. */
    u8 pad1C[0x74];
    u32 selectionFlagsA; /* 0x90 */
    u32 selectionFlagsB; /* 0x94 */
    s32 choicesA[26]; /* 0x98..0xFC */
    s32 choicesB[17]; /* 0x100..0x140 */
} SBTLCOMMAND_CONTEXT;

u32 btlNbScriptCheckActorFlag(void);

/* Remember the choice when the packed MP-percentage condition succeeds. */
u32 btlCmdStoreActorMpCheckChoice(void);
u32 btlCmdStoreBossHealthCheckChoice(void);

/* Remember the count limit only when the eligible enemy count is at most that limit.
   Failure clears the selection flag but leaves the previous stored choice intact. */
u32 btlCmdStoreEnemyCountCheckChoice(void);
u32 btlCmdStorePartyCountCheckChoice(void);
u32 btlCmdStoreActorActionMaskChoice(void);
u32 btlCmdStoreEnemyActionMaskChoice(void);
u32 btlCmdStorePartyActionMaskChoice(void);
u32 btlCmdRememberAllLowerActionBits(void);
u32 btlCmdRememberLowerUnitMode(void);
u32 btlCmdRememberOtherUpperUnitMode(void);
u32 btlCmdRememberLowerEntryCondition(void);
u32 btlCmdRememberUpperEntryCondition(void);

/* Remember the actor entry-code-10 gate result; failure clears only its selection flag. */
u32 btlCmdRememberActorActionGate(void);
u32 btlCmdReturnReadyWithoutTurnsCheck(void);
u32 btlCmdRememberCounterLimit(void);
u32 btlCmdStoreCounterReachedLimitChoice(void);
u32 btlCmdReturnLowHpActionReadiness(void);
u32 btlCmdReturnActorEligibility(void);

/* Report whether active-enemy and cumulative-spawn limits permit another enemy. */
u32 btlCmdReportEnemySpawnCapacity(void);
u32 btlCmdRememberLowerElementAccess(void);
u32 btlCmdRememberUpperElementAccess(void);
u32 btlCmdReturnPartyElementQueryCheck(void);
u32 btlCmdReturnEnemyElementQueryCheck(void);
u32 btlCmdRememberLowerElementExclusion(void);
u32 btlCmdRememberUpperElementExclusion(void);
u32 btlCmdRememberUpperActionAvailability(void);

/* Remember the actor entry-code condition choice only when its query succeeds. */
u32 btlCmdRememberActorEntryCondition(void);
u32 btlCmdStoreDefaultUnitActionChoice(void);
u32 btlCmdStoreAlternateUnitActionChoice(void);
u32 btlCmdStorePartyEntryCheckChoice(void);
u32 btlCmdStoreInversePartyEntryChoice(void);
u32 btlCmdRememberUpperEntryCodeMatch(void);
u32 btlCmdRememberUpperEntryCodeInverse(void);
u32 btlCmdRememberLowerEntryCodeMismatch(void);
u32 btlCmdRememberUpperEntryCodeMismatch(void);
u32 btlCmdReturnPartyActionPresence(void);
u32 btlCmdReturnEnemyActionPresence(void);

/* Remember a choice matching an eligible enemy's selected-action element. */
u32 btlCmdRememberUpperSelectedElement(void);

/* Remember the choice when an eligible party unit's queued action matches its category query. */
u32 btlCmdRememberPartyQueuedActionCategory(void);
u32 btlCmdRememberLowerMissingStatus(void);
u32 btlCmdReturnPartyMissingFlagCheck(void);
u32 btlCmdReturnPartyHeldFlagCheck(void);

/* Remember actor record bit 0x2000 when status bit 0x200 is clear. */
u32 btlCmdRememberNonLowerStatFlag(void);
u32 btlCmdRememberContextSecondaryFlag(void);
u32 btlCommandReportBossEffectOption(void);
u32 btlCmdTestEffectActor(void);
u32 btlCommandReportSpecialEnemyLink(void);

/* Report an eligible matching unit at or below the scripted HP percentage.
   Preserve the native signed products and their unsigned comparison. */
u32 btlCmdCheckHpPercent(void);
u32 btlScriptReturnFirstSpecialEnemySpecies(void);

/* Remember the choice when an enemy's queued query matches; failure clears only its flag. */
u32 btlCmdRememberUpperQueuedQuery(void);

/* Remember the choice when a party unit's queued query matches; failure clears only its flag. */
u32 btlCmdRememberLowerQueuedQuery(void);

/* Remember the choice on a successful party-element block check; failure clears only its flag. */
u32 btlCmdRememberLowerElementBlock(void);

/* Report whether an eligible party unit has zero current MP; the handler tests unit +0x12A. */
u32 btlCmdReportPartyEmptyMp(void);
u32 btlScriptReturnSceneSlotCount(void);

/* Publish the battle turn count to the script integer result; return VM success. */
u32 btlScriptReturnTurnCount(void);

/* Actor +0x134 is partyRecord.level, not an action timer; return VM success. */
u32 btlScriptReturnActorLevel(void);
u32 btlCommandReportSpecialModeValue(void);
u32 btlScriptReturnSelectedBossEffectId(void);
u32 btlScriptReturnEffectActive(void);
u32 btlCmdReturnBattlePhase(void);
u32 btlScriptReturnEffectValue(void);
u32 btlScriptReturnContextSignedByte(void);
u32 btlScriptReturnGlobalDebugValue(void);
u32 btlCommandReportEventEffectValue(void);
u32 btlCmdRunWeightedAiSelection(void);
u32 btlScriptSetBattleCommand(void);
u32 btlResetCommandContextAndSetStateFlag(void);
u32 btlScriptClearSceneTransition(void);
u32 btlScriptBeginSceneTransition(void);
u32 btlScriptFlagAllUnitsDefeatCandidates(void);
u32 btlScriptFlagPlayerDefeatCandidates(void);
u32 btlScriptFlagEnemyDefeatCandidates(void);
u32 btlCmdCameraMove(void);
u32 btlScriptQueueActorCommandSound(void);
u32 btlScriptQueueUnboundCommandSound(void);
s32 btlScriptSetCameraBlendStart(void);
s32 btlScriptSetCameraBlendEnd(void);
u32 btlCmdCameraMoveBlend(void);
u32 btlAiCheckBa(void);
u32 btlAiGetAllKoudo(void);

/* Complete zero-operand builtin 342; retain its unused command-work lookup. */
u32 btlScriptBuiltin342NoOp(void);
u32 btlAiGetFurea(void);
u32 btlCmdReportUnsupportedCameraOrigin(void);
u32 btlCmdReportUnsupportedCameraTwoShot(void);
u32 btlCmdReportUnsupportedCameraBougai(void);
u32 btlCmdReportUnsupportedEvent(void);
u32 btlCmdReportUnsupportedTaikyo(void);
u32 btlCmdReportUnsupportedAllTaikyo(void);
u32 btlCmdClearSpecialEnemyFlags(void);
void btlClosePfsDebugDirectory(s32 directoryHandle);

/* Debug mode delegates to the native reader. Built-in mode returns each name's length,
   clears the directory-type bit, and returns zero when the eight-name table is exhausted. */
s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, fsDirectoryEntry_t *reader);
extern u32 dds3BillboardGeneralNumber(void);
void btlLoadAndReplaceResourceHandle(SBTLRESOURCE_DESCRIPTOR *descriptor, const char *resourceName);
void btlReplaceResourceHandle(SBTLRESOURCE_DESCRIPTOR *descriptor, TMX_HEADER_t *textureResource);

#define EDITOR_KEY_COLS 13

#define EDITOR_KEY_ROWS 3

#define EDITOR_KEY_CELLS (EDITOR_KEY_COLS * EDITOR_KEY_ROWS)

#define EDITOR_CHARACTER_COUNT (EDITOR_KEY_CELLS - 1)

#define EDITOR_BLANK_CELL (EDITOR_KEY_CELLS - 1)

#define EDITOR_BACKSPACE_CELL EDITOR_KEY_CELLS

#define EDITOR_CONFIRM_CELL (EDITOR_KEY_CELLS + 1)

#define EDITOR_DELETE_CODE (EDITOR_KEY_CELLS + 3)

void btlBossDebugPrintfN(s32 x, s32 y, s32 color, const char *format, ...) {
}

void btlDebugDrawText(s32 x, s32 y, s32 style, const char *text) {
}

void btlDebugDrawRectangle(s32 x, s32 y, s32 width, s32 height) {
}

void btlDebugDrawTwoColorRectangle(void) {
}

void btlDebugDrawMappingColorSpan(void) {
}

void btlDebugDrawLine2D(void) {
}

void pbLeaf15(void) {
}

void btlDebugDrawPoint2DNoOp(void) {
}

void btlDebugDrawPoint(f32 *position) {
}

void pbLeaf16(void) {
}

void btlDebugDrawRadiusGuide(f32 *center, f32 radius, u32 color, s32 flags) {
}

void btlDebugDrawOrientedExtentGuide(f32 *center, f32 extent, f32 height, u32 color, f32 (*rotation)[4]) {
}

void btlDebugDrawScaledPositionGuide(void) {
}

void pbLeaf17(void) {
}

void btlDebugDrawSegment(f32 *from, f32 *to, u32 color) {
}

void btlDebugMenuBeforeRecordNoOp(void) {
}

void btlDebugMenuAfterRecordNoOp(void) {
}

INCLUDE_ASM(const s32, "code_r1_0017BBD8", pbLeaf18);

void btlDebugMenuRecordNoOp(void *context) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_001F6110.c:2933, rework, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
u8 *btlFindEntryByCommand(EffQueuedFileObject *list, s32 command) {
    s32 i;
    EffEditorRow *entry = list->rows;
    for (i = 0; i < list->count; i++) {
        if ((s32)entry->payload.numeric.target == command) {
            return (u8 *)entry;
        }
        entry++;
    }
    return (u8 *)entry;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_001F6110.c:2947, rework, same function 1.00 (words 24/24, calls 1.00, shape 1.00) */
s32 btlCountNonPartOpcodes(EffQueuedFileObject *list) {
    u32 count;
    EffEditorRow *entry;
    u32 i;
    s32 found;
    if (list == NULL) {
        return 0;
    }
    count = list->count;
    if (count == 0) {
        return 0;
    }
    entry = list->rows;
    found = 0;
    for (i = 0; i < count; i++) {
        switch (entry->kind) {
        case 0x14:
        case 0x15:
            break;
        default:
            found++;
            break;
        }
        entry++;
    }
    return found;
}

s32 btlOpenPfsDebugDirectory(const char *directoryName) {
    char pathBuffer[BTL_DIRECTORY_PATH_BYTES];

    /* Debug mode opens a formatted pfs0 path; built-in mode resets name iteration. */

    if (sdfHddValid != 0) {
        sprintf(pathBuffer, "pfs0:/%s", directoryName);
        return sceDopen(pathBuffer);
    }
    btlBuiltinResourceNameIndex = 0;
    return 0;
}

/* Close the scanned directory only in debug filesystem mode; ignore the native result. */
void btlClosePfsDebugDirectory(s32 directoryHandle) {
    if (sdfHddValid == 0) {
        return;
    }
    sceDclose(directoryHandle);
}

s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, fsDirectoryEntry_t *reader)
{
    if (sdfHddValid != 0) {
        return sceDread(directoryHandle, reader);
    }
    if ((u32)btlBuiltinResourceNameIndex >= BTL_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(reader->name, dds3EffectFileNameTable2[btlBuiltinResourceNameIndex]);
    reader->stat.mode &= ~BTL_DIRECTORY_FLAG_CLEAR;
    btlBuiltinResourceNameIndex++;
    return strlen(reader->name);
}

/* Build a scan-order list of built-in names and enabled resource-file categories. */
SBTLRESOURCEENTRY_LIST *btlScanDirectory(const char *path, s32 flags) {
    fsDirectoryEntry_t reader;
    SBTLRESOURCEENTRY_LIST *list;
    SBTLRESOURCE_ENTRY *entry;
    SBTLRESOURCE_ENTRY *tail;
    s32 directoryHandle;
    s32 generalCount;
    s32 index;
    s32 length;
    s32 category;

    directoryHandle = btlOpenPfsDebugDirectory(path);
    list = sdfChipAllocDirect(sizeof(SBTLRESOURCEENTRY_LIST));
    if (path != NULL) {
        list->pathPrefix = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(list->pathPrefix, path);
    } else {
        list->pathPrefix = NULL;
    }
    list->head = NULL;
    tail = NULL;
    list->count = 0;
    if (path == NULL) {
        return list;
    }
    if (flags & 8) {
        index = 0;
        generalCount = dds3BillboardGeneralNumber();
        for (; index < generalCount; index++) {
            entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
            entry->category = 8;
            entry->value = index;
            entry->id = 0;
            sprintf(entry->name, "GENERAL %d", index);
            if (tail != NULL) {
                tail->next = entry;
            } else {
                list->head = entry;
            }
            entry->prev = tail;
            entry->next = NULL;
            tail = entry;
            list->count++;
        }
    }
    while ((length = btlReadBattleResourceDirectoryEntry(directoryHandle, &reader)) > 0) {
        if ((reader.stat.mode & 0xF000) == 0x1000) {
            continue;
        }
        if (length < 4) {
            continue;
        }
        if (length >= 0x2F) {
            continue;
        }
        if ((flags & 1)
            && (reader.name[length - 3] == 't' || reader.name[length - 3] == 'T')
            && (reader.name[length - 2] == 'm' || reader.name[length - 2] == 'M')
            && (reader.name[length - 1] == 'x' || reader.name[length - 1] == 'X')) {
            category = 1;
        } else if ((flags & 2)
            && (reader.name[length - 3] == 'p' || reader.name[length - 3] == 'P')
            && reader.name[length - 2] == '2'
            && (reader.name[length - 1] == 'a' || reader.name[length - 1] == 'A')) {
            category = 2;
        } else if ((flags & 4)
            && (reader.name[length - 2] == 'p' || reader.name[length - 2] == 'P')
            && (reader.name[length - 1] == 'b' || reader.name[length - 1] == 'B')) {
            category = 4;
        } else if ((flags & 0x20)
            && (reader.name[length - 2] == 'e' || reader.name[length - 2] == 'E')
            && (reader.name[length - 1] == 'p' || reader.name[length - 1] == 'P')) {
            category = 0x20;
        } else if ((flags & 0x10)
            && (reader.name[length - 3] == 'e' || reader.name[length - 3] == 'E')
            && (reader.name[length - 2] == 'p' || reader.name[length - 2] == 'P')
            && (reader.name[length - 1] == 'l' || reader.name[length - 1] == 'L')) {
            category = 0x10;
        } else if ((flags & 0x40)
            && (reader.name[length - 3] == 't' || reader.name[length - 3] == 'T')
            && (reader.name[length - 2] == 'l' || reader.name[length - 2] == 'L')
            && (reader.name[length - 1] == 'p' || reader.name[length - 1] == 'P')) {
            category = 0x40;
        } else if ((flags & 0x80)
            && (reader.name[length - 2] == 'f' || reader.name[length - 2] == 'F')
            && reader.name[length - 1] == '2') {
            category = 0x80;
        } else {
            continue;
        }
        if (category != 0) {
            entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
            entry->category = category;
            entry->value = 0;
            entry->id = 0;
            strcpy(entry->name, reader.name);
            if (tail != NULL) {
                tail->next = entry;
            } else {
                list->head = entry;
            }
            entry->next = NULL;
            entry->prev = tail;
            list->count++;
            tail = entry;
        }
    }
    btlClosePfsDebugDirectory(directoryHandle);
    return list;
}

/* Release all scanned entries, the owned path prefix, and the list header. */
void btlDestroyEntryList(SBTLRESOURCEENTRY_LIST *list) {
    SBTLRESOURCE_ENTRY *entry;

    if (list->head != NULL) {
        entry = list->head;
        do {
            SBTLRESOURCE_ENTRY *next = entry->next;
            sdfChipFreeDirect(entry);
            entry = next;
        } while (entry != NULL);
    }
    sdfChipFreeDirect(list->pathPrefix);
    sdfChipFreeDirect(list);
}

/* Append a named browser entry without changing the current scan order. */
void btlAppendEntry(SBTLRESOURCEENTRY_LIST *list, const char *name, s32 category, s32 value, s32 id) {
    SBTLRESOURCE_ENTRY *entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
    SBTLRESOURCE_ENTRY *tail;
    entry->category = category;
    entry->value = value;
    entry->id = id;
    strcpy(entry->name, name);
    if (list->head == NULL) {
        list->head = entry;
        entry->prev = NULL;
        entry->next = NULL;
    } else {
        tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = entry;
        entry->prev = tail;
        entry->next = NULL;
    }
    list->count++;
}

SBTLRESOURCE_DESCRIPTOR *btlCreateResourceDescriptor(SBTLRESOURCEENTRY_LIST *list) {
    SBTLRESOURCE_DESCRIPTOR *descriptor = sdfChipAllocDirect(sizeof(SBTLRESOURCE_DESCRIPTOR));

    descriptor->originX = 8;
    descriptor->originY = 8;
    descriptor->selectionStatus = BTL_RESOURCE_SELECTION_PENDING;
    descriptor->entryCount = list->count;
    descriptor->word10 = 0;
    descriptor->selectedIndex = 0;
    descriptor->visibleIndex = 0;
    descriptor->previewActive = 0;
    descriptor->repeatDelay = 0;
    descriptor->drawSurfaceIndex = BTL_RESOURCE_DRAW_SURFACE;
    descriptor->borderColor = 0x80806020;
    descriptor->fillColor = 0x60000000;
    descriptor->firstVisibleEntry = list->head;
    descriptor->selectedEntry = list->head;
    descriptor->cachedEntry = NULL;
    descriptor->texture = 0;
    descriptor->entryList = list;
    return descriptor;
}

s32 Draw_effLensFlare(SBTLRESOURCE_DESCRIPTOR *descriptor) {
    SBTLRESOURCE_ENTRY *visibleEntry;
    sdfDrawTag_t *packetList;
    sdfGrOt_t *surface;
    s32 drawnRows;
    s32 topRow;
    s32 contentRow;
    s32 packedY;

    if (descriptor->repeatDelay != 0) {
        descriptor->repeatDelay--;
    } else if (descriptor->firstVisibleEntry == NULL) {
        if (dds3PadBitTable[1] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        }
    } else if (descriptor->selectionStatus == BTL_RESOURCE_SELECTION_PENDING) {
        SBTLRESOURCE_ENTRY *navigationEntry;
        SBTLRESOURCE_ENTRY *selectedEntry;
        s32 navigationStep;
        if ((dds3PadBitTable[6] & 2) != 0) {
            if (descriptor->selectedEntry->prev != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->prev;
                descriptor->selectedIndex--;
                if (descriptor->visibleIndex != 0) {
                    descriptor->visibleIndex--;
                }
                if (descriptor->selectedEntry == descriptor->firstVisibleEntry) {
                    if (descriptor->selectedEntry->prev != NULL) {
                        descriptor->firstVisibleEntry = descriptor->selectedEntry->prev;
                        descriptor->visibleIndex = 1;
                    }
                }
            }
        } else if ((dds3PadBitTable[7] & 2) != 0) {
            if (descriptor->selectedEntry->next != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->next;
                descriptor->selectedIndex++;
                descriptor->visibleIndex++;
                if (descriptor->visibleIndex == 0x11) {
                    if (descriptor->firstVisibleEntry->next != NULL) {
                        descriptor->firstVisibleEntry = descriptor->firstVisibleEntry->next;
                        descriptor->visibleIndex = 0x10;
                    }
                }
            }
        } else if ((dds3PadBitTable[9] & 2) != 0) {
            selectedEntry = descriptor->selectedEntry;
            for (navigationStep = 0; navigationStep < 0x12; navigationStep++) {
                navigationEntry = selectedEntry->prev;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex--;
                    if (descriptor->visibleIndex != 0) {
                        descriptor->visibleIndex--;
                    }
                    selectedEntry = descriptor->selectedEntry;
                    if (selectedEntry == descriptor->firstVisibleEntry) {
                        SBTLRESOURCE_ENTRY *previous = selectedEntry->prev;
                        if (previous != NULL) {
                            descriptor->firstVisibleEntry = previous;
                            descriptor->visibleIndex = 1;
                        }
                    }
                }
            }
        } else if ((dds3PadBitTable[11] & 2) != 0) {
            for (navigationStep = 0; navigationStep < 0x12; navigationStep++) {
                navigationEntry = descriptor->selectedEntry->next;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex++;
                    descriptor->visibleIndex++;
                    if (descriptor->visibleIndex == 0x11) {
                        SBTLRESOURCE_ENTRY *next = descriptor->firstVisibleEntry->next;
                        if (next != NULL) {
                            descriptor->firstVisibleEntry = next;
                            descriptor->visibleIndex = 0x10;
                        }
                    }
                }
            }
        } else if (dds3PadBitTable[1] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        } else if (dds3PadBitTable[3] < 0) {
            descriptor->selectionStatus = BTL_RESOURCE_SELECTION_ACCEPTED;
        } else {
            SBTLRESOURCE_ENTRY *cachedEntry = descriptor->cachedEntry;
            selectedEntry = descriptor->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category;
                category = selectedEntry->category;
                switch (category) {
                case BTL_RESOURCE_ENTRY_CATEGORY_TMX:
                    if (selectedEntry->id == 0) {
                        char resourceName[0x70];
                        btlFormatSelectedResourceName(descriptor, resourceName);
                        btlLoadAndReplaceResourceHandle(descriptor, resourceName);
                    } else {
                        btlReplaceResourceHandle(descriptor, (void *)(u32)selectedEntry->id);
                    }
                    selectedEntry = descriptor->selectedEntry;
                    descriptor->previewActive = descriptor->textureCategory = 1;
                    break;
                case BTL_RESOURCE_ENTRY_CATEGORY_GENERAL: {
                    sdfTexHandle_t *texture = dds3BillboardGetGeneralTex(selectedEntry->value);
                    descriptor->textureCategory = category;
                    descriptor->texture = texture;
                    descriptor->previewActive = 1;
                    selectedEntry = descriptor->selectedEntry;
                    break;
                }
                case BTL_RESOURCE_ENTRY_CATEGORY_P2A:
                default:
                    descriptor->previewActive = 0;
                    break;
                }
                descriptor->cachedEntry = selectedEntry;
            }
            if (descriptor->previewActive != 0) {
                btlDrawResourcePreview(descriptor);
            }
        }
    }

    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
        (descriptor->originX << 4) + 0x7000,
        (descriptor->originY << 3) + 0x7900,
        0xFEFFFF, 0xCC0, 0x740, descriptor->fillColor, descriptor->borderColor));

    if (descriptor->previewActive != 0) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
            (descriptor->originX << 4) + 0x7000,
            (descriptor->originY << 3) + 0x8050,
            0xFEFFFF, 0x800, 0x400, 0, descriptor->borderColor));
    }

    drawnRows = 0;
    topRow = descriptor->originY;
    contentRow = topRow + 2;
    if (descriptor->entryList->pathPrefix != NULL) {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            (descriptor->originX << 4) + 0x7020,
            (contentRow << 3) + 0x7900, 0xFF0000, 5,
            descriptor->entryList->pathPrefix));
        contentRow = topRow + 0xE;
    }
    packedY = (contentRow << 3) + 0x7900;
    visibleEntry = descriptor->firstVisibleEntry;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == descriptor->selectedEntry ? 4 : 0;
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            (descriptor->originX << 4) + 0x7020,
            packedY, 0xFF0000, selectionFlags, visibleEntry->name));
        drawnRows++;
        packedY += 0x60;
        if (drawnRows >= 0x12) {
            break;
        }
        visibleEntry = visibleEntry->next;
    }

    surface = &sdfGrOtTable[descriptor->drawSurfaceIndex];
    surface->regFunc(surface, packetList);
    return descriptor->selectionStatus;
}

/* Release an owned texture handle and the descriptor, but not its entry list. */
void btlDestroyResourceDescriptor(SBTLRESOURCE_DESCRIPTOR *descriptor) {
    sdfTexHandle_t *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
    }
    sdfChipFreeDirect(descriptor);
}

void btlSetResourceBrowserPosition(SBTLRESOURCE_DESCRIPTOR *descriptor, s32 x, s32 y) {
    descriptor->originX = x;
    descriptor->originY = y;
}

/* Read the browser descriptor's zero-based selected entry index. */
u32 btlGetResourceBrowserSelectedIndex(
    const SBTLRESOURCE_DESCRIPTOR *descriptor) {
    return descriptor->selectedIndex;
}

u32 btlGetResourceBrowserSelectionStatus(const SBTLRESOURCE_DESCRIPTOR *descriptor) {
    return descriptor->selectionStatus;
}

/* Format prefix + selected name; return its resource category, not its id. */
s32 btlFormatSelectedResourceName(SBTLRESOURCE_DESCRIPTOR *descriptor, char *output) {
    char *pathPrefix = descriptor->entryList->pathPrefix;
    if (pathPrefix != NULL) {
        sprintf(output, D_003BB818, pathPrefix, descriptor->selectedEntry->name);
    } else {
        sprintf(output, D_003BB820, descriptor->selectedEntry->name);
    }
    return descriptor->selectedEntry->category;
}

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(SBTLRESOURCE_DESCRIPTOR *descriptor, char *output) {
    u32 nameLength;
    u32 characterIndex;
    sprintf(output, D_003BB820, descriptor->selectedEntry->name);
    nameLength = strlen(output);
    for (characterIndex = 0; characterIndex < nameLength && output[characterIndex] != '.'; characterIndex++) {
    }
    if (nameLength != characterIndex) {
        output[characterIndex] = 0;
    }
    return descriptor->selectedEntry->category;
}

/* Return the selected entry's category-specific metadata word unchanged. */
u32 btlGetResourcePathVariant(const SBTLRESOURCE_DESCRIPTOR *resource) {
    const SBTLRESOURCE_ENTRY *entry = resource->selectedEntry;
    return entry->value;
}

/* Replace an owned old texture, acquire the named resource's texture,
 * then release the temporary allocation returned by the resource reader. */
void btlLoadAndReplaceResourceHandle(SBTLRESOURCE_DESCRIPTOR *descriptor, const char *resourceName) {
    u32 loadedResource;
    sdfTexHandle_t *texture = descriptor->texture;
    struct sdfMemHandle_t *allocation;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
        descriptor->texture = 0;
    }
    allocation = sdfDevLoadFileEx(resourceName, &loadedResource, 0);
    btlReplaceResourceHandle(descriptor, EE_ADDR_AS(TMX_HEADER_t *, loadedResource));
    sdfMemFreeDirect(allocation);
}

/* Acquire a texture from the supplied resource, releasing an owned old handle. */
void btlReplaceResourceHandle(SBTLRESOURCE_DESCRIPTOR *descriptor, TMX_HEADER_t *textureResource) {
    sdfTexHandle_t *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
        descriptor->texture = 0;
    }
    descriptor->texture = sdfTexSet(textureResource);
}

/* Queue the descriptor's preview texture as a quad at its screen position. */
void btlDrawResourcePreview(SBTLRESOURCE_DESCRIPTOR *descriptor) {
    sdfDrawTag_t *list;
    void *sprite;
    dds3EffectSpriteVertex_t *vertex;
    sdfTexHandle_t *texture;
    sdfGrOt_t *surface;

    if (descriptor->texture != 0) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        sprite = sdf2DAllocSprite(1);
        vertex = (dds3EffectSpriteVertex_t *)sdf2DGetPrimPtr(sprite);
        vertex->a = 0x80;
        vertex->b = 0x80;
        vertex->g = 0x80;
        vertex->r = 0x80;
        texture = descriptor->texture;
        vertex->corner[1].u = texture->width << 4;
        vertex->corner[1].v = texture->height << 4;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[1].flag = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[0].x = (descriptor->originX << 4) + 0x7000;
        vertex->corner[0].y = ((descriptor->originY + 0xEA) << 3) + 0x7900;
        vertex->corner[0].mask = 0xFF0000;
        vertex->corner[1].x = vertex->corner[0].x + 0x800;
        vertex->corner[1].y = vertex->corner[0].y + 0x400;
        vertex->corner[1].mask = 0xFF0000;
        sdf2DLinkTexPacket(list, texture, SDF_GS_CONTEXT_1);
        sdfDrawTagLink(list, (u32)sprite);
        surface = &sdfGrOtTable[descriptor->drawSurfaceIndex];
        surface->regFunc(surface, list);
    }
}

/* Allocate the native name record and initialize its header and extension. */
struct SBTLRESOURCENAME_RECORD *btlCreateResourceNameRecord(const char *extension) {
    struct SBTLRESOURCENAME_RECORD *record;

    record = (struct SBTLRESOURCENAME_RECORD *)sdfChipAllocDirect(BTL_RESOURCE_NAME_RECORD_BYTES);
    record->maximumNameLength = 9;
    record->x = 8;
    record->y = 8;
    record->selectionStatus = BTL_RESOURCE_SELECTION_PENDING;
    record->nameLength = 0;
    record->selection = 0;
    record->overwritePromptActive = 0;
    strcpy(record->extension, extension);
    return record;
}

/* Release the chip allocation used for an editable resource-name record. */
void btlDestroyResourceNameRecord(struct SBTLRESOURCENAME_RECORD *record) {
    sdfChipFreeDirect(record);
}

void btlUpdateResourceNameEditor(SBTLRESOURCENAME_RECORD *record) {
    char text[0x20];
    s32 x = record->x;
    s32 y = record->y;
    s32 editCode = -1;
    s32 deleteCharacter;
    s32 editLimit;
    s32 selection;
    u32 index;
    s32 row;
    s32 column;

    if (record->selectionStatus != BTL_RESOURCE_SELECTION_ACCEPTED) {
        if ((dds3PadBitTable[6] & 2) != 0) {
            if (record->selection == EDITOR_BACKSPACE_CELL) {
                record->selection = EDITOR_KEY_CELLS - 2;
            } else if (record->selection == EDITOR_CONFIRM_CELL) {
                record->selection = EDITOR_BACKSPACE_CELL;
            } else {
                record->selection -= EDITOR_KEY_COLS;
                if (record->selection < 0) {
                    record->selection = EDITOR_CONFIRM_CELL;
                }
            }
        } else if ((dds3PadBitTable[7] & 2) != 0) {
            if (record->selection == EDITOR_BACKSPACE_CELL) {
                record->selection = EDITOR_CONFIRM_CELL;
            } else if (record->selection == EDITOR_CONFIRM_CELL) {
                record->selection = EDITOR_KEY_COLS - 1;
            } else {
                record->selection += EDITOR_KEY_COLS;
                if ((u32)record->selection >= EDITOR_BACKSPACE_CELL) {
                    record->selection = EDITOR_BACKSPACE_CELL;
                }
            }
        } else if ((dds3PadBitTable[4] & 2) != 0) {
            selection = record->selection;
            if ((u32)(selection - EDITOR_BACKSPACE_CELL) >= 2) {
                if (selection % EDITOR_KEY_COLS == 0) {
                    record->selection = selection + EDITOR_KEY_COLS - 1;
                } else {
                    record->selection = selection - 1;
                }
            }
        } else if ((dds3PadBitTable[5] & 2) != 0) {
            selection = record->selection;
            if ((u32)(selection - EDITOR_BACKSPACE_CELL) >= 2) {
                if (selection % EDITOR_KEY_COLS == EDITOR_KEY_COLS - 1) {
                    record->selection = selection - (EDITOR_KEY_COLS - 1);
                } else {
                    record->selection = selection + 1;
                }
            }
        } else if (dds3PadBitTable[3] < 0) {
            selection = record->selection;
            if (selection == EDITOR_CONFIRM_CELL) {
                if (record->nameLength > 0) {
                    record->selectionStatus = BTL_RESOURCE_SELECTION_ACCEPTED;
                }
            } else if (selection == EDITOR_BACKSPACE_CELL) {
                editCode = EDITOR_DELETE_CODE;
            } else if (selection == EDITOR_BLANK_CELL) {
                /* Native001FC4E8 branches directly to shared001FC538. */
                goto next_cell;
            } else {
                editCode = selection;
            }
        } else if (dds3PadBitTable[1] < 0) {
            record->selectionStatus = BTL_RESOURCE_SELECTION_CANCELED;
        }
        if (record->selection == EDITOR_BLANK_CELL) {
        next_cell:
            record->selection++;
        }
        /* Stage the comparison bound before expansion; see docs/idioms.md,
         * "Trace-justified comparison staging: resource-name editor". */
        editLimit = EDITOR_DELETE_CODE - 1;
        deleteCharacter = editLimit < editCode;
        if (deleteCharacter && record->nameLength > 0) {
            record->nameLength--;
        } else if ((u32)editCode < EDITOR_CHARACTER_COUNT) {
            record->resourceName[record->nameLength] = btlUpdateResourceNameEditorData[editCode];
            if (record->nameLength < (s32)record->maximumNameLength) {
                record->nameLength++;
            }
        }
        record->resourceName[record->nameLength] = 0;
    }
    btlDebugDrawRectangle(x, y, 0xA4, 0x14);
    sprintf(text, btlFormatSelectedResourceNameData, record->resourceName, record->extension);
    btlDebugDrawText(x + 4, y + 4, 0, text);
    y += 0x18;
    btlDebugDrawRectangle(x, y, 0xA4, 0x44);
    if (record->overwritePromptActive == 0) {
        x += 4;
        y += 4;
        for (index = 0, row = 0; row < EDITOR_KEY_ROWS; row++) {
            for (column = EDITOR_KEY_COLS - 1; column >= 0; column--) {
                if (index < EDITOR_CHARACTER_COUNT) {
                    s32 color = record->selection == index ? 4 : 0;
                    btlUpdateResourceNameEditorData2[0] = btlUpdateResourceNameEditorData[index];
                    btlDebugDrawText(x, y, color, btlUpdateResourceNameEditorData2);
                }
                x += 12;
                index++;
            }
            x -= EDITOR_KEY_COLS * 12;
            y += 12;
        }
        x += 0x84;
        btlDebugDrawText(x, y, record->selection == index++ ? 4 : 0, btlUpdateResourceNameEditorData3);
        btlDebugDrawText(x, y + 12, record->selection == index ? 4 : 0, fmData);
    }
}

void btlSetResourceNamePosition(struct SBTLRESOURCENAME_RECORD *record, s32 x, s32 y) {
    record->x = x;
    record->y = y;
}

u32 btlGetResourceNameSelectionStatus(const struct SBTLRESOURCENAME_RECORD *record) {
    return record->selectionStatus;
}

/* Copy the resource name at +0x21 and store its length. */
void btlResourceRecordSetName(struct SBTLRESOURCENAME_RECORD *record, const char *name) {
    strcpy(record->resourceName, name);
    record->nameLength = strlen(name);
}

void btlFormatResourceNameWithExtension(const struct SBTLRESOURCENAME_RECORD *record, char *output) {
    sprintf(output, D_003BB818, record->resourceName, record->extension);
}

void btlFormatResourceNameWithoutExtension(const struct SBTLRESOURCENAME_RECORD *record, char *output) {
    sprintf(output, D_003BB820, record->resourceName);
}

void btlSetResourceNameLengthLimit(struct SBTLRESOURCENAME_RECORD *record, u32 maximumNameLength) {
    record->maximumNameLength = maximumNameLength;
}

INCLUDE_ASM(const s32, "code_r1_0017BBD8", fmOVERWRITEOK);

#ifdef VERSION_RAIDOU1
/* pb debug menus (DEBUG MENU and its field light, point light and camera pages); see effDebugMenu_t. */
extern u32 pbLeaf10Data2Entry14;
extern u32 pbLeaf10Data2Entry15;
extern char pbLeaf10Data2Entry16[];
extern char pbLeaf10Data2Entry17[];
extern char pbLeaf10Data2Entry18[];
extern char pbLeaf10Data2Entry19[];
extern char pbLeaf10Data2Entry20[];
extern char pbLeaf10Data2Entry21[];
extern char pbLeaf10Data2Entry22[];
extern char pbLeaf10Data2Entry23[];
extern u32 pbLeaf10Data2Entry24;
extern u32 pbLeaf10Data2Entry25;
extern u8 pbLeaf10Data2Entry26[];
extern char pbLeaf10Data2Entry27[];
extern char pbLeaf10Data2Entry28[];
extern char pbLeaf10Data2Entry29[];
extern char pbLeaf10Data2Entry30[];
extern char pbLeaf10Data2Entry31[];
extern char pbLeaf10Data2Entry32[];
extern u32 pbLeaf10Data2Entry3;
extern f32 pbLeaf10Data2Entry4;
extern f32 pbLeaf10Data2Entry5;
extern f32 pbLeaf10Data2Entry6;
extern char pbLeaf10Data2Entry7[];
extern char pbLeaf10Data2Entry8[];
extern char pbLeaf10Data2Entry9[];
extern char pbLeaf10DataEntry10[];
extern char pbLeaf10DataEntry11[];
extern char pbLeaf10DataEntry12[];
extern char pbLeaf10DataEntry13[];
extern char pbLeaf10DataEntry14[];
extern f32 pbLeaf10DataEntry15;
extern f32 pbLeaf10DataEntry16;
extern f32 pbLeaf10DataEntry17;
extern f32 pbLeaf10DataEntry18;
extern f32 pbLeaf10DataEntry19;
extern f32 pbLeaf10DataEntry20;
extern f32 pbLeaf10DataEntry21;
extern f32 pbLeaf10DataEntry22;
extern f32 pbLeaf10DataEntry23;
extern f32 pbLeaf10DataEntry24;
extern f32 pbLeaf10DataEntry25;
extern f32 pbLeaf10DataEntry26;
extern f32 pbLeaf10DataEntry27;
extern f32 pbLeaf10DataEntry28;
extern f32 pbLeaf10DataEntry2;
extern f32 pbLeaf10DataEntry3;
extern char pbLeaf10DataEntry4[];
extern char pbLeaf10DataEntry5[];
extern char pbLeaf10DataEntry6[];
extern char pbLeaf10DataEntry7[];
extern char pbLeaf10DataEntry8[];
extern char pbLeaf10DataEntry9[];
extern u32 pbLeaf3Data;
extern char pbStr1Shot[];
extern char pbStrAi[];
extern char pbStrAllCrticl[];
extern char pbStrAllDead[];
extern char pbStrAllHansya[];
extern char pbStrAllHide[];
extern char pbStrAllHit[];
extern char pbStrAllKyusyu[];
extern char pbStrAllSake[];
extern char pbStrAllSelect[];
extern char pbStrAllSkill[];
extern char pbStrBad[];
extern char pbStrBgm[];
extern char pbStrCamera[];
extern char pbStrChange[];
extern char pbStrCut23Fix[];
extern char pbStrCylinderC[];
extern char pbStrCylinderF[];
extern char pbStrDamage[];
extern char pbStrDebugMenu[];
extern char pbStrEffectEdit[];
extern char pbStrEnemyComm[];
extern char pbStrEnemyParty[];
extern char pbStrFieldLight[];
extern char pbStrFldFilter[];
extern char pbStrPointLight[];
extern char pbStrPress[];

extern effDebugMenuItem_t pbLeaf10Data2Entry12[];
extern effDebugMenu_t pbLeaf10Data2Entry10;
extern effDebugMenuItem_t pbLeaf10Data2Entry13[];
extern effDebugMenu_t pbLeaf10Data2Entry11;
extern effDebugMenuItem_t pbLeaf10DataEntry[];
extern effDebugMenu_t pbLeaf10Data;
extern effDebugMenuItem_t pbLeaf10Data2Entry[];
extern effDebugMenu_t pbLeaf10Data2;

effDebugMenuItem_t pbLeaf10Data2Entry12[13] = {
    { pbLeaf10Data2Entry23, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x20000000 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10Data2Entry22, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10Data2Entry21, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry14 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry20, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry14 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry19, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry14 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry18, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10Data2Entry17, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10Data2Entry21, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry15 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry20, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry15 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry19, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry15 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry16, EFF_DEBUG_ITEM_ALPHA, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry15 }, { .minValue = 0 }, 0, 1.0f },
};

effDebugMenu_t pbLeaf10Data2Entry10 = {
    NULL, 184, 4, 0, 0x0, pbLeaf10Data2Entry12, 13, NULL,
    { .value = pbLeaf4 }, pbLeaf5, pbLeaf6, NULL, { 0 }, NULL,
};

effDebugMenuItem_t pbLeaf10Data2Entry13[18] = {
    { pbLeaf10Data2Entry23, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x40000000 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10Data2Entry32, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10Data2Entry21, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry24 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry20, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry24 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry19, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry24 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry18, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10Data2Entry31, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10Data2Entry21, EFF_DEBUG_ITEM_RED, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry25 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry20, EFF_DEBUG_ITEM_GREEN, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry25 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry19, EFF_DEBUG_ITEM_BLUE, { 0 }, { .rgbaValue = &pbLeaf10Data2Entry25 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry18, EFF_DEBUG_ITEM_BLANK },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10Data2Entry30, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10Data2Entry29, EFF_DEBUG_ITEM_BYTE, { 0 }, { .u8Value = &pbLeaf10Data2Entry26[0] }, { .minValue = 0 }, 8, 1.0f },
    { pbLeaf10Data2Entry28, EFF_DEBUG_ITEM_BYTE, { 0 }, { .u8Value = &pbLeaf10Data2Entry26[1] }, { .minValue = 0 }, 5, 1.0f },
    { pbLeaf10Data2Entry27, EFF_DEBUG_ITEM_BYTE, { 0 }, { .u8Value = &pbLeaf10Data2Entry26[2] }, { .minValue = 0 }, 0xA, 1.0f },
};

effDebugMenu_t pbLeaf10Data2Entry11 = {
    NULL, 184, 4, 0, 0x0, pbLeaf10Data2Entry13, 18, NULL,
    { 0 }, pbLeaf7, pbLeaf8, NULL, { 0 }, NULL,
};

effDebugMenuItem_t pbLeaf10DataEntry[20] = {
    { pbLeaf10DataEntry14, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry2 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10DataEntry13, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry3 }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10DataEntry12, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10DataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry15 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry10, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry16 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry17 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry8, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry21 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry7, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry22 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry6, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry23 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry5, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry24 }, { .minValue = -1 }, -1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbLeaf10DataEntry4, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10DataEntry11, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry18 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry10, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry19 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10DataEntry20 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry8, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry25 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry7, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry26 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry6, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry27 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10DataEntry5, 0x26, { 0 }, { .f32Value = &pbLeaf10DataEntry28 }, { .minValue = -1 }, -1, 1.0f },
};

effDebugMenu_t pbLeaf10Data = {
    NULL, 184, 4, 0, 0x0, pbLeaf10DataEntry, 20, NULL,
    { 0 }, pbLeaf9, pbLeaf10, NULL, { 0 }, NULL,
};

effDebugMenuItem_t pbLeaf10Data2Entry[34] = {
    { pbStrChange, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x40 }, 0, 1.0f },
    { pbStrDamage, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x80 }, 0, 1.0f },
    { pbStrPress, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x100 }, 0, 1.0f },
    { pbStrBad, 0x0A, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x80 }, 0, 1.0f },
    { pbStrAllKyusyu, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 8 }, 0, 1.0f },
    { pbStrAllHansya, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x10 }, 0, 1.0f },
    { pbStrAllSake, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x20 }, 0, 1.0f },
    { pbStrAllHit, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x40 }, 0, 1.0f },
    { pbStrAllCrticl, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x1000 }, 0, 1.0f },
    { pbStrAllDead, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x200000 }, 0, 1.0f },
    { pbStrAllSkill, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x1000000 }, 0, 1.0f },
    { pbStrAllSelect, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x800 }, 0, 1.0f },
    { pbStrEnemyComm, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 1 }, 0, 1.0f },
    { pbStrAi, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 2 }, 0, 1.0f },
    { pbStrBgm, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x10000 }, 0, 1.0f },
    { pbStrCamera, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x400 }, 0, 1.0f },
    { pbStrAllHide, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x4000000 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbStrCylinderC, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x400000 }, 0, 1.0f },
    { pbStrCylinderF, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf10Data2Entry3 }, { .mask = 0x200 }, 0, 1.0f },
    { pbStr1Shot, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x800000 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbStrEnemyParty, EFF_DEBUG_ITEM_BLANK },
    { pbLeaf10Data2Entry9, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10Data2Entry4 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10Data2Entry28, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10Data2Entry5 }, { .minValue = -1 }, -1, 1.0f },
    { pbLeaf10Data2Entry8, EFF_DEBUG_ITEM_FLOAT, { 0 }, { .f32Value = &pbLeaf10Data2Entry6 }, { .minValue = -1 }, -1, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbStrCut23Fix, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x10000000 }, 0, 1.0f },
    { pbStrFldFilter, EFF_DEBUG_ITEM_BIT, { 0 }, { .bitValue = &pbLeaf3Data }, { .mask = 0x20000 }, 0, 1.0f },
    { pbStrFieldLight, 0x18, { 0 }, { .child = &pbLeaf10Data2Entry10 }, { .minValue = 0 }, 0, 1.0f },
    { pbStrPointLight, 0x18, { 0 }, { .child = &pbLeaf10Data2Entry11 }, { .minValue = 0 }, 0, 1.0f },
    { pbLeaf10Data2Entry7, 0x18, { 0 }, { .child = &pbLeaf10Data }, { .minValue = 0 }, 0, 1.0f },
    { NULL, EFF_DEBUG_ITEM_SEPARATOR },
    { pbStrEffectEdit, EFF_DEBUG_ITEM_COMMAND, { 0 }, { .command = pbLeaf3 }, { .color = 0x0 } },
};

effDebugMenu_t pbLeaf10Data2 = {
    pbStrDebugMenu, 8, 4, 0, 0x0, pbLeaf10Data2Entry, 34, NULL,
    { 0 }, NULL, NULL, NULL, { 0 }, NULL,
};

extern char dds3EffectFileNameTable2Entry[];
extern char dds3EffectFileNameTable2Entry2[];
extern char dds3EffectFileNameTable2Entry3[];
extern char dds3EffectFileNameTable2Entry4[];
extern char dds3EffectFileNameTable2Entry5[];
extern char dds3EffectFileNameTable2Entry6[];
extern char dds3EffectFileNameTable2Entry7[];
extern char dds3EffectFileNameTable2Entry8[];
extern char dds3EffectFileNameTable2Entry9[];
extern char dds3StrAgOutEpl[];
extern char dds3StrAgiTobaEpl[];
extern char dds3StrAgiball00Epl[];
extern char dds3StrAgiball01Epl[];
extern char dds3StrAgsword31Epl[];
extern char dds3StrAgsword32Epl[];
extern char dds3StrAgsword33Epl[];
extern char dds3StrB12Hit00Epl[];
extern char dds3StrB12Hit01Epl[];
extern char dds3StrB12Hit02Epl[];
extern char dds3StrB12Hit03Epl[];
extern char dds3StrBRange00Epl[];
extern char dds3StrBad01Epl[];
extern char dds3StrBad04Epl[];
extern char dds3StrBfInEpl[];
extern char dds3StrBhudainAEpl[];
extern char dds3StrBigDamYEpl[];
extern char dds3StrBlood00Epl[];
extern char dds3StrBlood02Epl[];
extern char dds3StrBuhmain00Epl[];
extern char dds3StrConbAuraEpl[];
extern char dds3StrDamYuka1Epl[];
extern char dds3StrDamYuka2Epl[];
extern char dds3StrDamYukaEpl[];
extern char dds3StrDamagMuaEpl[];
extern char dds3StrDamagMubEpl[];
extern char dds3StrEffTesEpl[];
extern char dds3StrFuDevilEpl[];
extern char dds3StrFuRaidohEpl[];
extern char dds3StrFuYukaEpl[];
extern char dds3StrKaitenEpl[];
extern char dds3StrKudanEpl[];
extern char dds3StrLaunch00Epl[];
extern char dds3StrLightEpl[];
extern char dds3StrLightaEp[];
extern char dds3StrLightaEpl[];
extern char dds3StrMhbfdinEpl[];
extern char dds3StrMhmudoEpl[];
extern char dds3StrMudshotEpl[];
extern char dds3StrNoooEpl[];
extern char dds3StrRevtinaEpl[];
extern char dds3StrShoukanEpl[];
extern char dds3StrStopHelperPb[];
extern char dds3StrTest02Epl[];
extern char dds3StrTest0Epl[];
extern char dds3StrTest1Epl[];
extern char dds3StrTest2Epl[];
extern char dds3StrTest3Epl[];
extern char dds3StrTestEpl[];
extern char dds3StrTestP2A[];
extern char dds3StrTestTlp[];
extern char dds3StrTestTmx[];
extern char dds3StrYuka02Fix02Pb[];

char *dds3EffectFileNameTable2[62] = {
    dds3StrDamagMuaEpl,
    dds3StrDamagMubEpl,
    dds3StrBad01Epl,
    dds3StrBad04Epl,
    dds3StrConbAuraEpl,
    dds3StrMhmudoEpl,
    dds3StrKudanEpl,
    dds3StrEffTesEpl,
    dds3StrDamYukaEpl,
    dds3StrDamYuka1Epl,
    dds3StrDamYuka2Epl,
    dds3StrBlood00Epl,
    dds3StrBlood02Epl,
    dds3EffectFileNameTable2Entry9,
    dds3EffectFileNameTable2Entry8,
    dds3StrBuhmain00Epl,
    dds3StrB12Hit00Epl,
    dds3StrB12Hit01Epl,
    dds3StrB12Hit02Epl,
    dds3StrB12Hit03Epl,
    dds3StrFuYukaEpl,
    dds3StrFuRaidohEpl,
    dds3StrFuDevilEpl,
    dds3StrBRange00Epl,
    dds3StrAgiball00Epl,
    dds3StrAgiball01Epl,
    dds3StrLaunch00Epl,
    dds3StrKaitenEpl,
    dds3StrNoooEpl,
    dds3StrTestTmx,
    dds3EffectFileNameTable2Entry7,
    dds3EffectFileNameTable2Entry6,
    dds3StrYuka02Fix02Pb,
    dds3StrStopHelperPb,
    dds3EffectFileNameTable2Entry5,
    dds3EffectFileNameTable2Entry4,
    dds3EffectFileNameTable2Entry3,
    dds3StrTestEpl,
    dds3StrMudshotEpl,
    dds3StrTest02Epl,
    dds3StrRevtinaEpl,
    dds3StrAgiTobaEpl,
    dds3StrLightEpl,
    dds3StrLightaEpl,
    dds3StrLightaEp,
    dds3StrAgsword31Epl,
    dds3StrAgsword32Epl,
    dds3StrAgsword33Epl,
    dds3StrBigDamYEpl,
    dds3StrBfInEpl,
    dds3StrBhudainAEpl,
    dds3StrShoukanEpl,
    dds3StrTest0Epl,
    dds3StrTest1Epl,
    dds3StrTest2Epl,
    dds3StrTest3Epl,
    dds3StrMhbfdinEpl,
    dds3StrTestP2A,
    dds3EffectFileNameTable2Entry2,
    dds3StrAgOutEpl,
    dds3StrTestTlp,
    dds3EffectFileNameTable2Entry,
};
#endif
INCLUDE_SDATA(const s32, "code_r1_0017BBD8", btlFormatSelectedResourceNameData);

INCLUDE_SDATA(const s32, "code_r1_0017BBD8", btlFormatSelectedResourceNameData2);

INCLUDE_SDATA(const s32, "code_r1_0017BBD8", btlUpdateResourceNameEditorData2);

INCLUDE_SDATA(const s32, "code_r1_0017BBD8", btlUpdateResourceNameEditorData3);

INCLUDE_SDATA(const s32, "code_r1_0017BBD8", fmData);

INCLUDE_SDATA(const s32, "code_r1_0017BBD8", fmOVERWRITEOKData);

INCLUDE_SDATA(const s32, "code_r1_0017BBD8", fmOVERWRITEOKData2);

