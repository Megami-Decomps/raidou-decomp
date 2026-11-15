#include "common.h"
#include "sdk_libc.h"
#include "sdk_libgcc.h"
#include "newlib_stat.h"
#include "ps2asm.h"

enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};

struct sdfDrawTag_t;

struct sdfSortOtNode_tag;

struct sdfGrOt_t;

#define SDF_POOL_FREE_KIND 0xFFFF

#include "sdk_libkernl.h"

/* Pool release logic tests the packet head address and kind at 0xC;
 * bytes at 0x8 remain opaque. The float-key tree insertion helper reads the
 * same +0xC word through sdfDrawTag_t. DDS1/2 game/code_002EEA18/003478C0.c. */
typedef struct sdfDrawTag_t {
    struct sdfDrawTag_t *next; /* 0x00 */
    s32 head;       /* 0x04: first DMA packet address. */
    u8 tail[4];                  /* 0x08 */
    f32 z;                    /* 0x0C */
} sdfDrawTag_t;

typedef struct sdfSortOtNode_tag {
    struct sdfSortOtNode_tag *right; /* 0x00 */
    struct sdfSortOtNode_tag *left;  /* 0x04 */
    struct sdfDrawTag_t *tag;  /* 0x08 */
    s32 balance;                  /* 0x0C */
} sdfSortOtNode_tag;

/* A key-sorted draw-surface pool: a draw tag whose append and prepend
 * callbacks release a node into the pool and update its free list. */
typedef struct sdfGrOt_t {
    void *next;                                                /* 0x00: the pool entry's link */
    sdfDrawTag_t *tagHead;                                     /* 0x04: first node in the emitted chain */
    sdfDrawTag_t *tagTail;                                     /* 0x08: last node in the emitted chain */
    sdfDrawTag_t *noSortTag;                                   /* 0x0C: the free list */
    void (*regFunc)(struct sdfGrOt_t *, sdfDrawTag_t *);       /* 0x10 */
    void (*callbackFunc)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    sdfSortOtNode_tag *data[1];                                /* 0x18: first root; allocation continues */
} sdfGrOt_t;


typedef struct _reent _reent;

typedef __builtin_va_list va_list;

typedef struct __sbuf {
    u8 *_base;
    s32 _size;
} __sbuf;

typedef struct __sFILE {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
    s16 _file;
    __sbuf _bf;
    s32 _lbfsize;
    void *_cookie;
    s32 (*_read)(void *cookie, char *buf, s32 n);
    s32 (*_write)(void *cookie, const char *buf, s32 n);
    long (*_seek)(void *cookie, s32 offset, s32 whence);
    s32 (*_close)(void *cookie);
    __sbuf _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    __sbuf _lb;
    s32 _blksize;
    s32 _offset;
    _reent *_data;
} FILE;

typedef struct __siov {
    void *iov_base;
    u32 iov_len;
} __siov;

typedef struct __suio {
    __siov *uio_iov;
    s32 uio_iovcnt;
    s32 uio_resid;
} __suio;

typedef struct _Bigint {
    struct _Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} _Bigint;

typedef struct _glue {
    struct _glue *next;
    s32 niobs;
    FILE *iobs;
} _glue;

/* libgcc frame-dwarf2.c's object list node. __register_frame_info stores the
 * FDE base and clears the rest; __register_frame allocates one of these. */
typedef struct object {
    void *pc_begin;
    void *pc_end;
    void *fde_begin;
    void *fde_array;
    void *count;
    struct object *next;
} object;

/* libgcc's fde_vector, as add_fdes fills it. */
typedef struct fde_vector {
    u32 **array;
    s32 count;
} fde_vector;

typedef struct _atexit {
    struct _atexit *_next;
    s32 _ind;
    void (*_fns[32])(void);
} _atexit;

struct _reent {
    s32 _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    s32 _inc;
    char _emergency[25];
    s32 _current_category;
    const char *_current_locale;
    s32 __sdidinit;
    void (*__cleanup)(struct _reent *r);
    _Bigint *_result;
    s32 _result_k;
    _Bigint *_p5s;
    _Bigint **_freelist;
    u8 _unused[0xF8];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};

extern _reent *_impure_ptr __attribute__((section(".data")));
extern void __sinit(_reent *r);
extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

