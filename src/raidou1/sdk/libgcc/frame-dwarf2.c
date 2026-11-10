#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libgcc.c */
#include "common.h"

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

extern void SleepThread(void);

extern s32 sceSifLoadModule(const char *, s32, const char *);

extern char *strcpy(char *, const char *);

extern char *strcat(char *, const char *);

extern void sceMcInit(void);

extern void sdfGrJoinDrawTag(void *aFore, void *aBack);

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

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

extern s32 WaitSema(s32);

extern void FlushCache(s32);

extern s32 SignalSema(s32);

extern s32 sceSifDmaStat(s32);

extern s32 GetThreadId(void);

extern void *memcpy(void *, const void *, size_t);

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent);

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent);

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **stack, s32 stack_depth, sdfSortOtNode_tag *node,
                   sdfSortOtNode_tag **root_p);

void sdfSortOtAvlRegist(sdfSortOtNode_tag **root_p, sdfDrawTag_t *tag);

void sdfGrSortOtReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

void sdfGrSortOtFixup(void *tag, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p);

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p);

void sdfGrSortOtSetup(sdfGrOt_t *aOt);

void sdfGrSortOtCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData);

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

extern _reent *_impure_ptr __attribute__((section(".data")));

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern s32 _setlocale_r(_reent *r, s32 category, const char *locale);

extern void *_localeconv_r(_reent *r);

extern void _cleanup_r(_reent *r);

extern s32 fflush(void *fp);

extern s32 _fwalk(_reent *r, s32 (*func)(void *));

extern s32 libc_2DC670(void);

extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);

extern long strtol(const char *s, char **end, s32 base);

extern s32 raise(s32 sig);

extern void _exit(s32 code) __attribute__((noreturn));

extern u8 localeconvData[] __attribute__((section(".data")));

extern s32 libcErrno __attribute__((section(".data")));

extern s32 mallocLockCount __attribute__((section(".data")));

extern s32 mallocLockOwner __attribute__((section(".data")));

extern s32 mallocSema __attribute__((section(".data")));

extern s32 _init_signal_r(_reent *r);

extern void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data);

extern _glue *__sfmoreglue(_reent *r, s32 n);

extern s32 __sigtramp_r(_reent *r, s32 sig);

extern void *__deregister_frame_info(void *fn);

extern void free(void *ptr);

extern void *malloc(size_t size);

extern void __malloc_lock(_reent *r);

extern void __malloc_unlock(_reent *r);

extern void *_malloc_r(_reent *r, u32 size);

extern void _free_r(_reent *r, void *ptr);

extern s32 SignalSema(s32 sema);

extern void *sbrk(s32 incr);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern void __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, void *buf);

extern s32 kill(s32 pid, s32 sig);

extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

extern long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence);

extern s32 __sfvwrite(FILE *fp, __suio *uio);

extern s32 __sread(void *cookie, char *buf, s32 n);

extern s32 __swrite(void *cookie, const char *buf, s32 n);

extern long __sseek(void *cookie, s32 offset, s32 whence);

extern s32 __sclose(void *cookie);

extern s32 GetThreadId(void);

extern s32 WaitSema(s32 sema);

extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern void *memset(void *a, s32 b, size_t c);

extern void *_calloc_r(_reent *r, u32 n, u32 size);

extern const double pow10Table[];

extern void abort(void);

extern void __do_global_ctors(void);

extern object ctorFrameObject;

extern s32 atexit(void (*fn)(void));

extern void (**dtorListCursor)(void) __attribute__((section(".data")));

extern s32 dtorFrameDeregistered __attribute__((section(".data")));

extern s32 frameObjectSema __attribute__((section(".data")));

extern u8 __EH_FRAME_BEGIN__[] __attribute__((section(".data")));

extern void (*__CTOR_LIST__[])(void) __attribute__((section(".data")));

extern s32 mainCtorsDone __attribute__((section(".data")));

extern object *frameObjectList __attribute__((section(".data")));

typedef struct fp_number_type {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

extern void sdfUnpackFloat(const void *x, fp_number_type *u);

extern f32 sdfPackFloat(const fp_number_type *u);

extern f64 __make_dp(s32 kind, s32 sign, s32 exp, s64 mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

extern s32 _vfprintf_r(_reent *r, FILE *fp, const char *fmt, va_list ap);

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

extern f64 __pack_df(const fp_number_type_df *u);

extern void sdfUnpackDouble(const double *x, fp_number_type_df *u);

extern const fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b, fp_number_type_df *r);

extern s32 __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);

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

