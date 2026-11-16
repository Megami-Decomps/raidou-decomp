#ifndef CODE_R1_001_CC1_D0_H
#define CODE_R1_001_CC1_D0_H

#include "common.h"
#include "kernel.h"
#include "sdf_resource.h"
#include "code_0027A1D0.h"

/* Draw geometry is copied together; grid easing walks the bound coordinates. */
typedef struct effSlotGeometry_t {
    s32 bounds[4];
    u32 cornerColors[4];
    f32 angleDegrees;
} effSlotGeometry_t;

/* Texture coordinates of a sprite quad; the draw code copies and edits them by index. */
typedef union effSpriteUV_t {
    struct {
        s32 u0;
        s32 v0;
        s32 u1;
        s32 v1;
    } rect;
    s32 coord[4];
} effSpriteUV_t;

/* Native 0x80-byte source descriptor; its bounds feed both grid and slot drawing. */
typedef struct effSlotDescription_t {
    u8 pad00[0x14];
    u32 textureIndex;
    s32 flags;
    u8 pad1C[0x10];
    s32 presetMode;
    u8 pad30[4];
    u32 rect[4];
    s32 bounds[4];       /* 0x44: x offset, y offset, width, height in grid units. */
    effSpriteUV_t uv;      /* 0x54 */
    u32 cornerColors[4]; /* 0x64: reordered into the live/saved corner colors. */
    s16 widthOverride;   /* 0x74: zero selects the uv width. */
    s16 heightOverride;  /* 0x76: zero selects the uv height. */
    u8 repeatCount[2];   /* 0x78: tiles across and down, counting the first. */
    u8 repeatStep[2];    /* 0x7A: tile step across (x16) and down (x8). */
    u16 initialDelay;    /* 0x7C: loaded into each timed state's delay before its +1. */
    u16 descriptionUpdateDelay; /* 0x7E: update-call delay loaded into the live work entry. */
} effSlotDescription_t;

typedef char effSlotDescriptionSizeCheck[sizeof(effSlotDescription_t) == 0x80 ? 1 : -1];

typedef s32 (*jmsInt32Delegate)(dds3ProcessID_t *ID);

/* One step of a sequence: Init/Destroy run on push and pop, jmsSeqChange returns
 * Calc, Draw or Update by mode, and Check accepts the change. */
typedef struct jmsSeqUnit_t {
    u32 Stat;
    jmsInt32Delegate pInit;
    jmsInt32Delegate pDestroy;
    dds3ProcessFunc_t pCalc;
    dds3ProcessFunc_t pDraw;
    dds3ProcessFunc_t pUpdate;
    jmsInt32Delegate pCheck;
#ifdef VERSION_RAIDOU2
    char *Name;
#endif
} jmsSeqUnit_t;

#ifdef VERSION_RAIDOU2
typedef char jmsSeqUnit_t_size_must_be_0x20[(sizeof(jmsSeqUnit_t) == 0x20) ? 1 : -1];
#else
typedef char jmsSeqUnit_t_size_must_be_0x1C[(sizeof(jmsSeqUnit_t) == 0x1C) ? 1 : -1];
#endif

/* Both games keep sixteen saved entries and the current/previous entry pointers. */
typedef struct jmsSeqHandle_t {
    s32 count;
    jmsSeqUnit_t *entries[16];
    jmsSeqUnit_t *entryAddress;
    jmsSeqUnit_t *lastEntryAddress;
} jmsSeqHandle_t;

typedef char jmsSeqHandle_t_size_must_be_0x4C[
    (sizeof(jmsSeqHandle_t) == 0x4C) ? 1 : -1];

/* Advance one popup transition state using its current scheduler word. */
s32 jmsSeqChange(jmsSeqHandle_t *work, s32 *entrySlot, s32 mode, dds3ProcessID_t *ID);

void jmsSeqJump(s32 *entrySlot, jmsSeqUnit_t *entry);

/* Leading fields of a menu process work driven by jmsSeqChange; the rest is per menu. */
typedef struct jmsSeqWork_t {
    struct sdfMemHandle_t *hMem;
    u32 flag;
    jmsSeqHandle_t panel;
    s32 seqUnit;
} jmsSeqWork_t;

/* Retail calls jmsSeqChange through an inline step; a direct call schedules its arguments differently. */
static inline dds3ProcessFunc_t menuRunPanel(jmsSeqHandle_t *panel, s32 *seqUnit, s32 mode, dds3ProcessID_t *PID) {
    return (dds3ProcessFunc_t)jmsSeqChange(panel, seqUnit, mode, PID);
}

