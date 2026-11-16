#ifndef CODE_0027_A1_D0_H
#define CODE_0027_A1_D0_H

#include "common.h"

/* Retained command operands. The task constructors copy the complete operand after the actor pointer. */
typedef struct SBTLOPERAND_ENTRY {
    s32 hpDelta;
    s32 mpDelta;
    u32 addedStatus;
    u32 removedStatus;
    u32 entryChangeMask;
    s16 entryChange;
    u16 entryChangeFlags;
    s32 entrySelection; /* Counter command to retain; zero means no selected entry. */
    s32 hpRecovery;
    s32 mpRecovery;
    u16 deferredStatus;
    u16 flags;
} SBTLOPERAND_ENTRY;

/* The command work owns thirteen groups. */
typedef struct SBTLOPERAND_GROUP {
    u8 count;
    u8 pad01[3];
    s32 interval;
    s32 kind;
    s32 parameter;
    /* Dispatch tests both bytes with lhu; reset and population use sb. */
    union {
        u16 inactiveOrStatusChanged;
        struct {
            u8 inactive;
            u8 statusChanged;
        };
    };
    u8 targetSpecialHit;
    u8 sourceSpecialHit;
    u8 reflected; /* Population prints "btl:HANSYA" when this is set. */
    u8 reflectionKind;
    u8 pad16[2];
    s32 reactionCode;
    SBTLOPERAND_ENTRY entries[64];
} SBTLOPERAND_GROUP;

/* The five base-stat slots used by party/enemy records and matching stat
 * vectors. These are not affinity-table selectors or script IDs. */
typedef enum DatBaseStatIndex {
    DAT_BASE_STAT_STRENGTH = 0,
    DAT_BASE_STAT_VITALITY = 1,
    DAT_BASE_STAT_MAGIC = 2,
    DAT_BASE_STAT_AGILITY = 3,
    DAT_BASE_STAT_LUCK = 4,
    DAT_BASE_STAT_COUNT = 5
} DatBaseStatIndex;

/* Actor record (0x1A4), not the 0xEC datUnitWork_t of the stock. The active
 * roster and saved template bank share it. */
typedef struct datActorWork_t {
    u16 flags;
    u16 affinityTableIndex;
    u16 unitId; /* 0x04: DatUnitId key stored at its native 16-bit width. */
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxMp;
    u16 status;
    u32 totalExp;
    u16 level;
    s8 baseStats[DAT_BASE_STAT_COUNT];
    u8 pad1B;
    u16 hpBonus;
    u16 mpBonus;
    u8 effectDataCount; /* Enemy constructor count; custom party setup clears it. */
    u8 pad21;
    u16 effectData[24];
    u16 menuValue;
    u8 pad54;
    s8 profileId;
    u8 pad56[2];
    union {
        struct {
            u32 skillFlags[76]; /* Eight four-bit states per word. */
            u8 pad188[4];
        };
        u32 skillSnapshotWords[77]; /* Stock merge includes the reserved trailing word. */
    };
    u16 savedCommandPhase; /* +0x18C: saved action phase restored with actionSlot. */
    u16 savedCommandSelectionEntry; /* +0x18E: selected list entry restored with savedCommandPhase. */
    u16 actionSlot; /* 0x190: saved command-table slot */
    u8 pad192[2];
    s32 randomizedValue;
    s32 huntExp; /* 0x198: hunt EP accumulated this battle (shared by ability 0x24C) */
    u8 pad19C[8];
} datActorWork_t;

typedef char DatPartyRecordSizeCheck[sizeof(datActorWork_t) == 0x1A4 ? 1 : -1];

/* The actor owns both status words as one complete, naturally aligned record.
 * Scalar accesses retain their ordinary word alias behavior. Packed observations
 * copy the whole record's representation, without casting a scalar member. */
typedef struct SBTLUNIT_STATUS {
    s32 flags;
    u32 stateFlags;
} SBTLUNIT_STATUS;

/* Six-byte status-entry record; each game's battle unit contains seven. */
typedef struct SBTLUNITENTRY_SLOT {
    s16 code;
    s16 initialValue;
    s16 countdown;
} SBTLUNITENTRY_SLOT;

/* Loaded as a pair with LD; SH flags and SW reference counts also access its halves. */
typedef union BtlEffectLinkState {
    u64 packed;
    struct {
        u16 flags;
        u8 pad02[2];
        s32 referenceCount;
    };
} BtlEffectLinkState;

/* Both titles keep the primary owner's existing eight-byte alignment. */
typedef char SBTLUNIT_STATUSSizeCheck[(sizeof(SBTLUNIT_STATUS) == 8) ? 1 : -1];

#ifdef VERSION_RAIDOU2
/* Three-byte scene scheduling slot; IDs move with their group/countdown. */
typedef struct SBTLSCENE_SLOT {
    u8 group;
    u8 remaining;
    u8 id;
} SBTLSCENE_SLOT;
#endif

#ifdef VERSION_RAIDOU2
typedef struct SBTLSCENEFADING_RECORD {
    SBTLSCENE_SLOT slot;
    u8 alpha;
    s32 target;
} SBTLSCENEFADING_RECORD;
#endif

