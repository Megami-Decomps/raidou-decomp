#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "dds/sdf_column_packet_api.h"
#include "dds/sdf_texture_file.h"
#include "dds/sdk_libkernl.h"

s32 sceDclose(s32 directory);
s32 sceDopen(const char *path);

/* Allocate a variable-length sprite column packet with this draw header. */
SdfDrawPacket *sdf2DAllocSprite(s32 loopCount);
SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);
void *sdfChipAllocDirect(s32 size);
void sdfChipFreeDirect(void *memory);
void sdfDrawTagInit(struct sdfDrawTag_t *list);
void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);
char *strcpy(char *dst, const char *src);
u32 strlen(const char *text);

#include "dds/btl_command.h"
#include "dds/dds3_effect_sprite.h"
#include "dds/btl_resource_browser.h"
#include "dds/ee_mmi.h"
#include "dds/btl_resource_name.h"
#include "dds/fpu.h"

enum {
    BTL_RESOURCE_DESCRIPTOR_BYTES = 0x48,
    BTL_RESOURCE_NAME_RECORD_BYTES = 0x38,
    BTL_PACKED_SCREEN_X_BIAS = 0x7000,
    BTL_PACKED_SCREEN_Y_BIAS = 0x7900,
    BTL_HP_PERCENT_SCALE = 100,
    BTL_DIRECTORY_PATH_BYTES = 0x70,
    BTL_BUILTIN_NAME_COUNT = 8,
    BTL_DIRECTORY_FLAG_CLEAR = 0x1000,
    BTL_RESOURCE_ENTRY_BYTES = sizeof(BtlResourceEntry),
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

typedef struct BtlActor {
    u8 pad00[0x18];
    BtlUnit *unit;
} BtlActor;

typedef struct BtlList {
    u8 pad00[0x20];
    s32 count;
} BtlList;

typedef struct BtlCmdCtx {
    u8 pad00[0xC];
    u32 flags; /* 0x0C: command-context flags */
    u8 pad10[8];
    BtlUnit *unit;
    u8 pad1C[4];
    s32 commandMode;
    s32 commandValue;
    u8 pad28[4];
    BtlUnit *actionProbeFirst;
    BtlUnit *actionProbeSecond;
    u8 pad34[4];
    u32 parameter38; /* 0x38 */
    u8 pad3C[0x50];
    u32 parameter8C; /* 0x8C */
} BtlCmdCtx;

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

struct BtlRuntimeTask;

typedef struct BtlControlObject {
    u8 enabled; /* 0x00 */
    u8 pad01[0x0F];
    u8 state; /* 0x10 */
    u8 pad11[0x0F];
    u16 type; /* 0x20 */
    u8 pad22[0x26];
    s32 status; /* 0x48 */
    void (*update)(void); /* 0x4C */
} BtlControlObject;

/* Select the smallest X for reference flag 0x200, otherwise the largest.
 * An empty indexed list returns NULL but still loads the native scratch vector. */

struct RefObj;

typedef struct BtnUv {
    s32 u;
    s32 v;
} BtnUv;

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
typedef struct BtlCommandContext {
    u8 pad00[0x18];
    BtlUnit *actor; /* 0x18: the command actor's unit. */
    u8 pad1C[0x74];
    u32 selectionFlagsA; /* 0x90 */
    u32 selectionFlagsB; /* 0x94 */
    s32 choicesA[26]; /* 0x98..0xFC */
    s32 choicesB[17]; /* 0x100..0x140 */
} BtlCommandContext;

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
void btlBossDebugPrintfN(s32 x, s32 y, s32 color, s32 format, ...);
s32 btlOpenPfsDebugDirectory(const char *directoryName);
void btlClosePfsDebugDirectory(s32 directoryHandle);

/* Debug mode delegates to the native reader. Built-in mode returns each name's length,
   clears the directory-type bit, and returns zero when the eight-name table is exhausted. */
s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, SceDirent *reader);
extern u32 dds3BillboardGeneralNumber(void);
BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags);
void btlDestroyEntryList(BtlResourceEntryList *list);
void btlAppendEntry(BtlResourceEntryList *list, const char *name, s32 category, s32 value, s32 id);
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *, char *);
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *, const char *);
void btlReplaceResourceHandle(BtlResourceDescriptor *, void *);
void btlDestroyResourceDescriptor(BtlResourceDescriptor *descriptor);
void btlSetResourceBrowserPosition(BtlResourceDescriptor *descriptor, s32 x, s32 y);

