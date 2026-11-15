#include "common.h"
#include "ps2asm.h"
#include "sdk_libc.h"
#include "sdk_libgcc.h"

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
 * same +0xC word through sdfDrawTag_t. */
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
    u32 count;
    struct object *next;
} object;

/* libgcc's fde_vector, as add_fdes fills it. */
typedef struct fde_vector {
    u32 **array;
    u32 count;
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


typedef struct fp_number_type {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

extern void sdfUnpackFloat(const void *x, fp_number_type *u);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

typedef union f32_bits {
    f32 f;
    u32 u;
    struct {
        u32 fraction : 23;
        u32 exponent : 8;
        u32 sign : 1;
    } fields;
} f32_bits;

typedef union sdfDoubleBits_t {
    double value;
    struct {
        u64 fraction : 52;
        u32 exponent : 11;
        u32 sign : 1;
    } fields;
} sdfDoubleBits_t;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u64 fraction;
} fp_number_type_df;

extern const fp_number_type_df libgccDfInvalidResult;

/* fp-bit tags: NaN classes 0/1, zero 2, normal 3, infinity 4. */
static inline s32 dfIsNaN(const fp_number_type_df *number) {
    return (u32)number->class < 2;
}

static inline s32 dfIsZero(const fp_number_type_df *number) {
    return number->class == 2;
}

static inline s32 dfIsInfinity(const fp_number_type_df *number) {
    return number->class == 4;
}

f64 fptodp(f32 x);

static inline const fp_number_type_df *dfMultiplyParts(fp_number_type_df *left,
                                                       fp_number_type_df *right,
                                                       fp_number_type_df *result) {
    u64 leftLow;
    u64 leftHigh;
    u64 rightLow;
    u64 rightHigh;
    u64 lowProduct;
    u64 crossLeft;
    u64 crossRight;
    u64 cross;
    u64 low;
    u64 high;
    u64 highProduct;

    if (dfIsNaN(left)) {
        left->sign = left->sign != right->sign;
        return left;
    }
    if (dfIsNaN(right)) {
        right->sign = left->sign != right->sign;
        return right;
    }
    if (dfIsInfinity(left)) {
        if (dfIsZero(right)) {
            return &libgccDfInvalidResult;
        }
        left->sign = left->sign != right->sign;
        return left;
    }
    if (dfIsInfinity(right)) {
        if (dfIsZero(left)) {
            return &libgccDfInvalidResult;
        }
        right->sign = left->sign != right->sign;
        return right;
    }
    if (dfIsZero(left)) {
        left->sign = left->sign != right->sign;
        return left;
    }
    if (dfIsZero(right)) {
        right->sign = left->sign != right->sign;
        return right;
    }

    leftLow = (u64)left->fraction & 0xFFFFFFFFULL;
    leftHigh = (u64)left->fraction >> 32;
    rightLow = (u64)right->fraction & 0xFFFFFFFFULL;
    rightHigh = (u64)right->fraction >> 32;
    lowProduct = rightLow * leftLow;
    crossLeft = rightHigh * leftLow;
    crossRight = rightLow * leftHigh;
    highProduct = rightHigh * leftHigh;
    {
        u64 upperAccum = 0;
        u64 lowerAccum;
        u64 shiftedCross;
        cross = crossLeft + crossRight;
        if (cross < crossLeft) {
            upperAccum += (u64)1 << 32;
        }
        shiftedCross = (cross << 32) & 0xFFFFFFFF00000000ULL;
        lowerAccum = lowProduct + shiftedCross;
        if (lowerAccum < lowProduct) {
            upperAccum++;
        }
        upperAccum += ((cross >> 32) & 0xFFFFFFFFULL) + highProduct;
        high = upperAccum;
        low = lowerAccum;
    }

    result->normal_exp = left->normal_exp + right->normal_exp;
    result->sign = left->sign != right->sign;
    result->normal_exp += 4;
    while (high > (~(u64)0 >> 3)) {
        result->normal_exp++;
        if (high & 1) {
            low >>= 1;
            low |= (u64)1 << 63;
        }
        high >>= 1;
    }
    while (high <= (~(u64)0 >> 4)) {
        result->normal_exp--;
        high <<= 1;
        if (low & ((u64)1 << 63)) {
            high |= 1;
        }
        low <<= 1;
    }

    result->class = 3;
    result->fraction = high;
    return result;
}


static inline s32 fde_compare(u32 *left, u32 *right)
{
    return (s32)(left[2] - right[2]);
}

typedef struct {
    fde_vector linear;
    fde_vector erratic;
} fde_accumulator;

/* Preserve the allocation boundary; see levers/cross-block-locals.md. */
static inline s32 start_fde_sort(fde_accumulator *vectors, u32 count)
{
    vectors->linear.array = count ? malloc(count * sizeof(u32 *)) : NULL;
    vectors->erratic.array = vectors->linear.array ? malloc(count * sizeof(u32 *)) : NULL;
    vectors->linear.count = 0;
    vectors->erratic.count = 0;
    return vectors->linear.array != NULL;
}

typedef struct {
    char *augmentation;
    void *ehPointer;
    u32 codeAlignment;
    s32 dataAlignment;
    u32 returnRegister;
} cieInfo_t;

/* The encoded EH pointer follows a variable-length string. */
typedef struct __attribute__((packed)) {
    void *value;
} cieEncodedPointer_t;

typedef struct {
    void *cfa;
    void *ehPointer;
    s64 cfaOffset;
    s64 baseOffset;
    s64 argumentSize;
    s64 registerOrOffset[123];
    u16 cfaRegister;
    u16 returnColumn;
    u8 saved[123];
    u8 indirect;
} frameState_t;

SIZE_CHECK(frameState_size_must_be_0x478, frameState_t, 0x478);

typedef struct frameStateInternal {
    frameState_t state;
    struct frameStateInternal *savedState;
} frameStateInternal_t;

SIZE_CHECK(frameStateInternal_size_must_be_0x480, frameStateInternal_t, 0x480);

typedef union __attribute__((packed)) {
    u16 half;
    u32 word;
} cfiEncodedValue_t;

void sdfUnpackFloat(const void *input, fp_number_type *number);

static inline s32 sfIsZero(const fp_number_type *number) {
    return number->class == 2;
}

f64 fptodp(f32 x) {
    fp_number_type u;

    sdfUnpackFloat(&x, &u);
    return __make_dp(u.class, u.sign, u.normal_exp, (s64)u.fraction << 30);
}
