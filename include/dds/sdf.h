#ifndef DDS_SDF_H

#define DDS_SDF_H
#include "../draw.h"
#include "../libsdf.h"
#include "../sdf_movie_types.h"
#include "common.h"

#include "sdf_resource.h"

#include "sdf_types.h"

#include "sdf_list_node.h"

#include "sdf_gs_packet.h"

#include "sdf_texture_clamp.h"

#include "sdf_stream_read_contract.h"

#include "sdf_thread.h"

/* Native battle-parameter blobs: DDS1 0xA6C bytes, DDS2 0xC14 bytes.
 * Level tables begin at level one; seven-entry scales use index three for zero. */
typedef struct SdfBattleMoneyRewardBand {
    s8 levelThreshold;
    u8 pad01[3];
    f32 moneyScale;
} SdfBattleMoneyRewardBand;

typedef char SdfBattleMoneyRewardBand_size_must_be_8[(sizeof(SdfBattleMoneyRewardBand) == 8) ? 1 : -1];

typedef struct SdfBattleParameters {
    f32 maxHpGrowth[99]; /* 0x000 */
    f32 maxMpGrowth[99]; /* 0x18C */
    f32 hpGradeValues[8]; /* 0x318 */
    f32 hpFineGradeValues[11]; /* 0x338 */
    f32 levelValuesA[99]; /* 0x364 */
    f32 levelValuesB[99]; /* 0x4F0 */
    f32 levelValuesC[99]; /* 0x67C */
    u8 pad808[0xB8];
    f32 adjustmentScale[7]; /* 0x8C0: signed adjustment -3..3 */
    u8 rewardLevelAllowance; /* 0x8DC: level gap before reducing defeat experience. */
    u8 pad8DD[3];
    f32 rewardDivisor; /* 0x8E0: zero leaves defeat experience unreduced. */
    f32 moneyTurnScales[16]; /* 0x8E4 */
    SdfBattleMoneyRewardBand moneyRewardBands[10]; /* 0x924 */
    f32 rewardLevelScale[31 * 2]; /* 0x974: level difference and reward kind */
#ifdef VERSION_DDS2
    f32 gradeScale[11]; /* 0xA6C: grade -5..5, centre at 0xA80 */
    f32 solarScale[2][9]; /* 0xA98: row 0 for source flag 0x200, row 1 otherwise. */
    f32 partyEntryScaleA[7]; /* 0xAE0 */
    f32 enemyEntryScaleA[7]; /* 0xAFC */
    f32 partyEntryScaleB[7]; /* 0xB18 */
    f32 enemyEntryScaleB[7]; /* 0xB34 */
    u8 padB50[0x14];
    f32 unkB64; /* 0xB64: command factor gated by model flag 0x80E and source/target flags. */
    u8 padB68[4];
    f32 specialAffinityScale; /* 0xB6C */
    f32 criticalScale; /* 0xB70: special-mode multiplier for the critical roll. */
    u8 padB74[4];
    f32 specialEncounterScale; /* 0xB78: scales the solar-phase chance after an attempted special encounter. */
    f32 preemptiveModeScale; /* 0xB7C: encounter-kind-3 preemptive chance scale. */
    f32 majinRewardScale; /* 0xB80: DDS2 battle-mode-3 experience multiplier. */
    f32 unkB84; /* 0xB84: source-conditioned factor, DDS2 001B1B64. */
    f32 unkB88; /* 0xB88: target-conditioned factor, DDS2 001B1B8C. */
    u8 unkB8C; /* 0xB8C: source-conditioned minimum chance, DDS2 001B1E90. */
    u8 unkB8D; /* 0xB8D: target-conditioned minimum chance, DDS2 001B1EBC. */
    u8 padB8E[2];
    f32 unkB90; /* 0xB90: command factor for a source with record flag 0x10 and unit flag 0x200. */
    f32 unkB94; /* 0xB94: corresponding target-conditioned command factor. */
    s8 criticalPartyAttackerBias; /* 0xB98: signed critical chance adjustments. */
    s8 criticalPartyDefenderBias; /* 0xB99 */
    u8 specialEncounterChance[9]; /* 0xB9A: unsigned chance by mirrored solar phase, DDS2 001B40E8. */
    u8 padBA3;
    f32 partyHpScale[10]; /* 0xBA4 */
    f32 enemyHpScale[10]; /* 0xBCC */
    f32 hekatoRatioScale; /* 0xBF4 */
    f32 hekatoRatioMax; /* 0xBF8 */
    f32 defaultElementAbilityScale; /* 0xBFC: party-unit fallback without an ability bonus. */
    f32 actionScale; /* 0xC00 */
    f32 brahmaRatioMultiplier; /* 0xC04 */
    f32 brahmaRatioMaximum; /* 0xC08 */
    f32 specialActionScale; /* 0xC0C */
    u8 padC10[4];
#endif
} SdfBattleParameters;