u32 btlGetResourceBrowserSelectedIndex(
    const BtlResourceDescriptor *descriptor);

u32 btlGetResourceBrowserSelectionStatus(const BtlResourceDescriptor *descriptor);
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *descriptor, char *output);
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output);
u32 btlGetResourcePathVariant(const BtlResourceDescriptor *resource);
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *descriptor, const char *resourceName);
void btlReplaceResourceHandle(BtlResourceDescriptor *descriptor, void *textureResource);
extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);
void btlDrawResourcePreview(BtlResourceDescriptor *descriptor);
struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension);
void btlDestroyResourceNameRecord(struct BtlResourceNameRecord *record);

#define EDITOR_KEY_COLS 13

#define EDITOR_KEY_ROWS 3

#define EDITOR_KEY_CELLS (EDITOR_KEY_COLS * EDITOR_KEY_ROWS)

#define EDITOR_CHARACTER_COUNT (EDITOR_KEY_CELLS - 1)

#define EDITOR_BLANK_CELL (EDITOR_KEY_CELLS - 1)

#define EDITOR_BACKSPACE_CELL EDITOR_KEY_CELLS

#define EDITOR_CONFIRM_CELL (EDITOR_KEY_CELLS + 1)

#define EDITOR_DELETE_CODE (EDITOR_KEY_CELLS + 3)

void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y);
u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record);
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name);
void btlFormatResourceNameWithExtension(const struct BtlResourceNameRecord *record, char *output);
void btlFormatResourceNameWithoutExtension(const struct BtlResourceNameRecord *record, char *output);
void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength);

void btlBossDebugPrintfN(s32 x, s32 y, s32 color, s32 format, ...) {
}

void btlDebugDrawText(void) {
}

void btlDebugDrawRectangle(void) {
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

void btlDebugDrawPoint(void) {
}

void pbLeaf16(void) {
}

void btlDebugDrawRadiusGuide(void) {
}

void btlDebugDrawOrientedExtentGuide(void) {
}

void btlDebugDrawScaledPositionGuide(void) {
}

void pbLeaf17(void) {
}

void btlDebugDrawSegment(void) {
}

void btlDebugMenuBeforeRecordNoOp(void) {
}

void btlDebugMenuAfterRecordNoOp(void) {
}

INCLUDE_ASM(const s32, "code_0017BBD8", pbLeaf18);

void btlDebugMenuRecordNoOp(void) {
}

INCLUDE_ASM(const s32, "code_0017BBD8", btlFindEntryByCommand);

INCLUDE_ASM(const s32, "code_0017BBD8", btlCountNonPartOpcodes);

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

INCLUDE_ASM(const s32, "code_0017BBD8", btlReadBattleResourceDirectoryEntry);

/* Build a scan-order list of built-in names and enabled resource-file categories. */
BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags) {
    SceDirent reader;
    BtlResourceEntryList *list;
    BtlResourceEntry *entry;
    BtlResourceEntry *tail;
    s32 directoryHandle;
    s32 generalCount;
    s32 index;
    s32 length;
    s32 category;

    directoryHandle = btlOpenPfsDebugDirectory(path);
    list = sdfChipAllocDirect(sizeof(BtlResourceEntryList));
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
void btlDestroyEntryList(BtlResourceEntryList *list) {
    BtlResourceEntry *entry;

    if (list->head != NULL) {
        entry = list->head;
        do {
            BtlResourceEntry *next = entry->next;
            sdfChipFreeDirect(entry);
            entry = next;
        } while (entry != NULL);
    }
    sdfChipFreeDirect(list->pathPrefix);
    sdfChipFreeDirect(list);
}

/* Append a named browser entry without changing the current scan order. */
void btlAppendEntry(BtlResourceEntryList *list, const char *name, s32 category, s32 value, s32 id) {
    BtlResourceEntry *entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
    BtlResourceEntry *tail;
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

INCLUDE_ASM(const s32, "code_0017BBD8", btlCreateResourceDescriptor);

INCLUDE_ASM(const s32, "code_0017BBD8", Draw_effLensFlare);

/* Release an owned texture handle and the descriptor, but not its entry list. */
void btlDestroyResourceDescriptor(BtlResourceDescriptor *descriptor) {
    sdfTexHandle_t *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
    }
    sdfChipFreeDirect(descriptor);
}

void btlSetResourceBrowserPosition(BtlResourceDescriptor *descriptor, s32 x, s32 y) {
    descriptor->originX = x;
    descriptor->originY = y;
}

/* Read the browser descriptor's zero-based selected entry index. */
u32 btlGetResourceBrowserSelectedIndex(
    const BtlResourceDescriptor *descriptor) {
    return descriptor->selectedIndex;
}

u32 btlGetResourceBrowserSelectionStatus(const BtlResourceDescriptor *descriptor) {
    return descriptor->selectionStatus;
}

/* Format prefix + selected name; return its resource category, not its id. */
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *descriptor, char *output) {
    char *pathPrefix = descriptor->entryList->pathPrefix;
    if (pathPrefix != NULL) {
        sprintf(output, D_003BB818, pathPrefix, descriptor->selectedEntry->name);
    } else {
        sprintf(output, D_003BB820, descriptor->selectedEntry->name);
    }
    return descriptor->selectedEntry->category;
}

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output) {
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
u32 btlGetResourcePathVariant(const BtlResourceDescriptor *resource) {
    const BtlResourceEntry *entry = resource->selectedEntry;
    return entry->value;
}

/* Replace an owned old texture, acquire the named resource's texture,
 * then release the temporary allocation returned by the resource reader. */
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *descriptor, const char *resourceName) {
    u32 loadedResource;
    sdfTexHandle_t *texture = descriptor->texture;
    struct sdfMemHandle_t *allocation;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
        descriptor->texture = 0;
    }
    allocation = sdfDevLoadFileEx(resourceName, &loadedResource, 0);
    btlReplaceResourceHandle(descriptor, (void *)loadedResource);
    sdfMemFreeDirect(allocation);
}