#ifdef VERSION_RAIDOU2
typedef char SBTLSCENE_SLOTSizeCheck[(sizeof(SBTLSCENE_SLOT) == 3) ? 1 : -1];
#endif

#ifdef VERSION_RAIDOU2
typedef char SBTLSCENEFADING_RECORDSizeCheck[(sizeof(SBTLSCENEFADING_RECORD) == 8) ? 1 : -1];
#endif

typedef struct SBTL_UNIT SBTL_UNIT;

typedef struct SBTLINDEX_LIST {
    s32 capacity;       /* 0x00: allocated entry count */
    s32 count;          /* 0x04: live entry count */
    struct SBTL_UNIT **entries; /* 0x08: points just past this header */
} SBTLINDEX_LIST;

typedef struct SBTLINDEX_WORK {
    s32 phase;
    s32 skillId;
    s32 reference;
    SBTL_UNIT *companionA;
    SBTL_UNIT *companionB;
    SBTL_UNIT *linkedUnit;
    s32 summonId; /* +0x18: also reused as a scene-command party-record index. */
    s32 stageValue;
    s32 precedence; /* Command precedence value: 8 for the default, otherwise helper result minus 8 (logged as btl:precede). */
    s32 slot; /* complete action-classification result */
    s32 adjustedValue;
    s8 resultKind;
    u8 costValidationFailed; /* +0x2D: cost check failed; suppress normal command follow-up. */
    u8 allTargetSideActorsSkipped; /* +0x2E: target resolution reports every active actor on selected sides skipped. */
    u8 pad2F;
    u16 stage;
    u8 pad32[2];
    s32 parameter;
    s32 wait;
    s32 unk3C;
    SBTLINDEX_LIST *indices;
    u8 pad44[4];
    u64 ownerId;
    s32 huntEpReward; /* +0x50: accumulated hunt EP awarded after target resolution. */
    s32 battleExperienceReward; /* 0x54: accumulated per-enemy EP sent to the reward packet. */
    s32 moneyReward; /* 0x58: accumulated per-enemy currency sent to the reward packet. */
    u16 flags;
    u8 refreshEligibleActors;
    u8 pad5F;
    SBTLOPERAND_GROUP *groups;
    struct sdfMemHandle_t *allocationHandle;
} SBTLINDEX_WORK;

/* btlCreateActionSeq owns one command actor. */
typedef struct SBTL_TASK {
    s32 state;
    u16 actionNumber;
    u8 pad06[2];
    u32 flags;
    union {
        u32 options;
        s32 actionFlags; /* +0x0C: AI context tests bit 1 as signed state */
    };
    s32 stateTime;
    s32 completedTurns; /* +0x14: incremented when an actor turn is committed */
    SBTL_UNIT *unit;
    u8 unk_1C[4];
    SBTLINDEX_WORK indexWork; /* +0x20..+0x87 */
    u16 aiCounter; /* +0x88: reset, incremented, then clamped by AI queries */
    u8 pad8A[0xBC];
    s8 lowHpActionHold; /* +0x146: positive suppresses the low-HP action */
    u8 pad147;
    u32 actions[8]; /* +0x148: queued action words; consumers also read low halfword IDs */
    struct SBTL_TASK *prev; /* +0x168 */
    struct SBTL_TASK *next; /* +0x16C */
} SBTL_TASK;

typedef char SBTL_TASK_size_must_be_0x170[(sizeof(SBTL_TASK) == 0x170) ? 1 : -1];

/* Battle unit. Retail accessors distinguish world rotation at
 * +0x40 from the orientation quaternion at +0x70; body/muzzle offsets are
 * +0x90/+0xA0 respectively. Effect origin/scene extension are +0x31C/+0x320.
 * The actor chain ends at +0x344. Command state/action words are a different
 * object and must not be interpreted through this unit layout. */