typedef struct campEffectRows_t {
    u32 values[2][4];
} campEffectRows_t;

typedef struct mnuGradientFade_t {
    u32 active;
    s32 color;
    s32 blend;
} mnuGradientFade_t;

typedef char mnuGradientFade_size_must_be_0x0C[(sizeof(mnuGradientFade_t) == 0x0C) ? 1 : -1];

/* Native 0x10-byte list-node payload. Its first word is an entry index or
 * displayed value according to the list; price is the preserved base price. */
typedef struct campWindowParams_t {
    s32 value;
    s32 id;
    s32 price;
    s32 mode;
} campWindowParams_t;

typedef char campWindowParams_size_must_be_0x10[(sizeof(campWindowParams_t) == 0x10) ? 1 : -1];

/* The two terminal payload words are a party index and its signed recovery
 * cost. The surrounding node storage
 * is also the camp/sort payload, not a second progress-node struct view. */
typedef struct mnuThresholdEntry_t {
    s32 entryId;
    s32 requiredAmount;
} mnuThresholdEntry_t;

/* Each mapped record owns status storage expanded while its resource is loaded. */
typedef struct effMappedRecord_t {
    u8 pad00[0x14];
    u32 category;
    u32 statusBytes;
    u8 pad1C[4];
    u8 *status;
} effMappedRecord_t;

/* Native 0x14-byte timed state embedded in a slot's kind-specific payload. */
typedef struct effTimedState_t {
    u32 flags;
    s32 value; /* Signed 16.16 transition phase, clamped to 0..1.0 */
    union {
        s32 delay;
        u32 materialFlags;
    };
    union {
        s32 delayMax;
        u32 materialValue;
    };
    struct effMappedRecord_t *source;
} effTimedState_t;

/* Native 0xA0-byte slot work. Material, grid and textured-surface operations
 * use overlapping payload fields in the same slot, not separate allocations. */
typedef struct itfBdWork_t {
    s32 flags;           /* 0x00 */
    effSlotGeometry_t geometry; /* 0x04: x, y, width, height, colors, angle */
    effTimedState_t states[2]; /* 0x28 */
    /* Per-mode words: corner-color offsets, grid x/y/width/height, or bar crop. */
    s32 parameters[4];   /* 0x50: bar crop width uses word 2 */
    void *owner;         /* 0x60 */
    s32 slotIndex;       /* 0x64 */
    union {
        struct {
            u8 edgeAlpha[4]; /* 0x68: per-edge alpha, 0x80 keeps the corner colors */
            u8 pad6C[0x10];
        } texture;
        struct {
            u8 pad68[4];
            s32 quantizedBounds[4]; /* 0x6C */
        } grid;
    } bounds;
    s32 sourceWidth;     /* 0x7C */
    s32 sourceHeight;    /* 0x80 */
    u32 savedColors[4];  /* 0x84 */
    s32 slotOffset;      /* 0x94 */
    s32 remainingDescriptionUpdates; /* 0x98: update-call countdown before advancing a timed description. */
    union {
        s32 address;
        u32 bits;
        void *payload; /* Alternate payloads may be only 0x6C bytes. */
    } alternate;        /* 0x9C */
} itfBdWork_t;

/* Native 0x30-byte resource-slot owner: source descriptors and live work arrays. */
typedef struct sprHandle_t {
    struct sdfMemHandle_t *sourceAllocation;
    u32 sharesTextureReferences;
    u32 SpriteNums;
    struct sdfMemHandle_t *descriptionAllocation;
    effSlotDescription_t *descriptions;
    struct sdfMemHandle_t *workAllocation;
    itfBdWork_t *workEntries;
    u32 TexNums;
    struct sdfMemHandle_t *textureAllocation;
    sdfTexHandle_t **textureReferences;
    s32 defaultValue;
    u8 pad2C[4];
} sprHandle_t;

typedef char itfBdWorkSizeCheck[sizeof(itfBdWork_t) == 0xA0 ? 1 : -1];

typedef char sprHandleSizeCheck[sizeof(sprHandle_t) == 0x30 ? 1 : -1];

void sdfDrawTexturedQuad(s32 x, s32 y, u32 z, s32 width, s32 height, const effSpriteUV_t *uv, const u32 *colors,
                         u32 flip, f32 angle, u32 alphaMode, s32 filter, sdfTexHandle_t *tex, s32 ot);
void effSelectPresetAndDispatch(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *vertexColors,
                                u32 presetMode, u32 presetValue);