typedef struct fp_number_type {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

extern s32 _vfprintf_r(_reent *r, FILE *fp, const char *fmt, va_list ap);

typedef union f32_bits {
    f32 f;
    u32 u;
} f32_bits;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

typedef struct mallocChunkPrefix_t {
    u32 previousSize;
    u32 size;
    struct mallocChunkPrefix_t *forward;
    struct mallocChunkPrefix_t *backward;
} mallocChunkPrefix_t;

typedef struct mallocBinLinks_t {
    mallocChunkPrefix_t *forward;
    mallocChunkPrefix_t *backward;
} mallocBinLinks_t;

typedef struct mallocArenaPrefix_t {
    u32 opaque00;
    u32 bitmap;
    mallocChunkPrefix_t *top;
    u32 opaque0C;
    mallocBinLinks_t bins[127];
} mallocArenaPrefix_t;

typedef struct mallocInfo_t {
    s32 arena;
    u8 opaque04[0x24];
} mallocInfo_t;

typedef struct mallocFenceposts_t {
    u32 previousSize;
    u32 firstSize;
    u32 secondSize;
} mallocFenceposts_t;

typedef s32 (*qsortCompare_t)(const void *, const void *);

static inline char *qsortMedian3(char *a, char *b, char *c, qsortCompare_t compare)
{
    return compare(a, b) < 0 ?
        (compare(b, c) < 0 ? b : (compare(a, c) < 0 ? c : a)) :
        (compare(b, c) > 0 ? b : (compare(a, c) < 0 ? a : c));
}

static inline void qsortSwapRange(char *first, char *second, s32 bytes, s32 swapType)
{
    if (swapType < 2) {
        s64 remaining = (u32)bytes / sizeof(s64);
        s64 *left = (s64 *)first;
        s64 *right = (s64 *)second;
        do {
            s64 temporary = *left;
            *left++ = *right;
            *right++ = temporary;
        } while (--remaining > 0);
    } else {
        s64 remaining = (u32)bytes;
        char *left = first;
        char *right = second;
        do {
            char temporary = *left;
            *left++ = *right;
            *right++ = temporary;
        } while (--remaining > 0);
    }
}

#define QSORT_SWAP_ELEMENT(first, second) { \
    if (swapType == 0) { \
        s64 temporary = *(s64 *)(first); \
        *(s64 *)(first) = *(s64 *)(second); \
        *(s64 *)(second) = temporary; \
    } else { \
        qsortSwapRange(first, second, size, swapType); \
    } \
}

#define STRTOL_MAX 0x7fffffffffffffffUL

#define STRTOL_MIN 0x8000000000000000UL

s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern char *_dtoa_r(_reent *r, f64 value, s32 mode, s32 digits,
                     s32 *decimalPoint, s32 *sign, char **end);

char *cvt(_reent *r, f64 value, s32 precision, s32 flags, char *sign,
          s32 *decimalPoint, s32 format, s32 *length);

s32 exponent(char *output, s32 value, s32 marker);

/* Little-endian IEEE binary64 word view used by the numeric helpers. */
typedef union libcDoubleBits_t {
    f64 value;
    u32 word[2];
    u64 bits;
} libcDoubleBits_t;

extern const char dtoaInfinityText[], dtoaNanText[];
extern const char printfNanText[8] __attribute__((section(".rodata")));
extern const char printfHexLower[24];
extern const char printfNullText[8] __attribute__((section(".rodata")));
extern const char printfHexUpper[24];
extern const char printfBadBaseMsg[32];
extern const char printfZeroDigit[8] __attribute__((section(".rodata")));
extern const char printfDecimalPoint[16];
extern const char dtoaInfinityText[16];
void __sinit(_reent *r);

s32 vfprintf(FILE *fp, const char *fmt, va_list ap)
{
    /* Keep attachment and initialization as one phase, as in __swsetup. */
    do {
        if (fp->_data == NULL) {
            fp->_data = _impure_ptr;
        }
        if (fp->_data->__sdidinit == 0) {
            __sinit(fp->_data);
        }
    } while (0);
    return _vfprintf_r(fp->_data, fp, fmt, ap);
}

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", vfprintfAbcdefTable);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", vfprintfAbcdefTable2);

INCLUDE_ASM(const s32, "sdk/libc/vfprintf", vfprintfAbcdef);

char *cvt(_reent *r, f64 value, s32 precision, s32 flags, char *sign,
          s32 *decimalPoint, s32 format, s32 *length)
{
    union {
        f64 value;
        u64 bits;
    } input;
    s32 mode = 3;
    s32 dtoaSign;
    char *end;
    char *digits;
    char *limit;

    if (format != 'f') {
        if (format == 'e' || format == 'E') {
            precision++;
        }
        mode = 2;
    }
    input.value = value;
    if ((s32)(input.bits >> 32) < 0) {
        value = 0.0 - value;
        *sign = '-';
    } else {
        *sign = 0;
    }
    digits = _dtoa_r(r, value, mode, precision, decimalPoint, &dtoaSign, &end);
    if ((format != 'g' && format != 'G') || (flags & 1)) {
        limit = digits + precision;
        if (format == 'f') {
            if (*digits == '0' && value != 0.0) {
                *decimalPoint = 1 - precision;
            }
            limit += *decimalPoint;
        }
        if (value == 0.0) {
            end = limit;
        }
        while (end < limit) {
            *end++ = '0';
        }
    }
    *length = end - digits;
    return digits;
}

s32 exponent(char *output, s32 value, s32 marker)
{
    /* The reverse-digit cursor ends at stack + 0x134 in the 0x140-byte frame. */
    char digits[0x134];
    char *cursor = output;
    char *digit;

    *cursor++ = marker;
    if (value < 0) {
        value = -value;
        *cursor++ = '-';
    } else {
        *cursor++ = '+';
    }
    digit = digits + sizeof(digits);
    if (value > 9) {
        while (value > 9) {
            *--digit = value % 10 + '0';
            value /= 10;
        }
        *--digit = value + '0';
        while (digit < digits + sizeof(digits)) {
            *cursor++ = *digit++;
        }
    } else {
        *cursor++ = '0';
        *cursor++ = value + '0';
    }
    return cursor - output;
}

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfNanText);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfHexLower);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfNullText);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfHexUpper);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfBadBaseMsg);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfZeroDigit);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", printfDecimalPoint);

INCLUDE_RODATA(const s32, "sdk/libc/vfprintf", jtbl_00468750);