typedef struct SBTL_UNIT {
    f32 lightDirection[4]; /* 0x00: source direction restored by battle light transitions. */
    f32 colorStart[4]; /* 0x10: first source color used by the effect blend callback. */
    f32 colorEnd[4]; /* 0x20: second source color used by the effect blend callback. */
    f32 position[4]; /* 0x30: world position */
    f32 rotation[4]; /* 0x40: world rotation passed to btlSetUnitRotation */
    f32 actorScale; /* 0x50: base actor scale applied to model resources and effect vectors. */
    u32 baseColor; /* 0x54: RGB restored after temporary battle effects */
    union {
        f32 unk58;
        u32 unk58Bits;
    };
    u8 pad5C[4];
    f32 currentPosition[4]; /* 0x60: position retained by the unit setter */
    f32 orientation[4]; /* 0x70: quaternion converted to a VU matrix */
    f32 scale; /* 0x80 */
    u32 overlayColor; /* 0x84: packed transient actor color. */
    union {
        f32 positionZOffset; /* 0x88: added to world Z before rotating a local offset. */
        u32 positionZOffsetBits; /* Integer-zero initialization preserves the native store view. */
    };
    u8 pad8C[4];
    union {
        /* Accessor names describe caller intent: GetMuzzlePosVU reads this
         * body-reach offset; GetBodyPosVU reads muzzleOffset at +0xA0. */
        f32 bodyOffset[4]; /* 0x90 */
        struct {
            /* Some setup paths write zero as integer bits, not float stores. */
            s32 bodyOffsetXBits;
            f32 bodyOffsetY;
            f32 bodyOffsetZ;
            s32 bodyOffsetWBits;
        };
    };
    f32 muzzleOffset[4]; /* 0xA0: read by btlUnitGetBodyPosVU */
    f32 height; /* 0xB0 */
    f32 reach; /* 0xB4 */
    f32 geometryHeight; /* 0xB8: geometry height, independently copied or overridden. */
    f32 bodyRadius; /* 0xBC: unscaled actor half-width used by formation and positioning. */
    f32 cameraRadius; /* 0xC0: scaled actor clearance used by camera framing. */
    s32 resourceKind; /* 0xC4: actor resource table selector. */
    u32 species; /* 0xC8 */
    u8 modelBorrowed; /* Nonzero shares another unit's model state. */
    u8 padCD[3];
    s32 modelId; /* 0xD0: model passed to btlCreateModelChangeTask by the gun-change command */
    s32 modelVariant; /* 0xD4: its variant argument */
    u8 padD8[4];
    s32 modelResourceKind; /* 0xDC: model-asset kind paired with displaySpecies for special-mode loads. */
    u32 displaySpecies; /* 0xE0 */
    u8 padE4[4];
    u32 updateFlags; /* 0xE8: bit 1 forces the actor update's reset path */
    s32 motionId; /* 0xEC: current primary model motion; -1 before selection. */
    s32 effectState; /* 0xF0 */
    f32 motionRate; /* 0xF4: stored rate applied to the owner's primary SDK motion. */
    s16 motionStartFrame; /* 0xF8: retained motion-range start frame. */
    s16 motionEndFrame; /* 0xFA: retained motion-range end frame. */
    s32 effectIndex; /* 0xFC: retained motion-selection index. */
    s32 effectParameter; /* 0x100: retained motion-selection mode. */
    f32 effectScale; /* 0x104: retained motion-selection rate. */
    u64 identity; /* 0x108: copied to effect tasks and compared to command IDs */
    SBTLUNIT_STATUS status; /* 0x110: complete low/high status record. */
    u32 gunResourceFlags; /* 0x118 */
    u8 lookupId; /* 0x11C: party slots 0-2 or enemy slots 0-8; 0xFF when unset. */
    u8 pad11D[3];
    datActorWork_t partyRecord; /* 0x120: complete copied actor record (0x1A4). */
    u8 partyIndex; /* 0x2C4: saved party-entry index, read with lbu. */
    u8 pad2C5;
    SBTLUNITENTRY_SLOT entrySlots[7]; /* 0x2C6 */
    s32 selectedEntryIndex; /* 0x2F0: command-record index; -1 denotes no selected entry. */
    s32 pendingSkillNameIndex; /* 0x2F4: skill-name override index; -1 uses the action skill. */
    struct SoundResourceNode *resourceNode; /* 0x2F8: owned system-effect resource. */
    struct SoundResourceLink *resourceLink; /* 0x2FC: linked model-resource task. */
    struct SoundLink *link; /* 0x300: per-unit command effect. */
    struct ActiveSoundNode *listNode; /* 0x304: independently loaded sound node. */
    struct SoundSlotOwner *soundSlotOwner; /* 0x308: shared category/id motion-SE owner. */
    struct SoundMixer *gunResource; /* 0x30C: retained actor sound mixer. */
    BtlEffectLinkState effectLink; /* 0x310: effect flags and outstanding effect-task references. */
    u8 firstCountdown; /* 0x318: active linked-number displays; selects modulo-12 offset. */
    u8 counterDisplayCount; /* 0x319: active counter displays; selects modulo-12 offset. */
    u8 pad31A[2];
    struct dds3Basic_t *effectObject; /* 0x31C: actor's world node and transform. */
    struct evtUnitHandle_t *ext; /* 0x320: the event manager's complete 0x170-byte work. */
    struct sdfModelRootNode_t *transparencyModel; /* 0x324: alternate SDF model retained during transparency. */
    struct SBTL_UNIT *mirror; /* 0x328: unit drawn from this unit's transparency packet buffer */
    s32 transparencyBuffer; /* 0x32C: mirror pass's 512x224 transfer image. */
    struct sdfMemHandle_t *transparencyBufferAllocation; /* 0x330: allocation handle released with the image. */
    u8 pad334[8];
    struct sdfMemHandle_t *handle; /* 0x33C: allocation that owns this unit. */
    struct SBTL_UNIT *previousActor; /* 0x340 */
    struct SBTL_UNIT *next; /* 0x344: actor-list link, not a task-chain link */
} SBTL_UNIT;

typedef char SBTL_UNITSizeCheck[(sizeof(SBTL_UNIT) == 0x348) ? 1 : -1];

#endif
