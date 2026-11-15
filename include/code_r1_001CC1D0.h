#ifndef CODE_R1_001_CC1_D0_H
#define CODE_R1_001_CC1_D0_H

#include "common.h"
#include "kernel.h"
#include "sdf_resource.h"
#include "code_0027A1D0.h"

/* Draw geometry is copied together; grid easing walks the bound coordinates. */
typedef struct EffectSlotGeometry {
    s32 bounds[4];
    u32 cornerColors[4];
    f32 angleDegrees;
} EffectSlotGeometry;

/* Native 0x80-byte source descriptor; its bounds feed both grid and slot drawing. */
typedef struct EffectSlotDescription {
    u8 pad00[0x14];
    u32 textureIndex;
    s32 flags;
    u8 pad1C[0x10];
    s32 presetMode;
    u8 pad30[4];
    u32 rect[4];
    s32 bounds[4];       /* 0x44: x offset, y offset, width, height in grid units. */
    s32 colors[4];
    u32 cornerColors[4]; /* 0x64: reordered into the live/saved corner colors. */
    s32 widthOverride;   /* 0x74: zero selects the source bounds' width. */
    s32 heightOverride;  /* 0x78: zero selects the source bounds' height. */
    u16 initialDelay;    /* 0x7C: loaded into each timed state's delay before its +1. */
    u16 descriptionUpdateDelay; /* 0x7E: update-call delay loaded into the live work entry. */
} EffectSlotDescription;

typedef char EffectSlotDescriptionSizeCheck[sizeof(EffectSlotDescription) == 0x80 ? 1 : -1];

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

typedef struct CampEffectRows {
    u32 values[2][4];
} CampEffectRows;

typedef struct MenuGradientFade {
    u32 active;
    s32 color;
    s32 blend;
} MenuGradientFade;

typedef char MenuGradientFade_size_must_be_0x0C[(sizeof(MenuGradientFade) == 0x0C) ? 1 : -1];

/* Native 0x10-byte list-node payload. Its first word is an entry index or
 * displayed value according to the list; price is the preserved base price. */
typedef struct CampWindowParams {
    s32 value;
    s32 id;
    s32 price;
    s32 mode;
} CampWindowParams;

typedef char CampWindowParams_size_must_be_0x10[(sizeof(CampWindowParams) == 0x10) ? 1 : -1];

/* The two terminal payload words are a party index and its signed recovery
 * cost. The surrounding node storage
 * is also the camp/sort payload, not a second progress-node struct view. */
typedef struct MenuThresholdEntry {
    s32 entryId;
    s32 requiredAmount;
} MenuThresholdEntry;

/* Each mapped record owns status storage expanded while its resource is loaded. */
typedef struct EffMappedRecord {
    u8 pad00[0x14];
    u32 category;
    u32 statusBytes;
    u8 pad1C[4];
    u8 *status;
} EffMappedRecord;

/* Native 0x14-byte timed state embedded in a slot's kind-specific payload. */
typedef struct EffTimedState {
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
    struct EffMappedRecord *source;
} EffTimedState;

/* Native 0xA0-byte slot work. Material, grid and textured-surface operations
 * use overlapping payload fields in the same slot, not separate allocations. */
