#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "sif_rpc.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfPrint.h"
#include "sdfSound.h"

struct sdfGsMemSlice_t;

typedef struct {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} sdfSceIoStat_t;

typedef struct
{
    u32 unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
} p2sdLoadEntry_t;

/* The PCM fill hook, held as an address and called through a cast. */
typedef u8 *(*p2sdPcmFunc_t)(s32);

extern u32 sdfSndMap[];

#define LastLoad ((p2sdLoadEntry_t *)((u8 *)sdfSndMap + 0x190))

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

typedef struct sndRingPacket_t {
    u32 transportHeader[3];
    u32 command;
    u32 sequence;
    u8 payload[108];
} sndRingPacket_t;

typedef struct sdfSndTrackPacket_t {
    u32 trackId;
    u32 unk4;
    u16 setting;
    u16 unkA;
    u32 unkC;
} sdfSndTrackPacket_t;

typedef struct sndSequenceVolumePanPacket_t {
    u32 trackId;
    u32 opaque04;
    u16 volume;
    u16 pan;
    u32 opaqueNotSent0C;
} sndSequenceVolumePanPacket_t;

typedef struct sndListenerState_t {
    s32 opaque00[2];
    s32 scaledValues[6];
} sndListenerState_t;

typedef struct sndFileRequestState_t {
    u8 path[0x100];
    s32 size;
    s32 pending;
    s32 data;
} sndFileRequestState_t;

typedef struct sndChannel_t {
    u8 index;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    u32 iopWorkAddress;
    u32 unkC;
} sndChannel_t;

typedef struct sndIopBuffer_t {
    s32 size;
    u32 iopAddress;
} sndIopBuffer_t;

typedef struct {
    u8 values[24];
    u8 pad18[8];
} sndMidiStateGroup_t;

typedef struct sndMergedWork_t {
    s32 header;
    s32 bufferCount;
    u32 iopBufferAddress;
    u32 iopBufferBytes;
    sndChannel_t channels[16];
    sndIopBuffer_t buffers[16];
    p2sdLoadEntry_t slots[7];
    void *eeWorkAddress;
    u32 eeWorkByteCount;
    u32 opaque1D0;
    u32 opaque1D4;
    u32 opaque1D8;
    u32 opaque1DC;
    u8 controlData[7][0x80];
    sndMidiStateGroup_t stateGroups[2];
} sndMergedWork_t;

extern sndMergedWork_t sdfSndWork FAKEMATCH_ASM_NAME("sdfSndMap");

typedef struct sdfSoundRpcRequest_t {
    u8 opaque00[8];
    s32 destination;
    s32 byteCount;
} sdfSoundRpcRequest_t;

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

typedef struct {
    u32 control;
    u32 address;
    u32 vifCode0;
    u32 vifCode1;
} sdfDmaDumpTag_t;

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};


#include "sdk_libc.h"

extern char sdfDumpAddrFmt[];
extern char sdfDumpPadStr[];
extern char sdfDumpMidGapStr[];
extern char sdfDumpByteSepStr[];
extern char sdfDumpByteFmt[];
extern char sdfDumpLineBuf[];
extern char sdfDumpEndStr[];
extern const char sdfDmaDumpStartFmt[];
extern const char sdfDmaNotQwordFmt[];
extern const char sdfDmaTagLineFmt[];
extern const char sdfDmaNestOverflowMsg[];
extern char *sdfDmaTagNameTable[];
sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);
extern const char sdfGsMemInfoTitle[];
extern const char sdfGsMemNodeFmt[];
extern char sdfGsMemTypeFmt[];
extern char *sdfGsMemTypeNames[];
sdfGsMemSlice_t *sdfGsMemGetLinkTail(void);
extern char *sdfGsMemTypeNamesTail[];

typedef struct sdfFileCmdHandle_t sdfFileCmdHandle_t;

void sdfGrFlushOtChain(u32 *source);

typedef struct sdfSndInitThreadParam_t {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 reserved[0x18];
} sdfSndInitThreadParam_t;

#include "sdk_libkernl.h"