void sdfCopyCornerWordsWithEdgeByte(u32 *source, u32 *destination, u8 *edgeValues, u32 edge);
void cmpFunc3(s32 x, s32 y, u32 z, s32 width, s32 height, u32 *edges, u8 *edgeValues, effSpriteUV_t *uv,
              u32 *colors, u32 flip, u32 alphaMode, s32 filter, sdfTexHandle_t *tex, s32 ot);
void cmpFunc4(s32 x, s32 y, u32 z, s32 width, s32 height, u8 *counts, u8 *steps, effSpriteUV_t *uv, u32 *colors,
              u32 flip, u32 alphaMode, s32 filter, sdfTexHandle_t *tex, s32 ot);
void effDrawTextureSlot(s32 x, s32 y, u32 z, u32 *colors, u32 flags, sprHandle_t *set, s32 slot, itfBdWork_t *draw, s32 ot);
sprHandle_t *effUpdateTimedStates(sprHandle_t *effect, u32 slot, void *entryData);

/* The native window constructor allocates 0x98 bytes; panel layout is 0x38. */
struct mnuIconState_t {
    u32 kind;
    u32 unk4;
    s32 count;
    struct sprHandle_t *sprite[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 fade;
};

typedef char mnuIconState_size_must_be_0x38[(sizeof(struct mnuIconState_t) == 0x38) ? 1 : -1];

/* The backdrop packet's complete resource payload, also used by shop callbacks. */
typedef struct mapPacket_t {
    u32 type;
    u32 value;
    struct sprHandle_t *sheets[1]; /* The resource destructor iterates this one-sheet bank. */
    u32 items[11];
    s32 count;
} mapPacket_t;

typedef char mapPacket_size_must_be_0x3C[(sizeof(mapPacket_t) == 0x3C) ? 1 : -1];

typedef struct mnuDspScrollingStrip_t {
    struct sprHandle_t *resource;
    s32 frameIndex;
    s32 horizontalOffset;
    s32 verticalOffset;
    s32 scrollSpeed;
} mnuDspScrollingStrip_t;

typedef struct mnuDspScrollingStripState_t {
    s32 unk0;
    s32 layout;
    s32 unk8;
    struct sprHandle_t *resource;
    s32 layer;
    s32 overlayX;
    s32 overlayY;
    mnuDspScrollingStrip_t strips[6];
} mnuDspScrollingStripState_t;

typedef char mnuDspScrollingStrip_size_must_be_0x14[(sizeof(mnuDspScrollingStrip_t) == 0x14) ? 1 : -1];

typedef char mnuDspScrollingStripState_size_must_be_0x94[(sizeof(mnuDspScrollingStripState_t) == 0x94) ? 1 : -1];

typedef struct mnuIconSprites_t {
    struct sdfMemHandle_t *allocation; /* 0x00: retained descriptor for this bundle. */
    u32 value;
    u32 unk8;
    struct sprHandle_t *sprite[3];
} mnuIconSprites_t;

typedef char mnuIconSprites_size_must_be_0x18[(sizeof(mnuIconSprites_t) == 0x18) ? 1 : -1];

/* Native list owner, shared by both games' allocator and window consumers. */
struct mnuListNode_t {
    s32 index;
    const u8 *text; /* 0x04: borrowed encoded display text; removing a node frees only the node. */
    struct {
        struct sprHandle_t *sprite;
        u32 effect;
    } sprites[8];       /* 0x08: four normal and four selected-row sprites */
    u32 flags48;        /* 0x48 */
    u8 pad4C[4];
    s32 animationTimer; /* 0x50: stepped down to zero while a list is visible */
    u8 selectionByte54; /* 0x54: cleared on moving the list selection */
    u8 pad55[3];
    struct mnuListNode_t *next;
    struct mnuListNode_t *prev;
    /* The allocator reserves 0x74 bytes. Camp builders fill four payload
     * words at 0x60; other lists use the same storage as sorting keys. */
    union {
        struct {
            u32 sortKeyPrimary;   /* 0x60 */
            u32 sortKeySecondary; /* 0x64 */
            u32 sortKeyTertiary;  /* 0x68 */
            u8 pad6C[4];
        };
        campWindowParams_t camp;
        mnuThresholdEntry_t terminal;
    };
    /* +0x70: terminal recovery panel at 267050; title at 2674C8.
     * Mantra lists retain a datActorWork_t address in the same word. */
    union {
        u32 userValue;
        void *childPanel;
        char *title;
        struct datActorWork_t *partyRecord;
        /* Separate 0x10-byte payload. */
        struct SdfCounterDisplay *counterDisplay;
    };
};

typedef char MenuListNode_sprite_word_must_be_4[
    (sizeof(((struct mnuListNode_t *)0)->sprites[0].sprite) == 4) ? 1 : -1];

typedef char mnuListNode_size_must_be_0x74[(sizeof(struct mnuListNode_t) == 0x74) ? 1 : -1];

/* Timed states borrow records through effTimedState_t.source. Keep this
 * resource alive until those states no longer reference its records.
 */
typedef struct effMappedResource_t {
    s32 count;
    struct sdfMemHandle_t *allocation;
    effMappedRecord_t *records;
} effMappedResource_t;

typedef struct mnuEffectResources_t {
    mapPacket_t packet;
    struct effMappedResource_t *animationHandle;
    campEffectRows_t rows;
} mnuEffectResources_t;

/* Result/camp backdrop: resources, sixteen sparks and the badge fade. */
typedef struct mnuCampEffect_t {
    mnuEffectResources_t resources;
    s32 direction[16];
    s32 velocity[16][2];
    s32 life[16];
    s32 count;
    s32 fade;
} mnuCampEffect_t;

typedef char mnuEffectResources_size_must_be_0x60[(sizeof(mnuEffectResources_t) == 0x60) ? 1 : -1];

typedef char mnuCampEffect_size_must_be_0x168[(sizeof(mnuCampEffect_t) == 0x168) ? 1 : -1];

typedef char effMappedRecord_size_must_be_0x24[(sizeof(effMappedRecord_t) == 0x24) ? 1 : -1];

typedef char effMappedResource_size_must_be_0x0C[(sizeof(effMappedResource_t) == 0x0C) ? 1 : -1];

void jmsSeqJumpAndFree(s32 *entrySlot, jmsSeqUnit_t *entry);

/* An indexed render slot owned by an effect resource set. */
typedef struct mnuGridSlot_t {
    struct sprHandle_t *set;
    s32 index;
} mnuGridSlot_t;

typedef char mnuGridSlot_size_must_be_8[(sizeof(mnuGridSlot_t) == 8) ? 1 : -1];

struct mnuList_t {
    u32 stateFlags;     /* 0x00: cursor and selection-control bits */
    u32 flags;
    s32 id;             /* 0x08: owner/list identifier */
    s32 visibleCount;
    struct mnuListNode_t *first;
    struct mnuListNode_t *last;
    struct mnuListNode_t *head;
    struct mnuListNode_t *cursor;
    s32 count;
    s32 windowOffset;
    s32 rowStep;        /* 0x28: constructor argument scaled by eight */
    void (*drawCallback)(); /* 0x2C: native callers supply list-specific arguments. */
    /* 0x30 is generic list user data: a countdown pointer, a buffer or
     * staff draw context. */
    void *context;
    s32 categoryIndex; /* 0x34: selects one of the skill-window category layouts. */
    u32 categoryMarkerEnabled; /* 0x38: draw the active category marker. */
    s32 scale;          /* 0x3C: 8.8 fixed-point default */
};

typedef char mnuList_size_must_be_0x40[(sizeof(struct mnuList_t) == 0x40) ? 1 : -1];

typedef u32 MenuPanelSelectionIndex;

typedef struct mnuPanelStatePrefix_t {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    MenuPanelSelectionIndex selectedIndex;
    u32 firstValueA;
    u32 firstValueB;
} mnuPanelStatePrefix_t;

typedef char mnuPanelStatePrefixSizeCheck[(sizeof(mnuPanelStatePrefix_t) == 0x20) ? 1 : -1];

/* Remove and free the cursor node, returning the updated cursor or NULL. */
struct mnuListNode_t *mnuRemoveListCursorNode(struct mnuList_t *list);

/* Append a node borrowing display text that must outlive its use by the list. */
struct mnuListNode_t *mnuListAppendNode(struct mnuList_t *list, const void *text);

/* Move the visible list window and return its unchanged cursor node. */
struct mnuListNode_t *mnuAdvanceListWindowStart(struct mnuList_t *list);

/* Anchored insertion defaults before; empty/null/last anchors append regardless of this option. */
/* Insert a node borrowing display text; the list owns only its node. */
struct mnuListNode_t *mnuInsertListNodeRelativeToAnchor(struct mnuList_t *list,
                                                       struct mnuListNode_t *anchor,
                                                       const void *text, s32 mode,
                                                       u32 options);

struct mnuListNode_t *mnuAdvanceListCursorDefault(struct mnuList_t *list);

/* The item-list word is an address-or-sentinel result, not always a pointer. */
typedef u32 ItfMesEntryItemsResult;

/* Message tables contain relocated encoded-text addresses. */
typedef struct itfMesTable_t {
    u8 unk0[0x18];
    s16 count;
    s16 bitCount;
    u32 items[1];
} itfMesTable_t;

typedef struct itfMesEntry_t {
    /* Relocated table address, or NONE / BRANCH sentinel. */
    ItfMesEntryItemsResult itemList;
    itfMesTable_t *table;
} itfMesEntry_t;

typedef char itfMesEntry_size_must_be_0x8[
    sizeof(itfMesEntry_t) == 0x8 ? 1 : -1];

/* Native resource handle and the relocatable message data it owns. */
typedef struct evtResourcePair_t {
    struct sdfMemHandle_t *handle;
    struct itfMesResource_t *messageData;
} evtResourcePair_t;

typedef char evtResourcePair_size_must_be_0x08[(sizeof(evtResourcePair_t) == 0x08) ? 1 : -1];

typedef struct mnuWindowContainer_t {
    s32 id;                /* 0x00 */
    u32 flags;             /* 0x04 */
    s32 originX;           /* 0x08 */
    s32 originY;           /* 0x0C */
    s32 width;             /* 0x10 */
    s32 height;            /* 0x14 */
    struct mnuList_t *list; /* 0x18 */
    s32 suppressBaseSprite; /* 0x1C */
    struct sprHandle_t *spriteResource; /* 0x20 */
    s32 rowSpriteSlot;     /* 0x24 */
    s32 baseSpriteSlot;    /* 0x28 */
    struct {
        struct sprHandle_t *sprite;
        u32 parameter;
    } decorations[3];      /* 0x2C: optional window decoration sprites */
    u32 decorationX[3];    /* 0x44 */
    s32 scale50;           /* 0x50 */
    s32 scale54;           /* 0x54 */
    struct mnuIconState_t panel; /* 0x58: embedded drawable panel layout */
    struct mnuIconSprites_t *resource; /* 0x90: owned sprite-resource bundle */
    u32 unk94;             /* 0x94: cleared by jmsCreateCmdMenu */
    u32 unk98;             /* 0x98: cleared by jmsCreateCmdMenu */
    u32 fadeScale;         /* 0x9C: read as the opacity by the window draw functions */
} mnuWindowContainer_t;

typedef struct mnuWindowContainer_t mnuWindowContainer_t;

typedef char mnuWindowContainer_size_must_be_0xA0[(sizeof(mnuWindowContainer_t) == 0xA0) ? 1 : -1];

/* Terminal/shop modes share this complete 0x38C-byte scene allocation. */
typedef struct mnuTerminalContext_t {
    struct sdfMemHandle_t *resourceHandle;
    u8 pad04[4];
    s32 type;
    jmsSeqHandle_t transitionWork;
    s32 popupState;
    jmsSeqUnit_t *popupTable;
    evtResourcePair_t messageResources;
    struct sprHandle_t *effectSlots[4];
    s32 state;
    mnuWindowContainer_t *ownedWindows[1];
    mnuWindowContainer_t *window;
    struct effMappedResource_t *objects[2];
    s32 shopRow;
    s32 multiplier;
    s32 dispatchMode;
    u8 pad98[4];
    s32 availableCount;
    s16 hasBulletStock;
    s16 hasHighRangeStock;
    s16 extraEntryEnabled;
    u16 selectedSlot;
    u32 rewardCursor;
    u32 options;
    s32 previousValue;
    s32 elapsedFrames;
    s32 retryFrames;
    s32 commandFrames; /* 0xBC */
    s32 phase;
    s16 stateStep;
    s8 pulseFrame;
    s8 quantityAtLimit;
    s32 previousProgressTotal;
    s8 advancedSlots;
    s8 transactionConfirmed;
    s8 rewardMode;
    u8 padCF;
    s8 rewardRow;
    s8 remainingRewards;
    s8 rewardIndex;
    s8 announceNextReward;
    s8 grantPendingReward;
    s8 rewardDelay;
    u8 padD6[2];
    s32 rewardKind;
    s32 rewardValue;
    s32 prepared;
    s32 delayFrames;
    mnuDspScrollingStripState_t panelWork[2];
    mnuCampEffect_t campEffect; /* 0x210: resources, sixteen sparks and badge fade */
    struct mnuIconSprites_t *windowResource;
    mnuGradientFade_t gradientFade;
    s8 rewardGranted;
    s8 sceneReady;
    u8 pad38A[2];
} mnuTerminalContext_t;

typedef char mnuTerminalContext_size_must_be_0x38C[(sizeof(mnuTerminalContext_t) == 0x38C) ? 1 : -1];

s32 mnuSolveCubicBezierYForX(s32 target, const s32 *x, const s32 *y);

#endif
