#ifndef EVENT_H
#define EVENT_H

#include "basic.h"
#include "model.h"
#include "motion.h"

void evtStartEvent(s32 no);

/* A timeline key's scalar formats are selected by its track kind. */
typedef union EvtViewParam {
    f32 f; s32 i; u32 u; u16 h[2]; s16 sh[2]; u8 b[4]; s8 sb[4];
} EvtViewParam;

/* Raidou's key is 0x38 bytes: SMT3's layout with a 0x20-byte parameter area. */
typedef struct evtFrameInfo_t {
    u16 frame;
    u16 index; /* Also the consecutive index in type-0xB groups. */
    s32 nameIndex;
    union {
        union { s8 sb[0x20]; u16 sh[0x10]; s32 sw[8]; f32 sf[8]; } param;
        union {
            struct {
                u8 unk08[2]; u16 typeIndex;
                u8 unk0C[6]; u16 groupTypeAIndex;
            } f;
            s32 words[8];
        } body;
        struct {
            EvtViewParam p08, p0C, p10, p14, p18, p1C, p20, p24;
        };
    };
    u8 pad28[4];
    void *payload; /* Type 0x12 owns a dds3Basic_t; other kinds hold buffers or borrowed data. */
    struct evtFrameInfo_t *next;
    struct evtFrameInfo_t *prev;
} evtFrameInfo_t;
SIZE_CHECK(evtFrameInfoSizeCheck, evtFrameInfo_t, 0x38);

typedef struct evtViewerObject_s {
    s32 type;
    s32 entryHeader;
    s32 fileIndex;
    dds3Basic_t *basic;
    union {
        u8 unk10[8];
        struct { s16 major, minor; s32 reserve; } resource;
        struct { void *data; s32 reserve; } effect;
    };
    s32 childCount;
    evtFrameInfo_t *children;
    evtFrameInfo_t *lastChild;
    struct evtViewerObject_s *next;
    struct evtViewerObject_s *prev;
} evtViewerObject_s;

typedef struct {
    u16 mode;
    u16 unk02;
    u16 unk04;
    u16 unk06;
} evtViewerMenuEntry_t;

typedef struct evtViewerInfo_t evtViewerInfo_t;

typedef struct pmdUnitTable_t {
    s32 NameIndex, FileIndex, MajorNum, MinorNum, DataOffset, DataSize, Reserve1, Reserve2;
} pmdUnitTable_t;

typedef struct pmdStageTable_t {
    s32 NameIndex, FileIndex, MajorNum, MinorNum;
} pmdStageTable_t;

typedef struct pmdCutInfo_t {
    s32 FirstFrame;
    s32 LastFrame;
    s32 TotalFrame;
    s32 reserve;
} pmdCutInfo_t;

typedef struct pmdEntry_t {
    u32 type;
    u32 elementSize;
    u32 value;
    u32 offset;
} pmdEntry_t;

typedef struct pmdEffectTable_t {
    s32 NameIndex;
    s32 DataOffset;
    s32 Type;
    s32 reserve;
} pmdEffectTable_t;

typedef struct pmdHeader_t {
    s8 FileType;
    s8 FileFormat;
    s16 UserID;
    s32 FileSize;
    char MagicCode[4];
    s32 ExpandSize;
    s32 count;
    s32 kind;
    s32 Reserve2;
    s32 Reserve3;
    pmdEntry_t entries[1];
} pmdHeader_t;

typedef struct pmdFrameTable_t {
    u16 ObjectType;
    u16 Frame;
    u16 Length;
    s16 NameIndex;
    union {
        s8 b[8];
        u8 ub[8];
        s16 h[4];
        u16 uh[4];
        s32 w[2];
        f32 f[2];
    } data;
} pmdFrameTable_t;

/* Interpolated event vector and packed colour. */
typedef struct evtBlendVecColor_t {
    f32 vec[4];
    u32 color;
} evtBlendVecColor_t;

/* Fog record in a polygon movie's scene table. */
typedef struct pmdSceneFogTable_t {
    evtBlendVecColor_t fog;
    s32 reserve[3];
} pmdSceneFogTable_t;

/* Scene-light record: three lights' colours and directions, then the ambient. */
typedef struct {
    f32 color[3][4];
    f32 direction[3][4];
    f32 ambient[4];
} pmdSceneLightTable_t;

/* Event-unit allocation: evtCreateUnitHandle allocates and clears sizeof(evtUnit_t), 0x230 bytes in Raidou.
 * SMT3 omits the motion, shadow and vertical-movement members. */
typedef enum evtUnitMotionState_t {
    EVT_UNIT_MOTION_STATE_IDLE = 0,
    EVT_UNIT_MOTION_STATE_SOURCE = 1,
    EVT_UNIT_MOTION_STATE_MOTION = 2,
    EVT_UNIT_MOTION_STATE_VECTOR = 3,
    EVT_UNIT_MOTION_STATE_VALUE = 4
} evtUnitMotionState_t;