extern const char sdfDmaDumpStartFmt[29];
extern const char sdfDmaNotQwordFmt[21];
extern const char sdfDmaTagLineFmt[33];
extern const char sdfDmaNestOverflowMsg[27];
void sdfDumpDmaChain(sdfDmaDumpTag_t *source);
extern char *sdfGrVifUnpackNames[];
extern s32 sdfGrVifUnpackBits[];
extern char sdfVifMaskedName[];
extern char sdfVifXferOkName[];
extern char sdfDumpIndentStr[];
extern char sdfDumpWordFmt[];
void sdfDumpGsMemory(void);
void sdfDumpGsMemory2(void);
extern const char sdfGsMemInfoTitle[116];
extern const char sdfGsMemNodeFmt[31];
void sdfDumpMemory(void);
void sdfDumpChipMemory(void);
extern char sdfDumpAddrFmt[8];
extern char sdfDumpPadStr[8];
extern char sdfDumpMidGapStr[8];
extern char sdfDumpByteSepStr[8];
extern char sdfDumpByteFmt[8];
extern char sdfDumpLineBuf[2];
extern char sdfDumpEndStr[8];
extern char sdfVifMaskedName[8];
extern char sdfVifXferOkName[8];
extern char sdfVifUnpackV4_5Name[8];
extern char sdfVifUnpackV4_8Name[8];
extern char sdfVifUnpackV4_16Name[8];
extern char sdfVifUnpackV4_32Name[8];
extern char sdfVifUnpackV3_8Name[8];
extern char sdfVifUnpackV3_16Name[8];
extern char sdfVifUnpackV3_32Name[8];
extern char sdfVifUnpackV2_8Name[8];
extern char sdfVifUnpackV2_16Name[8];
extern char sdfVifUnpackV2_32Name[8];
extern char sdfVifUnpackS_8Name[8];
extern char sdfVifUnpackS_16Name[8];
extern char sdfVifUnpackS_32Name[8];
extern char sdfVifFlagsMruName[8];
extern char sdfVifFlagsRuName[8];
extern char sdfVifFlagsMrName[8];
extern char sdfVifFlagsRName[8];
extern char sdfVifFlagsMuName[8];
extern char sdfVifFlagsUName[8];
extern char sdfVifFlagsMName[8];
extern char sdfVifFlagsNoneName[8];
extern char sdfDumpIndentStr[8];
extern char sdfDumpWordFmt[8];
extern char sdfGsMemClutName[8];
extern char sdfGsMemTexName[8];
extern char sdfGsMemFrameName[8];
extern char sdfGsMemFreeName[8];
extern char sdfGsMemTypeFmt[8];

/* Print size bytes in address-aligned, sixteen-column hexadecimal rows. */
void sdfDumpHexLine(const void *aData, s32 size) {
    const u8 *data = aData;
    s32 column = 0;
    s32 i;
    while (size > 0) {
        if (column == 0) {
            sdfPrintf(sdfDumpAddrFmt, data);
            column = EE_ADDR(data) & 0xF;
            for (i = column; i > 0; i--) {
                sdfPrintf(sdfDumpPadStr);
            }
        } else if (column == 8) {
            sdfPrintf(sdfDumpMidGapStr);
        } else {
            sdfPrintf(sdfDumpByteSepStr);
        }
        sdfPrintf(sdfDumpByteFmt, *data++);
        column++;
        if (column == 0x10) {
            column = 0;
            sdfPrintf(sdfDumpLineBuf);
        }
        size--;
    }
    if (column == 0) {
        sdfPrintf(sdfDumpLineBuf);
    } else {
        sdfPrintf(sdfDumpEndStr);
    }
}

const char sdfDmaDumpStartFmt[29] = "<<< DMA dump start [%p] >>>\n";

const char sdfDmaNotQwordFmt[21] = "%p not qword align.\n";

const char sdfDmaTagLineFmt[33] = "%08X : [%s] %08X %08X %08X %08X\n";

const char sdfDmaNestOverflowMsg[27] = "call stack nest overflow!\n";