#ifdef VERSION_DDS1

typedef char SdfBattleParameters_size_must_be_0xA6C[(sizeof(SdfBattleParameters) == 0xA6C) ? 1 : -1];

#endif

#ifdef VERSION_DDS2

typedef char SdfBattleParameters_size_must_be_0xC14[(sizeof(SdfBattleParameters) == 0xC14) ? 1 : -1];

#endif

extern SdfBattleParameters *datBattleParameters;

/* Four integer words used as a camera input record (0x10), not ScrVec4 floats. */
typedef struct SdfQuad {
    u32 word[4];
} SdfQuad;

/* IPU register state saved across stream callbacks (0x24 bytes). */
typedef struct IpuDmaState {
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
} IpuDmaState;

typedef char IpuDmaState_size_must_be_0x24[
    (sizeof(IpuDmaState) == 0x24) ? 1 : -1];

/* audioMode stores these decoded sound modes in a byte; keep the field byte-sized. */
typedef enum SdfStreamAudioMode {
    SDF_STREAM_AUDIO_DISABLED = 0,
    SDF_STREAM_AUDIO_MONO = 1,
    SDF_STREAM_AUDIO_STEREO = 2
} SdfStreamAudioMode;

/* One 0x8C allocation owns the stream/sound links and IPU transfer state. */
struct SdfStreamInputDmaTag;

struct movieStream_t;

struct SoundFormat;

/* Store the stream-frame draw-path word unchanged; 1 selects the alternate path. */
void sdfSetStreamFrameDrawMode(u32 mode);

typedef struct SdfMovieDescriptor {
    u16 drawX;
    u16 drawY;
    u32 drawZ;
    u16 drawWidth;
    u16 drawHeight;
    s32 source;
    u8 hasAudio;
    u8 stereo;
    u8 playbackCadenceStep;
    u8 pad13;
} SdfMovieDescriptor;

typedef char SdfMovieDescriptor_size_must_be_0x14[
    (sizeof(SdfMovieDescriptor) == 0x14) ? 1 : -1];

/* The +0x1C movie work pointer selects one of two separately allocated records.
 * isPac at +3 chooses the active arm; the variant payload types live in
 * sdf_movie_stream.h. */
struct MovLinearStream;

struct MovPacStream;

typedef union MovObjStream {
    struct MovLinearStream *linear;
    struct MovPacStream *pac;
} MovObjStream;

typedef u8 SdfMovieState;

#if !defined(SMT3_DEFINES_movieStream_t)
typedef struct movieStream_t {
    u8 active;
    SdfMovieState state;
    u8 stopRequested;
    u8 isPac;
    s16 drawX;
    s16 drawY;
    u32 drawZ;
    s16 drawWidth;
    s16 drawHeight;
    struct sdfFileCmdHandle_t *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    MovObjStream stream;
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    sdfIpuDecodeCb_t soundNode;
} movieStream_t;
#endif