typedef enum evtUnitTransitionSourceKind_t {
    EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR = 0,
    EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE = 1,
    EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE = 2
} evtUnitTransitionSourceKind_t;

enum evtUnitFlags_t {
    EVT_UNIT_FLAG_PATH_REVERSE = 0x4,
    EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR = 0x100,
    EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR = 0x200,
    EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS = 0x300,
    EVT_UNIT_FLAG_RGB_TRANSITION = 0x8000,
    EVT_UNIT_FLAG_ALPHA_TRANSITION = 0x10000,
    EVT_UNIT_FLAG_VALUE_CHANGED = 0x20000,
    EVT_UNIT_FLAG_TARGET_TRANSITION = 0x40000,
    EVT_UNIT_FLAG_TARGET_BLEND_IN = 0x80000,
    EVT_UNIT_FLAG_TARGET_BLEND_OUT = 0x100000,
    EVT_UNIT_FLAG_TARGET_BLEND_PHASES = 0x180000
};

typedef struct {
    s16 motionId;
    s16 startFrame;
    s16 endFrame;
} evtUnitMotionSetup_t;

typedef struct evtUnit_t {
    u32 color;
    union {
        s32 objectId;
        u32 firstCurrent;
    };
    union {
        u8 pad08[8];
        struct {
            u32 color08;
            u32 color0C;
        };
    };
    f32 vec10[4];
    f32 vec20[4];
    f32 vec30[4];
    union {
        u8 pad40[0x10];
        f32 vec40[4];
    };
    u32 color50;
    union {
        u8 pad54[0xC];
        struct {
            u32 color54;
            u32 color58;
            u32 color5C;
        };
    };
    /* Raidou: retail places color60 at 0x100, fields from 0x9C on at +0xA4,
     * motionSetup at +0xA6 and the transition timers at +0xB8. Only the observed offsets are known. */
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    f32 vec60[4];
    f32 vec70[4];
    f32 vec80[4];
    f32 vec90[4];
    u32 colorA0;
    u32 colorA4;
    u32 colorA8;
    u32 colorAC;
    f32 vecB0[4];
    f32 vecC0[4];
    f32 vecD0[4];
    f32 vecE0[4];
    u32 colorF0;
    u32 colorF4;
    u32 colorF8;
    u32 colorFC;
#endif
    u32 color60;
    union {
        u8 pad64[4];
        u32 color64;
    };
    void *endpointWork;
    union {
        u32 value;
        struct sdf3DLightParallelPacket_t *lightPacket;
    };
    f32 targetVector[4];           /* 0x70 */
    dds3Basic_t *effObj;           /* 0x80 */
    saddr_t currentTransitionValue;
    s32 previousTransitionValue;
    struct mdlObj_t *owner;
    void *linkedUnit;
    s32 (*motionCallback)(struct evtUnit_t *, s32);
    f32 wobblePhase;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    u8 padRaidou9C[4];
#endif
    saddr_t pathHandle;         /* 0x9C */
    f32 pathSpeed;
    u32 flags;                     /* 0xA4 */
    s16 motionState;               /* 0xA8 */
    s16 transitionSourceKind;
    s16 motionSubmode;
    s16 motionTicks;
    s16 motionParameter;
    s16 directionOffset;
    f32 transitionMotionScale;
    s16 waitMotion;
    s16 waitBlendStart;
    s16 waitBlendEnd;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    u8 padRaidouBE[2];
#endif
    evtUnitMotionSetup_t motionSetup; /* 0xBE */
    s16 motionEndFrame;
    s16 movingTicks;
    s16 idleTicks;
    s8 firstSlot;
    s8 secondSlot;
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    u8 padRaidou172;
    u8 ShadowAlpha;
    f32 ShadowY;
    u32 unk178;
    u32 unk17C;
#endif
    u8 slotFlags[12];              /* 0xCC */
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    u8 padRaidou18C[4];
#endif
    s16 tableValues[12];           /* 0xD8 */
    s16 slotA[12];
    s16 slotB[12];
    s16 slotC[12];
    union {
        s16 colorDuration;
        u16 colorFramesRemaining;
    };
    union {
        s16 directionDuration;
        u16 directionFramesRemaining;
    };
    union {
        u8 pad154[8];
        struct {
            u16 rgbElapsed;
            u16 rgbDuration;
            u16 alphaElapsed;
            u16 alphaDuration;
        };
    };
    s16 transitionElapsed;
    union {
        s16 transitionDuration;
        u16 transitionFrameCount;
    };
#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
    f32 moveY; /* 0x200: vertical offset applied by the submode 1 move */
    u8 pad204[2];
    s16 unk206;
    u8 pad208[0x28];
#endif
} evtUnit_t;

#if defined(VERSION_RAIDOU1) || defined(VERSION_RAIDOU2)
SIZE_CHECK(evtUnitSizeCheck, evtUnit_t, 0x230);
#endif



/* Transform prefix in the scene's type-17 source records. */
typedef struct {
    sdfVector4_t position;
    f32 rotation[4];
} evtWorldSourceTransformPrefix_t;

#endif /* EVENT_H */