void sdfDumpDmaChain(sdfDmaDumpTag_t *source) {
    sdfDmaDumpTag_t *callStack[2];
    sdfDmaDumpTag_t *cursor;
    u32 tag;
    u32 tagAddress;
    s32 tagType;
    s32 tagCount;
    s32 depth = 0;
    s32 done = 0;
    sdfPrintf(sdfDmaDumpStartFmt, source);
    cursor = source;
    for (;;) {
        if (EE_ADDR(cursor) & 0xF) {
            sdfPrintf(sdfDmaNotQwordFmt, cursor);
            return;
        }
        tag = cursor->control;
        tagAddress = cursor->address;
        tagType = (tag >> 28) & 7;
        tagCount = tag & 0xFFFF;
        sdfPrintf(sdfDmaTagLineFmt, cursor, sdfDmaTagNameTable[tagType], tag, tagAddress, cursor->vifCode0, cursor->vifCode1);
        cursor++;
        switch (tagType) {
        case 0:
            done = 1;
            break;
        case 1:
            cursor += tagCount;
            break;
        case 2:
            cursor = EE_ADDR_AS(sdfDmaDumpTag_t *, tagAddress);
            break;
        case 3:
        case 4:
            break;
        case 5:
            if (depth == 2) {
                sdfPrintf(sdfDmaNestOverflowMsg);
                return;
            }
            callStack[depth++] = cursor;
            cursor = EE_ADDR_AS(sdfDmaDumpTag_t *, tagAddress);
            break;
        case 6:
            if (depth == 0) {
                done = 1;
            } else {
                cursor = callStack[--depth];
            }
            break;
        case 7:
            cursor += tagCount;
            done = 1;
            break;
        }
        if (done != 0) {
            sdfPrintf("<<< DMA normal end [%p] >>>\n", cursor);
            return;
        }
    }
}