typedef char MovObj_size_must_be_0xB0[
    (sizeof(movieStream_t) == 0xB0) ? 1 : -1];

/* A mode's 12-byte display defaults row, copied into sdfGrFrameSet_t. */
typedef struct SdfGraphModeDefaults {
    s16 width;
    s16 unk2;
    s16 height;
    u16 bufferFormat;
    u16 auxiliaryFormat;
    u16 interlace;
} SdfGraphModeDefaults;

typedef char SdfGraphModeDefaults_size_must_be_0xC[
    (sizeof(SdfGraphModeDefaults) == 0xC) ? 1 : -1];

/* Release one caller-owned texture reference. The direct form releases the
 * texture immediately when this is the last reference; the handler form queues
 * that final release for deferred processing. Neither function consumes more
 * than the single reference represented by the supplied pointer. */
void sdfTexReleaseReference(sdfTexHandle_t *texture);

/* Semaphore ID and attached work pointers (0x14); DDS1/2 game/code_002D10B0/00329F60.c. */

/* Callback-list hooks also accept the shared task-entry no-op. */
typedef void (*SdfListCallback)();

struct sdfMemHandle_t;

typedef struct SdfList {
    struct sdfMemHandle_t *allocation;
    u32 count;
    SdfListNode *head;
    SdfListNode *tail;
    void *userData;
    SdfListCallback onRemove;
    SdfListCallback onDestroy;
} SdfList;

typedef char SdfList_size_must_be_0x1C[(sizeof(SdfList) == 0x1C) ? 1 : -1];

SdfList *sdfCreateTaskHeader(void *);

void sdfDestroyTaskWork(SdfList *);

void sdfSetTaskDestroyCallback(SdfList *, SdfListCallback);

void sdfSetTaskSecondaryCallback(SdfList *, SdfListCallback);

SdfListNode *sdfListAppend(SdfList *, s32, void *);

SdfListNode *sdfListInsertAfter(SdfList *, SdfListNode *, s32, void *);

SdfListNode *sdfRemoveAndReindexListNode(SdfList *, SdfListNode *);

SdfListNode *sdfListRemoveNode(SdfList *, SdfListNode *);

void sdfClearTaskList(SdfList *);

void sdfSwapLinkedListNodes(SdfList *, SdfListNode *, SdfListNode *);

SdfListNode *sdfFindTaskListNodeByKey(SdfList *, s32);

/* Four-doubleword DMA packet payload (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfPacket {
    u64 unk0;
    u64 unk8;
    u64 unk10;
    u64 unk18;
} SdfPacket;

/* DMA source header and trailing 64-bit field (0x10); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfDmaSrc {
    u16 quadwordCount;
    u8 pad2[6];
    u64 vifCommands;
} SdfDmaSrc;

/* Resource entry with word at +0xC (0x10); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfResEntry {
    u8 pad00[0xC];
    u32 baseAddress; /* Shifted right six bits when patching a GS texture base. */
} SdfResEntry;

/* Frame/depth builder state: one DMA/GIF header and two A+D register writes. */
typedef struct SdfPacketBuilder {
    u8 pad00[4];
    void (*prepare)(struct SdfPacketBuilder *, s32 bufferIndex);
    u8 pad08[8];
    SdfGsPacketHeader packetHeader;
    SdfGsRegisterWrite frame;
    SdfGsRegisterWrite zbuf;
    sdfGrFrameSet_t *source;
    u32 frameMask;
    s32 region;
    s32 mode;
} SdfPacketBuilder;

typedef char SdfPacketBuilder_size_must_be_0x60[
    (sizeof(SdfPacketBuilder) == 0x60) ? 1 : -1];