/* Acquire a texture from the supplied resource, releasing an owned old handle. */
void btlReplaceResourceHandle(BtlResourceDescriptor *descriptor, void *textureResource) {
    sdfTexHandle_t *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
        descriptor->texture = 0;
    }
    descriptor->texture = sdfTexSet((SdfTextureFileHeader *)(textureResource));
}

/* Queue the descriptor's preview texture as a quad at its screen position. */
void btlDrawResourcePreview(BtlResourceDescriptor *descriptor) {
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
struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension) {
    struct BtlResourceNameRecord *record;

    record = (struct BtlResourceNameRecord *)sdfChipAllocDirect(BTL_RESOURCE_NAME_RECORD_BYTES);
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
void btlDestroyResourceNameRecord(struct BtlResourceNameRecord *record) {
    sdfChipFreeDirect(record);
}

INCLUDE_ASM(const s32, "code_0017BBD8", btlUpdateResourceNameEditor);

void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y) {
    record->x = x;
    record->y = y;
}

u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record) {
    return record->selectionStatus;
}

/* Copy the resource name at +0x21 and store its length. */
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name) {
    strcpy(record->resourceName, name);
    record->nameLength = strlen(name);
}

void btlFormatResourceNameWithExtension(const struct BtlResourceNameRecord *record, char *output) {
    sprintf(output, D_003BB818, record->resourceName, record->extension);
}

void btlFormatResourceNameWithoutExtension(const struct BtlResourceNameRecord *record, char *output) {
    sprintf(output, D_003BB820, record->resourceName);
}

void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength) {
    record->maximumNameLength = maximumNameLength;
}

INCLUDE_ASM(const s32, "code_0017BBD8", fmOVERWRITEOK);
INCLUDE_SDATA(const s32, "code_0017BBD8", btlFormatSelectedResourceNameData);

INCLUDE_SDATA(const s32, "code_0017BBD8", btlFormatSelectedResourceNameData2);

INCLUDE_SDATA(const s32, "code_0017BBD8", btlUpdateResourceNameEditorData2);

INCLUDE_SDATA(const s32, "code_0017BBD8", btlUpdateResourceNameEditorData3);

INCLUDE_SDATA(const s32, "code_0017BBD8", fmData);

INCLUDE_SDATA(const s32, "code_0017BBD8", fmOVERWRITEOKData);

INCLUDE_SDATA(const s32, "code_0017BBD8", fmOVERWRITEOKData2);