void sdfGrFlushOtChain(u32 *source) {
    u32 *returnStack[2];
    u32 *cursor;
    u32 *wordCursor = NULL;
    s32 dataMode = 0;
    s32 tagId = 0;
    s32 depth = 0;
    s32 n = 0;
    s32 directRow;
    s32 phase;
    s32 wordsLeft;
    u32 tagAddress;
    s32 qwc;
    s32 done;
    u32 value;

    sdfPrintf("<<< DMA-VIF dump start [%p] >>>\n", source);
    cursor = source;
    directRow = 0;
    tagAddress = 0;
    qwc = 0;
    phase = 0;
    wordsLeft = 0;
    done = 0;
    for (;;) {
        if (wordsLeft == 0) {
            switch (phase) {
            case 0: {
                u32 tag;
                if (EE_ADDR(cursor) & 0xF) {
                    sdfPrintf(sdfDmaNotQwordFmt, cursor);
                    return;
                }
                tag = cursor[0];
                tagAddress = cursor[1];
                tagId = (tag >> 28) & 7;
                qwc = tag & 0xFFFF;
                sdfPrintf("\n%08X [DMA %s] : %08X %08X\n",
                                           cursor, sdfDmaTagNameTable[tagId], tag, tagAddress);
                wordCursor = cursor + 2;
                cursor += 4;
                wordsLeft = 2;
                phase = 1;
                break;
            }
            case 1:
                wordsLeft = qwc * 4;
                switch (tagId) {
                case 0:
                case 3:
                case 4:
                    wordCursor = EE_ADDR_AS(u32 *, tagAddress);
                    break;
                default:
                    wordCursor = cursor;
                    break;
                }
                phase = 2;
                break;
            case 2:
                switch (tagId) {
                case 0:
                case 7:
                    done = 1;
                    break;
                case 1:
                    cursor += qwc * 4;
                    break;
                case 2:
                    cursor = EE_ADDR_AS(u32 *, tagAddress);
                    break;
                case 3:
                case 4:
                    break;
                case 5:
                    if (depth == 2) {
                        sdfPrintf("\ncall stack nest overflow!\n");
                        return;
                    }
                    returnStack[depth++] = cursor + qwc * 4;
                    cursor = EE_ADDR_AS(u32 *, tagAddress);
                    break;
                case 6:
                    if (depth == 0) {
                        done = 1;
                    } else {
                        depth--;
                        cursor = returnStack[depth];
                    }
                    break;
                }
                phase = 0;
                break;
            }
        } else {
            value = *wordCursor++;
            wordsLeft--;
            switch (dataMode) {
            case 0: {
                u32 opcode = (value >> 24) & 0x7F;
                u32 num = (value >> 16) & 0xFF;
                u32 immediate = value & 0xFFFF;
                s32 interrupt = (value >> 31) ? 'i' : ' ';
                s32 bits;
                s32 format;
                s32 unpackFormat;

                sdfPrintf("%08X %c%08X : ", EE_ADDR((wordCursor - 1)),
                                           interrupt, value);
                switch (opcode) {
                case 0x00:
                    sdfPrintf("VIF NOP\n");
                    break;
                case 0x01:
                    sdfPrintf("VIF STCYCL (CL=%d, WL=%d)\n", (s32)(immediate & 0xFF),
                                    (s32)(immediate >> 8));
                    break;
                case 0x02:
                    sdfPrintf("VIF OFFSET (%04X)\n", immediate & 0x3FF);
                    break;
                case 0x03:
                    sdfPrintf("VIF BASE   (%04X)\n", immediate & 0x3FF);
                    break;
                case 0x04:
                    sdfPrintf("VIF ITOP   (%04X)\n", immediate & 0x3FF);
                    break;
                case 0x05:
                    sdfPrintf("VIF STMOD  (%d)\n", (s32)(immediate & 3));
                    break;
                case 0x06:
                    sdfPrintf("VIF MSKPATH3 (%s)\n",
                                    (immediate & 0x8000) ? sdfVifMaskedName : sdfVifXferOkName);
                    break;
                case 0x07:
                    sdfPrintf("VIF MARK   (%04X)\n", immediate);
                    break;
                case 0x10:
                    sdfPrintf("VIF FLUSHE\n");
                    break;
                case 0x11:
                    sdfPrintf("VIF FLUSH\n");
                    break;
                case 0x13:
                    sdfPrintf("VIF FLUSHA\n");
                    break;
                case 0x14:
                    sdfPrintf("VIF MSCAL  (%04X)\n", immediate);
                    break;
                case 0x15:
                    sdfPrintf("VIF MSCALF (%04X)\n", immediate);
                    break;
                case 0x17:
                    sdfPrintf("VIF MSCNT\n");
                    break;
                case 0x20:
                    dataMode = 1;
                    sdfPrintf("VIF STMASK\n");
                    break;
                case 0x30:
                    dataMode = 2;
                    n = 4;
                    sdfPrintf("VIF STROW\n");
                    break;
                case 0x31:
                    dataMode = 3;
                    n = 4;
                    sdfPrintf("VIF STCOL\n");
                    break;
                case 0x4A:
                    n = num * 2;
                    sdfPrintf("VIF MPG   (%04X)\n", immediate);
                    if (n == 0) {
                        n = 0x200;
                    }
                    dataMode = 4;
                    break;
                case 0x50:
                    n = immediate * 4;
                    if (n == 0) {
                        n = 0x40000;
                    }
                    sdfPrintf("VIF DIRECT (%06X bytes)\n", (u32)(n * 4));
                    dataMode = 5;
                    directRow = 0;
                    break;
                case 0x51:
                    n = immediate * 4;
                    if (n == 0) {
                        n = 0x40000;
                    }
                    sdfPrintf("VIF DIRECTHL (%06X bytes)\n", (u32)(n * 4));
                    dataMode = 6;
                    directRow = 0;
                    break;
                default:
                    if ((opcode & 0x60) == 0x60) {
                        unpackFormat = opcode & 0xF;
                        if (((char **)((u8 *)sdfDmaTagNameTable + 0x20))[unpackFormat] == NULL) {
                            sdfPrintf("illegal unpack type !\n");
                            return;
                        }
                        format = (((s32)immediate >> 13) & 6) | (((s32)opcode >> 4) & 1);
                        dataMode = 7;
                        sdfPrintf("VIF UNPACK%s %s,%04X,%04X\n",
                                                   sdfGrVifUnpackNames[format],
                                                   ((char **)((u8 *)sdfDmaTagNameTable + 0x20))[unpackFormat],
                                                   immediate & 0x3FF, num);
                        bits = sdfGrVifUnpackBits[unpackFormat];
                        n = (bits * (s32)num + 0x1F) >> 5;
                    } else {
                        sdfPrintf("unkown vif command\n");
                        return;
                    }
                    break;
                }
                break;
            }
            case 1:
                sdfPrintf("    STMASK : %08X\n", value);
                dataMode = 0;
                break;
            case 2:
                n--;
                sdfPrintf("    STROW : %08X\n", value);
                if (n == 0) {
                    dataMode = 0;
                }
                break;
            case 3:
                n--;
                sdfPrintf("    STCOL : %08X\n", value);
                if (n == 0) {
                    dataMode = 0;
                }
                break;
            case 4:
                if (--n == 0) {
                    dataMode = 0;
                }
                break;
            case 5:
                if (directRow == 0) {
                    sdfPrintf(sdfDumpIndentStr);
                } else {
                    sdfPrintf(sdfDumpByteSepStr);
                }
                directRow++;
                sdfPrintf(sdfDumpWordFmt, value);
                if (directRow == 4) {
                    directRow = 0;
                    sdfPrintf(sdfDumpLineBuf);
                }
                if (--n == 0) {
                    if (directRow != 0) {
                        sdfPrintf(sdfDumpLineBuf);
                    }
                    dataMode = 0;
                }
                break;
            case 6:
                if (--n == 0) {
                    dataMode = 0;
                }
                break;
            case 7:
                if (--n == 0) {
                    dataMode = 0;
                }
                break;
            }
        }
        if (done != 0) {
            sdfPrintf("<<< DMA-VIF normal end [%p] >>>\n", (void *)wordCursor);
            return;
        }
    }
}