typedef struct BdWork {
    s32 flags;           /* 0x00 */
    EffectSlotGeometry geometry; /* 0x04: x, y, width, height, colors, angle */
    EffTimedState states[2]; /* 0x28 */
    /* Per-mode words: corner-color offsets, grid x/y/width/height, or bar crop. */
    s32 parameters[4];   /* 0x50: bar crop width uses word 2 */
    void *owner;         /* 0x60 */
    s32 slotIndex;       /* 0x64 */
    union {
        struct {
            u32 rect[4]; /* 0x68 */
            u8 pad78[4];
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
} BdWork;

/* Native 0x30-byte resource-slot owner: source descriptors and live work arrays. */
typedef struct EffectSlotSet {
    struct sdfMemHandle_t *sourceAllocation;
    u32 sharesTextureReferences;
    u32 count;
    struct sdfMemHandle_t *descriptionAllocation;
    EffectSlotDescription *descriptions;
    struct sdfMemHandle_t *workAllocation;
    BdWork *workEntries;
    u32 textureCount;
    struct sdfMemHandle_t *textureAllocation;
    sdfTexHandle_t **textureReferences;
    s32 defaultValue;
    u8 pad2C[4];
} EffectSlotSet;

typedef char BdWorkSizeCheck[sizeof(BdWork) == 0xA0 ? 1 : -1];

typedef char EffectSlotSetSizeCheck[sizeof(EffectSlotSet) == 0x30 ? 1 : -1];

/* The native window constructor allocates 0x98 bytes; panel layout is 0x38. */
struct MenuIconState {
    u32 kind;
    u32 unk4;
    s32 count;
    struct EffectSlotSet *sprite[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 fade;
};

typedef char MenuIconState_size_must_be_0x38[(sizeof(struct MenuIconState) == 0x38) ? 1 : -1];

/* The backdrop packet's complete resource payload, also used by shop callbacks. */
typedef struct MapPacket {
    u32 type;
    u32 value;
    struct EffectSlotSet *sheets[1]; /* The resource destructor iterates this one-sheet bank. */
    u32 items[11];
    s32 count;
} MapPacket;

typedef char MapPacket_size_must_be_0x3C[(sizeof(MapPacket) == 0x3C) ? 1 : -1];

typedef struct DspScrollingStrip {
    struct EffectSlotSet *resource;
    s32 frameIndex;
    s32 horizontalOffset;
    s32 verticalOffset;
    s32 scrollSpeed;
} DspScrollingStrip;

typedef struct DspScrollingStripState {
    s32 unk0;
    s32 layout;
    s32 unk8;
    struct EffectSlotSet *resource;
    s32 layer;
    s32 overlayX;
    s32 overlayY;
    DspScrollingStrip strips[6];
} DspScrollingStripState;

typedef char DspScrollingStrip_size_must_be_0x14[(sizeof(DspScrollingStrip) == 0x14) ? 1 : -1];

typedef char DspScrollingStripState_size_must_be_0x94[(sizeof(DspScrollingStripState) == 0x94) ? 1 : -1];

typedef struct MenuIconSprites {
    struct sdfMemHandle_t *allocation; /* 0x00: retained descriptor for this bundle. */
    u32 value;
    u32 unk8;
    struct EffectSlotSet *sprite[3];
} MenuIconSprites;

typedef char MenuIconSprites_size_must_be_0x18[(sizeof(MenuIconSprites) == 0x18) ? 1 : -1];

/* Native list owner, shared by both games' allocator and window consumers. */
struct MenuListNode {
    s32 index;
    const u8 *text; /* 0x04: borrowed encoded display text; removing a node frees only the node. */
    struct {
        struct EffectSlotSet *sprite;
        u32 effect;
    } sprites[8];       /* 0x08: four normal and four selected-row sprites */
    u32 flags48;        /* 0x48 */
    u8 pad4C[4];
    s32 animationTimer; /* 0x50: stepped down to zero while a list is visible */
    u8 selectionByte54; /* 0x54: cleared on moving the list selection */
    u8 pad55[3];
    struct MenuListNode *next;
    struct MenuListNode *prev;
    /* The allocator reserves 0x74 bytes. Camp builders fill four payload
     * words at 0x60; other lists use the same storage as sorting keys. */
    union {
        struct {
            u32 sortKeyPrimary;   /* 0x60 */
            u32 sortKeySecondary; /* 0x64 */
            u32 sortKeyTertiary;  /* 0x68 */
            u8 pad6C[4];
        };
        CampWindowParams camp;
        MenuThresholdEntry terminal;
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
    (sizeof(((struct MenuListNode *)0)->sprites[0].sprite) == 4) ? 1 : -1];

typedef char MenuListNode_size_must_be_0x74[(sizeof(struct MenuListNode) == 0x74) ? 1 : -1];

/* Timed states borrow records through EffTimedState.source. Keep this
 * resource alive until those states no longer reference its records.
 */
typedef struct EffMappedResource {
    s32 count;
    struct sdfMemHandle_t *allocation;
    EffMappedRecord *records;
} EffMappedResource;

typedef struct MenuEffectResources {
    MapPacket packet;
    struct EffMappedResource *animationHandle;
    CampEffectRows rows;
} MenuEffectResources;

/* Result/camp backdrop: resources, sixteen sparks and the badge fade. */
typedef struct MenuCampEffect {
    MenuEffectResources resources;
    s32 direction[16];
    s32 velocity[16][2];
    s32 life[16];
    s32 count;
    s32 fade;
} MenuCampEffect;

typedef char MenuEffectResources_size_must_be_0x60[(sizeof(MenuEffectResources) == 0x60) ? 1 : -1];

typedef char MenuCampEffect_size_must_be_0x168[(sizeof(MenuCampEffect) == 0x168) ? 1 : -1];

typedef char EffMappedRecord_size_must_be_0x24[(sizeof(EffMappedRecord) == 0x24) ? 1 : -1];

typedef char EffMappedResource_size_must_be_0x0C[(sizeof(EffMappedResource) == 0x0C) ? 1 : -1];

void jmsSeqJumpAndFree(s32 *entrySlot, jmsSeqUnit_t *entry);

/* An indexed render slot owned by an effect resource set. */
typedef struct MenuGridSlot {
    struct EffectSlotSet *set;
    s32 index;
} MenuGridSlot;

typedef char MenuGridSlot_size_must_be_8[(sizeof(MenuGridSlot) == 8) ? 1 : -1];

struct MenuList {
    u32 stateFlags;     /* 0x00: cursor and selection-control bits */
    u32 flags;
    s32 id;             /* 0x08: owner/list identifier */
    s32 visibleCount;
    struct MenuListNode *first;
    struct MenuListNode *last;
    struct MenuListNode *head;
    struct MenuListNode *cursor;
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

typedef char MenuList_size_must_be_0x40[(sizeof(struct MenuList) == 0x40) ? 1 : -1];

typedef u32 MenuPanelSelectionIndex;

typedef struct MenuPanelStatePrefix {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    MenuPanelSelectionIndex selectedIndex;
    u32 firstValueA;
    u32 firstValueB;
} MenuPanelStatePrefix;

typedef char MenuPanelStatePrefixSizeCheck[(sizeof(MenuPanelStatePrefix) == 0x20) ? 1 : -1];

/* Remove and free the cursor node, returning the updated cursor or NULL. */
struct MenuListNode *mnuRemoveListCursorNode(struct MenuList *list);

/* Append a node borrowing display text that must outlive its use by the list. */
struct MenuListNode *mnuListAppendNode(struct MenuList *list, const void *text);

/* Move the visible list window and return its unchanged cursor node. */
struct MenuListNode *mnuAdvanceListWindowStart(struct MenuList *list);

/* Anchored insertion defaults before; empty/null/last anchors append regardless of this option. */
/* Insert a node borrowing display text; the list owns only its node. */
struct MenuListNode *mnuInsertListNodeRelativeToAnchor(struct MenuList *list,
                                                       struct MenuListNode *anchor,
                                                       const void *text, s32 mode,
                                                       u32 options);

struct MenuListNode *mnuAdvanceListCursorDefault(struct MenuList *list);

/* The item-list word is an address-or-sentinel result, not always a pointer. */
typedef u32 ItfMesEntryItemsResult;

/* Message tables contain relocated encoded-text addresses. */
typedef struct ItfMesTable {
    u8 unk0[0x18];
    s16 count;
    s16 bitCount;
    u32 items[1];
} ItfMesTable;

typedef struct ItfMesEntry {
    /* Relocated table address, or NONE / BRANCH sentinel. */
    ItfMesEntryItemsResult itemList;
    ItfMesTable *table;
} ItfMesEntry;

typedef char ItfMesEntry_size_must_be_0x8[
    sizeof(ItfMesEntry) == 0x8 ? 1 : -1];

typedef struct ItfMesSub {
    u8 unk0[8];
    u32 magic;
    u8 unkC[4];
    s32 fixupTableOffset;
    s32 fixupTableBytes;
    u32 entryCount;
    u8 relocated;
    u8 unk1D[3];
    ItfMesEntry entries[1];
} ItfMesSub;

/* Native resource handle and the relocatable message data it owns. */
typedef struct EvtResourcePair {
    struct sdfMemHandle_t *handle;
    struct ItfMesSub *messageData;
} EvtResourcePair;

typedef char EvtResourcePair_size_must_be_0x08[(sizeof(EvtResourcePair) == 0x08) ? 1 : -1];

typedef struct MenuWindowContainer {
    s32 id;                /* 0x00 */
    u32 flags;             /* 0x04 */
    s32 originX;           /* 0x08 */
    s32 originY;           /* 0x0C */
    s32 width;             /* 0x10 */
    s32 height;            /* 0x14 */
    struct MenuList *list; /* 0x18 */
    s32 suppressBaseSprite; /* 0x1C */
    struct EffectSlotSet *spriteResource; /* 0x20 */
    s32 rowSpriteSlot;     /* 0x24 */
    s32 baseSpriteSlot;    /* 0x28 */
    struct {
        struct EffectSlotSet *sprite;
        u32 parameter;
    } decorations[3];      /* 0x2C: optional window decoration sprites */
    u32 decorationX[3];    /* 0x44 */
    s32 scale50;           /* 0x50 */
    s32 scale54;           /* 0x54 */
    struct MenuIconState panel; /* 0x58: embedded drawable panel layout */
    struct MenuIconSprites *resource; /* 0x90: owned sprite-resource bundle */
    u32 unk94;             /* 0x94: cleared by jmsCreateCmdMenu */
    u32 unk98;             /* 0x98: cleared by jmsCreateCmdMenu */
    u32 fadeScale;         /* 0x9C: read as the opacity by the window draw functions */
} MenuWindowContainer;

typedef struct MenuWindowContainer MenuWindowContainer;

typedef char MenuWindowContainer_size_must_be_0xA0[(sizeof(MenuWindowContainer) == 0xA0) ? 1 : -1];

/* Terminal/shop modes share this complete 0x38C-byte scene allocation. */
typedef struct MenuTerminalContext {
    struct sdfMemHandle_t *resourceHandle;
    u8 pad04[4];
    s32 type;
    jmsSeqHandle_t transitionWork;
    s32 popupState;
    jmsSeqUnit_t *popupTable;
    EvtResourcePair messageResources;
    struct EffectSlotSet *effectSlots[4];
    s32 state;
    MenuWindowContainer *ownedWindows[1];
    MenuWindowContainer *window;
    struct EffMappedResource *objects[2];
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
    DspScrollingStripState panelWork[2];
    MenuCampEffect campEffect; /* 0x210: resources, sixteen sparks and badge fade */
    struct MenuIconSprites *windowResource;
    MenuGradientFade gradientFade;
    s8 rewardGranted;
    s8 sceneReady;
    u8 pad38A[2];
} MenuTerminalContext;

typedef char MenuTerminalContext_size_must_be_0x38C[(sizeof(MenuTerminalContext) == 0x38C) ? 1 : -1];

#endif