void __do_global_dtors(void);

void __do_global_ctors(void);

void __main(void);

f64 fptodp(f32 x);

const fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b,
                                     fp_number_type_df *result);

f64 dpadd(double a, double b);

f64 dpsub(double a, double b);

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

f64 dpmul(f64 a, f64 b);

s32 dpcmp(double a, double b);

f64 litodp(s32 n);

s32 dptoli(f64 value);

f64 __make_dp(s32 kind, s32 sign, s32 exp, s64 mant);

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant);

u32 dptoul(f64 value);

u8 *libc_2D1E08(u8 *p, u32 *out);

u8 *libc_2D1E50(u8 *p, s32 *out);

static inline s32 fde_compare(u32 *left, u32 *right)
{
    return (s32)(left[2] - right[2]);
}

void libc_2D1EA8(fde_vector *left, const fde_vector *right);

void __register_frame_info(void *fn, void *entry);

void __register_frame(void *fn);

s32 __register_frame_info_table(void *fn, void *entry);

s32 __register_frame_table(void *fn);

void *__deregister_frame_info(void *fn);

void __deregister_frame(void *fn);

s32 libc_2D2728(u32 *p);

void libc_2D2768(u32 *p, fde_vector *list, u32 *pmin, u32 *pmax);

void *libc_2D27F8(u32 *p, u32 addr);

typedef struct {
    fde_vector linear;
    fde_vector erratic;
} fde_accumulator;

extern u32 **libc_2D1FA0(fde_accumulator *vectors, u32 count);

/* Preserve the allocation boundary; see levers/cross-block-locals.md. */
static inline s32 start_fde_sort(fde_accumulator *vectors, u32 count)
{
    vectors->linear.array = count ? malloc(count * sizeof(u32 *)) : NULL;
    vectors->erratic.array = vectors->linear.array ? malloc(count * sizeof(u32 *)) : NULL;
    vectors->linear.count = 0;
    vectors->erratic.count = 0;
    return vectors->linear.array != NULL;
}

void libc_2D2850(object *entry);

u32 *libc_2D29B8(u32 pc);

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

extern const char libgccCieAugEmpty[];

extern const char libgccCieAugEh[];

extern s32 strcmp(const char *left, const char *right);

extern u32 strlen(const char *text);

u8 *libc_2D2B60(u32 *frame, cieInfo_t *info);

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

u8 *libc_2D2C70(u8 *p, frameState_t *state);

typedef struct frameStateInternal {
    frameState_t state;
    struct frameStateInternal *savedState;
} frameStateInternal_t;

SIZE_CHECK(frameStateInternal_size_must_be_0x480, frameStateInternal_t, 0x480);

typedef union __attribute__((packed)) {
    u16 half;
    u32 word;
} cfiEncodedValue_t;

extern const char libgccCieAugEmpty[8] __attribute__((section(".rodata")));

extern const char libgccCieAugEh[8] __attribute__((section(".rodata")));

u8 *libc_2D2D90(u8 *p, frameStateInternal_t *state, cieInfo_t *info, u32 *location);

frameState_t *libc_2D31E0(u32 pc, frameState_t *output);

void sdfUnpackFloat(const void *input, fp_number_type *number);

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant);

f64 __pack_df(const fp_number_type_df *u);

void sdfUnpackDouble(const double *x, fp_number_type_df *u);

s32 __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);

static inline s32 sfIsZero(const fp_number_type *number) {
    return number->class == 2;
}

f32 sdfPackFloat(const fp_number_type *number);
/* End of declarations from src/smt3/sdk/libgcc.c */