void sdfDumpGsMemory(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;
    sdfPrintf(sdfGsMemInfoTitle);
    head = sdfGsMemGetLinkHead();
    node = head;
    while (node != 0) {
        if ((u32)node->type < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = sdfGsMemTypeNames[node->type];
        } else {
            sdfPrintf(buf, sdfGsMemTypeFmt, node->type);
            name = buf;
        }
        sdfPrintf(sdfGsMemNodeFmt, node, node->prev, node->next, name, node->addr, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->next;
        }
        node = node->next;
    }
}

void sdfDumpGsMemory2(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;
    sdfPrintf(sdfGsMemInfoTitle);
    head = sdfGsMemGetLinkTail();
    node = head;
    while (node != 0) {
        if ((u32)node->type < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = sdfGsMemTypeNamesTail[node->type];
        } else {
            sdfPrintf(buf, sdfGsMemTypeFmt, node->type);
            name = buf;
        }
        sdfPrintf(sdfGsMemNodeFmt, node, node->prev, node->next, name, node->addr, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->prev;
        }
        node = node->prev;
    }
}

const char sdfGsMemInfoTitle[116] = " <<< GS memory information >>>\n -----------------------------\n address  : previous   next     type   GS-addr  size\n";

const char sdfGsMemNodeFmt[31] = " %08X : %08X %08X %8s %08X %d\n";

void sdfDumpMemory(void) {
    sdfMemInfo_t info;
    sdfMemGetInfo(&info);
    sdfPrintf(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info.total, info.freeSize,
                    info.maxFreeSize, info.minFreeSize,
                    info.handleCount, info.freeHandleCount);
}

/* Print the chip heap totals and how many cells are in use per size class (1..16, 17..32, ...). */
void sdfDumpChipMemory(void) {
    sdfChipInfo_t stats;
    u32 limit = 16;
    s32 i = 0;
    u32 first;
    u32 *used;
    sdfChipGetInfo(&stats);
    sdfPrintf(" <<< chip memory information >>>\n                 total : 0x%06X\n            free total : 0x%06X\n            page count : %d\n       free page count : %d\n fragmented page count : %d\n", stats.total, stats.freeSize, stats.pageCount, stats.freePageCount, stats.fragmentedPageCount);
    sdfPrintf("\n several size alloc count...\n");
    first = 0;
    used = stats.severalAllocCount;
    do {
        sdfPrintf("       for %3d .. %4d : %d\n", first, limit, *used++);
        i++;
        first = limit + 1;
        limit *= 2;
    } while (i != 7);
    sdfPrintf(sdfDumpLineBuf, first);
}
char sdfDumpAddrFmt[8] = "%08X : ";

char sdfDumpPadStr[8] = "   ";

char sdfDumpMidGapStr[8] = "-";

char sdfDumpByteSepStr[8] = " ";

char sdfDumpByteFmt[8] = "%02X";

char sdfDumpLineBuf[2] = "\n";

char sdfDumpEndStr[8] = "\n\n";

char sdfVifMaskedName[8] = "mask";

char sdfVifXferOkName[8] = "xfer-ok";

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV4_5Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV4_8Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV4_16Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV4_32Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV3_8Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV3_16Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV3_32Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV2_8Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV2_16Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackV2_32Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackS_8Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackS_16Name);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfSoundData);

#endif
INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifUnpackS_32Name);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsMruName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsRuName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsMrName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsRName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsMuName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsUName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsMName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfVifFlagsNoneName);

char sdfDumpIndentStr[8] = "    ";

char sdfDumpWordFmt[8] = "%08X";

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfGsMemClutName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfGsMemTexName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfGsMemFrameName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfGsMemFreeName);

INCLUDE_SDATA(const s32, "sdf/sdfSound_00371BF0", sdfGsMemTypeFmt);