/* Optional packed or five-float state block owned by an sdfMat_t. */
typedef union SdfSubParam {
    struct {
        u64 unk0;
        u64 unk8;
        u32 unk10;
        u32 unk14;
    } packed;
    struct {
        f32 values[5];
        u32 unk14;
    } scalar;
    /* Named view of scalar.values; the last word remains opaque. */
    struct {
        f32 centerOffsetX;
        f32 centerOffsetY;
        f32 basisScaleX;
        f32 basisScaleY;
        f32 rotationInput;
        u32 unk14;
    } transformInputs;
} SdfSubParam;

typedef char SdfSubParam_size_must_be_0x18[
    (sizeof(SdfSubParam) == 0x18) ? 1 : -1];

extern s32 sdfThreadSema;

/* Embedded end sentinels and backing-span metadata for the general heap (0x28). */
typedef struct SdfMemHeap {
    sdfMemHandle_t head; /* 0x00: low-address sentinel */
    sdfMemHandle_t tail; /* 0x10: high-address sentinel */
    u32 base;         /* 0x20: unaligned backing allocation */
    u32 size;         /* 0x24 */
} SdfMemHeap;

typedef char SdfMemHeap_size_must_be_0x28[(sizeof(SdfMemHeap) == 0x28) ? 1 : -1];

enum SdfColorTrackMode {
    SDF_COLOR_TRACK_MODE_ENDPOINTS = 0,
    SDF_COLOR_TRACK_MODE_ONE_INTERMEDIATE_KEY = 1,
    SDF_COLOR_TRACK_MODE_TWO_INTERMEDIATE_KEYS = 2
};

/* The flag-list factory appends this owner after its vertex, color and mark
 * arrays. Its parameter record is copied whole into the owner. */
typedef struct SdfColorTrack {
    u8 segmentMode; /* enum SdfColorTrackMode, stored as a byte */
    u8 pad01[3];
    u32 initialColor;
    u32 finalColor;
    /* The editor selects either intermediate key with an eight-byte stride. */
    struct {
        u32 color;
        f32 fraction;
    } keys[2];
    u8 pad1C[8];
} SdfColorTrack;

typedef struct SdfAlphaTrack {
    u32 alpha;
    s32 surfaceIndex;
    f32 fadeInFraction;
    f32 fadeOutFraction;
} SdfAlphaTrack;

typedef struct SdfFlagListParams {
    SdfColorTrack color;
    SdfAlphaTrack alpha;
    s32 maxFrames;
    u32 count;
    f32 speed;
} SdfFlagListParams;

typedef struct SdfFlagListMark {
    s32 timer;
    u8 alpha;
    u8 pad05[3];
} SdfFlagListMark;

typedef struct SdfFlagListWork {
    s32 frame;
    u32 unk04;
    SdfFlagListMark *marks;
    f32 (*vertices)[4];
    u32 *colors;
    SdfFlagListParams params;
    sdfMemHandle_t *allocation;
} SdfFlagListWork;

typedef char SdfColorTrack_key_stride_must_be_8[(sizeof(((SdfColorTrack *)0)->keys[0]) == 8) ? 1 : -1];

typedef char SdfColorTrack_size_must_be_0x24[(sizeof(SdfColorTrack) == 0x24) ? 1 : -1];

typedef char SdfColorTrack_fields_offset[((unsigned long)&((SdfColorTrack *)0)->segmentMode == 0x00 &&
                                          (unsigned long)&((SdfColorTrack *)0)->initialColor == 0x04 &&
                                          (unsigned long)&((SdfColorTrack *)0)->finalColor == 0x08 &&
                                          (unsigned long)&((SdfColorTrack *)0)->keys[0].color == 0x0C &&
                                          (unsigned long)&((SdfColorTrack *)0)->keys[0].fraction == 0x10 &&
                                          (unsigned long)&((SdfColorTrack *)0)->keys[1].color == 0x14 &&
                                          (unsigned long)&((SdfColorTrack *)0)->keys[1].fraction == 0x18) ? 1 : -1];

typedef char SdfAlphaTrack_size_must_be_0x10[(sizeof(SdfAlphaTrack) == 0x10) ? 1 : -1];