u8 *libc_2D1E50(u8 *p, s32 *out) {
    s32 shift = 0;
    u32 v = 0;
    u32 b;

    do {
        b = *p++;
        v |= (b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if ((u32)shift < 32 && (b & 0x40)) {
        v |= -1 << shift;
    }
    *out = v;
    return p;
}

void libc_2D1EA8(fde_vector *left, const fde_vector *right)
{
    u32 leftIndex;
    u32 rightIndex;
    u32 *entry;

    /* Preserve owner-based indexing; see levers/cross-block-locals.md. */
    rightIndex = right->count;
    if (rightIndex > 0) {
        leftIndex = left->count;
        do {
            rightIndex--;
            entry = right->array[rightIndex];
            while (leftIndex > 0 && fde_compare(left->array[leftIndex - 1], entry) > 0) {
                left->array[leftIndex + rightIndex] = left->array[leftIndex - 1];
                leftIndex--;
            }
            left->array[leftIndex + rightIndex] = entry;
        } while (rightIndex > 0);
        left->count += right->count;
    }
}

INCLUDE_ASM(const s32, "sdk/libgcc/frame-dwarf2", func_00386268);

void __register_frame_info(void *fn, void *entry) {
    object *e = entry;

    e->fde_begin = fn;
    e->pc_end = NULL;
    e->pc_begin = NULL;
    e->fde_array = NULL;
    e->count = 0;
    WaitSema(frameObjectSema);
    e->next = frameObjectList;
    frameObjectList = e;
    SignalSema(frameObjectSema);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}

void __register_frame(void *fn) {
    __register_frame_info(fn, malloc(0x18));
}

s32 __register_frame_info_table(void *fn, void *entry) {
    object *e = entry;

    e->fde_begin = fn;
    e->fde_array = fn;
    e->pc_end = NULL;
    e->pc_begin = NULL;
    e->count = 0;
    WaitSema(frameObjectSema);
    e->next = frameObjectList;
    frameObjectList = e;
    SignalSema(frameObjectSema);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}

s32 __register_frame_table(void *fn) {
    return __register_frame_info_table(fn, malloc(0x18));
}

void *__deregister_frame_info(void *fn) {
    object **link;
    object *entry;

    WaitSema(frameObjectSema);
    for (link = &frameObjectList; *link != NULL; link = &(*link)->next) {
        entry = *link;
        if (entry->fde_begin == fn) {
            *link = entry->next;
            if (entry->fde_array != NULL && entry->fde_array != fn) {
                free(entry->fde_array);
            }
            SignalSema(frameObjectSema);
            return entry;
        }
    }
    SignalSema(frameObjectSema);
    abort();
}

void __deregister_frame(void *fn) {
    free(__deregister_frame_info(fn));
}

s32 libc_2D2728(u32 *p) {
    s32 n = 0;

    while (p[0] != 0) {
        if (p[1] != 0 && p[2] != 0) {
            n++;
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    return n;
}

void libc_2D2768(u32 *p, fde_vector *list, u32 *pmin, u32 *pmax) {
    u32 lo = *pmin;
    u32 hi = *pmax;

    while (p[0] != 0) {
        if (p[1] != 0) {
            u32 a = p[2];

            if (a != 0) {
                if (list->array != NULL) {
                    list->array[list->count++] = p;
                }
                if (a < lo) {
                    lo = a;
                }
                if (hi < a + p[3]) {
                    hi = a + p[3];
                }
            }
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    *pmin = lo;
    *pmax = hi;
}

void *libc_2D27F8(u32 *p, u32 addr) {
    while (p[0] != 0) {
        if (p[1] != 0 && p[2] != 0 && addr - p[2] < p[3]) {
            return p;
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    return NULL;
}

void libc_2D2850(object *entry)
{
    fde_accumulator vectors;
    u32 minimum;
    u32 maximum;
    u32 count;
    u32 **sorted;

    if (entry->pc_begin != NULL) {
        count = entry->count;
    } else if (entry->fde_array != NULL) {
        u32 **table = entry->fde_array;
        for (count = 0; *table != NULL; table++)
            count += libc_2D2728(*table);
    } else {
        count = libc_2D2728(entry->fde_begin);
    }
    entry->count = count;
    if (!start_fde_sort(&vectors, count) && entry->pc_begin != NULL)
        return;
    minimum = 0xffffffff;
    maximum = 0;
    if (entry->fde_array != NULL) {
        u32 **table = entry->fde_array;
        for (; *table != NULL; table++)
            libc_2D2768(*table, &vectors.linear, &minimum, &maximum);
    } else {
        libc_2D2768(entry->fde_begin, &vectors.linear, &minimum, &maximum);
    }
    sorted = libc_2D1FA0(&vectors, count);
    if (sorted != NULL)
        entry->fde_array = sorted;
    entry->pc_begin = EE_ADDR_AS(void *, minimum);
    entry->pc_end = EE_ADDR_AS(void *, maximum);
}

u32 *libc_2D29B8(u32 pc)
{
    object *entry;
    u32 low;
    u32 high;
    u32 *found;

    WaitSema(frameObjectSema);
    for (entry = frameObjectList; entry != NULL; entry = entry->next) {
        if (entry->pc_begin == NULL)
            libc_2D2850(entry);
        if (pc >= EE_ADDR(entry->pc_begin) && pc < EE_ADDR(entry->pc_end))
            break;
    }
    if (entry == NULL) {
        SignalSema(frameObjectSema);
        return NULL;
    }
    if (entry->fde_array == NULL || entry->fde_array == entry->fde_begin)
        libc_2D2850(entry);
    if (entry->fde_array != NULL && entry->fde_array != entry->fde_begin) {
        SignalSema(frameObjectSema);
        for (low = 0, high = entry->count; low < high;) {
            u32 index = (low + high) / 2;
            u32 *frame = ((u32 **)entry->fde_array)[index];
            if (pc < frame[2])
                high = index;
            else if (pc >= frame[2] + frame[3])
                low = index + 1;
            else
                return frame;
        }
    } else {
        if (entry->fde_array != NULL) {
            u32 **table = entry->fde_array;
            do {
                found = libc_2D27F8(*table, pc);
            } while (found == NULL && *++table != NULL);
        } else {
            found = libc_2D27F8(entry->fde_begin, pc);
        }
        SignalSema(frameObjectSema);
        return found;
    }
    return NULL;
}

u8 *libc_2D2B60(u32 *frame, cieInfo_t *info)
{
    u8 *p;
    u32 length;

    info->augmentation = (char *)frame - frame[1] + 13;
    if (strcmp(info->augmentation, libgccCieAugEmpty) != 0 &&
        strcmp(info->augmentation, libgccCieAugEh) != 0 &&
        info->augmentation[0] != 'z')
        return NULL;
    length = strlen(info->augmentation);
    p = (u8 *)info->augmentation + length + 1;
    if (strcmp(info->augmentation, libgccCieAugEh) == 0) {
        info->ehPointer = ((const cieEncodedPointer_t *)p)->value;
        p += sizeof(void *);
    } else {
        info->ehPointer = NULL;
    }
    p = libc_2D1E08(p, &info->codeAlignment);
    p = libc_2D1E50(p, &info->dataAlignment);
    info->returnRegister = *p++;
    if (info->augmentation[0] == 'z') {
        u32 payloadSize;
        p = libc_2D1E08(p, &payloadSize);
        p += payloadSize;
    }
    return p;
}

u8 *libc_2D2C70(u8 *p, frameState_t *state)
{
    u32 opcode = *p++;
    union {
        s32 signedValue;
        u32 unsignedValue;
    } value;

    switch (opcode) {
    case 0x50 ... 0x6f:
        state->cfaRegister = opcode - 0x50;
        break;
    case 0x90:
        p = libc_2D1E50(p, &value.signedValue);
        state->cfaRegister = value.signedValue;
        /* The native regx case falls through and replaces this register. */
    case 0x70 ... 0x8f:
        state->cfaRegister = opcode - 0x70;
        p = libc_2D1E50(p, &value.signedValue);
        state->baseOffset = value.signedValue;
        break;
    case 0x92:
        p = libc_2D1E50(p, &value.signedValue);
        state->cfaRegister = value.signedValue;
        p = libc_2D1E50(p, &value.signedValue);
        state->baseOffset = value.signedValue;
        break;
    case 0x06:
        state->indirect = 1;
        break;
    case 0x23:
        p = libc_2D1E08(p, &value.unsignedValue);
        state->cfaOffset = value.signedValue;
        break;
    default:
        abort();
    }
    return p;
}

INCLUDE_RODATA(const s32, "sdk/libgcc/frame-dwarf2", D_004682A0);

INCLUDE_RODATA(const s32, "sdk/libgcc/frame-dwarf2", D_004682A8);

u8 *libc_2D2D90(u8 *p, frameStateInternal_t *state, cieInfo_t *info, u32 *location)
{
    union {
        s32 signedValue;
        u32 unsignedValue;
    } offset;
    u32 reg;
    u32 sourceReg;
    u32 opcode = *p++;

    if (opcode & 0x40) {
        *location += (opcode & 0x3f) * info->codeAlignment;
    } else if (opcode & 0x80) {
        reg = opcode & 0x3f;
        p = libc_2D1E08(p, &offset.unsignedValue);
        if (reg != state->state.cfaRegister) {
            offset.signedValue *= info->dataAlignment;
            state->state.saved[reg] = 1;
            state->state.registerOrOffset[reg] = offset.signedValue;
        }
    } else if (opcode & 0xc0) {
        reg = opcode & 0x3f;
        state->state.saved[reg] = 0;
    } else switch (opcode) {
    case 0:
    case 7:
    case 8:
        break;
    case 1:
        *location = ((const cfiEncodedValue_t *)p)->word;
        p += 4;
        break;
    case 2:
        *location += *p++;
        break;
    case 3:
        *location += ((const cfiEncodedValue_t *)p)->half;
        p += 2;
        break;
    case 4:
        *location += ((const cfiEncodedValue_t *)p)->word;
        p += 4;
        break;
    case 5:
        p = libc_2D1E08(p, &reg);
        p = libc_2D1E08(p, &offset.unsignedValue);
        if (reg != state->state.cfaRegister) {
            offset.signedValue *= info->dataAlignment;
            state->state.saved[reg] = 1;
            state->state.registerOrOffset[reg] = offset.signedValue;
        }
        break;
    case 6:
        p = libc_2D1E08(p, &reg);
        state->state.saved[reg] = 0;
        break;
    case 9:
        p = libc_2D1E08(p, &reg);
        p = libc_2D1E08(p, &sourceReg);
        state->state.saved[reg] = 2;
        state->state.registerOrOffset[reg] = sourceReg;
        break;
    case 0x0c:
        p = libc_2D1E08(p, &reg);
        p = libc_2D1E08(p, &offset.unsignedValue);
        state->state.cfaRegister = reg;
        state->state.cfaOffset = offset.signedValue;
        break;
    case 0x0d:
        p = libc_2D1E08(p, &reg);
        state->state.cfaRegister = reg;
        break;
    case 0x0e:
        p = libc_2D1E08(p, &offset.unsignedValue);
        state->state.cfaOffset = offset.signedValue;
        break;
    case 0x0f: {
        u8 *end;
        state->state.cfaRegister = 0;
        state->state.cfaOffset = 0;
        state->state.baseOffset = 0;
        state->state.indirect = 0;
        p = libc_2D1E08(p, &offset.unsignedValue);
        end = p + offset.unsignedValue;
        while (p < end)
            p = libc_2D2C70(p, &state->state);
        break;
    }
    case 0x0a: {
        frameStateInternal_t *saved = malloc(sizeof(*saved));
        *saved = *state;
        state->savedState = saved;
        break;
    }
    case 0x0b: {
        frameStateInternal_t *saved = state->savedState;
        *state = *saved;
        free(saved);
        break;
    }
    case 0x2d:
        for (reg = 16; reg < 32; reg++) {
            state->state.saved[reg] = 1;
            state->state.registerOrOffset[reg] = (reg - 16) * sizeof(void *);
        }
        break;
    case 0x2e:
        p = libc_2D1E08(p, &offset.unsignedValue);
        state->state.argumentSize = offset.signedValue;
        break;
    case 0x2f:
        p = libc_2D1E08(p, &reg);
        p = libc_2D1E08(p, &offset.unsignedValue);
        offset.signedValue *= info->dataAlignment;
        state->state.saved[reg] = 1;
        state->state.registerOrOffset[reg] = (s32)(0u - (u32)offset.signedValue);
        break;
    default:
        abort();
    }
    return p;
}

frameState_t *libc_2D31E0(u32 pc, frameState_t *output)
{
    cieInfo_t info;
    frameStateInternal_t state;
    u32 *frame;
    u8 *p;
    u8 *end;
    u32 payloadSize;
    u32 location;
    u32 *cie;

    frame = libc_2D29B8(pc);
    if (frame == NULL)
        return NULL;
    p = libc_2D2B60(frame, &info);
    if (p == NULL)
        return NULL;
    memset(&state, 0, sizeof(state));
    state.state.returnColumn = info.returnRegister;
    state.state.ehPointer = info.ehPointer;
    cie = (u32 *)((u8 *)frame - frame[1] + 4);
    end = (u8 *)cie + cie[0] + 4;
    while (p < end)
        p = libc_2D2D90(p, &state, &info, NULL);
    p = (u8 *)frame + 16;
    if (info.augmentation[0] == 'z') {
        p = libc_2D1E08(p, &payloadSize);
        p += payloadSize;
    }
    end = (u8 *)frame + frame[0] + 4;
    location = frame[2];
    while (p < end && pc >= location)
        p = libc_2D2D90(p, &state, &info, &location);
    *output = state.state;
    return output;
}