typedef char SdfAlphaTrack_fields_offset[((unsigned long)&((SdfAlphaTrack *)0)->alpha == 0x00 &&
                                          (unsigned long)&((SdfAlphaTrack *)0)->surfaceIndex == 0x04 &&
                                          (unsigned long)&((SdfAlphaTrack *)0)->fadeInFraction == 0x08 &&
                                          (unsigned long)&((SdfAlphaTrack *)0)->fadeOutFraction == 0x0C) ? 1 : -1];

typedef char SdfFlagListParams_size_must_be_0x40[(sizeof(SdfFlagListParams) == 0x40) ? 1 : -1];

typedef char SdfFlagListMark_size_must_be_0x08[(sizeof(SdfFlagListMark) == 0x08) ? 1 : -1];

typedef char SdfFlagListWork_size_must_be_0x58[(sizeof(SdfFlagListWork) == 0x58) ? 1 : -1];

typedef char SdfFlagListWork_allocation_offset_must_be_0x54[
    ((unsigned long)&((SdfFlagListWork *)0)->allocation == 0x54) ? 1 : -1];

SdfFlagListWork *sdfCreateFlagListWork(const SdfFlagListParams *);

void sdfResetFlagListEntries(SdfFlagListWork *);

void sdfReleaseFlagListResource(SdfFlagListWork *);

SdfFlagListWork *sdfInitializeFlagListFromResource(void *);

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *);

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *);

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *);

void effReleaseSelectionFlagList(SdfFlagListWork *);

void effResetSelectionEntryBuffers(SdfFlagListWork *);

void effResetSelectionEntriesAndState(SdfFlagListWork *);

void effDrawSelectionEntryVectors(SdfFlagListWork *);

void effUpdateAndDrawSelectionEntries(SdfFlagListWork *);

float sdfGetFlagListSpeed(SdfFlagListWork *);

void sdfSetFlagListSpeed(SdfFlagListWork *, float);

u32 sdfGetFlagListAlphaSurfaceIndex(SdfFlagListWork *);

void sdfSetFlagListAlphaSurfaceIndex(SdfFlagListWork *, u32 surfaceIndex);

void itfSetPackedRgbAlpha(SdfFlagListWork *, u32);

void itfCopyColorFields(SdfFlagListWork *, const SdfFlagListParams *);

#ifdef VERSION_DDS1

void sdfUpdateFlagListEntries(SdfFlagListWork *);

void sdfDrawFlagListVertices(SdfFlagListWork *);

void effUpdateSelectionEntryState(SdfFlagListWork *);

#elif defined(VERSION_DDS2)

void sdfUpdateFlagListEntries(SdfFlagListWork *);

void sdfDrawFlagListVertices(SdfFlagListWork *);

void effUpdateSelectionEntryState(SdfFlagListWork *);

#endif

/* SDF chunk fourCC values (little-endian byte order). */

#define SDF_CHUNK_UNIQUE_VALUE 0x51494e55 /* "UNIQ" in little-endian byte order */

#define SDF_CHUNK_MAP_POSITIONS 0x534F504D /* "MPOS" in little-endian byte order */

#define SDF_CHUNK_LOD_VALUE 0x43444f4c /* "LODC" in little-endian byte order */

/* Free-list kind for SDF pools. */

#define SDF_POOL_FREE_KIND 0xFFFF

/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
typedef struct SdfPoolNode {
    struct SdfPoolNode *next; /* 0x00 */
    sdfDrawTag_t *first;      /* 0x04 */
    sdfDrawTag_t *last;       /* 0x08 */
    u32 unkC;
    void (*append)(struct SdfPoolNode *, sdfDrawTag_t *);      /* 0x10 */
    s32 (*prepend)(struct SdfPoolNode *, s32, sdfDrawTag_t *); /* 0x14 */
    u32 unk18;
    u32 unk1C;
} SdfPoolNode;

#endif /* DDS_SDF_H */
