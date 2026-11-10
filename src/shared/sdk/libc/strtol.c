#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libc.c */
#include "sdk_libc.h"

#include "common.h"

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

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern char *_setlocale_r(_reent *r, s32 category, const char *locale);

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

extern void __sinit(_reent *r);

extern void __smakebuf(FILE *fp);

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

extern void *_sbrk_r(_reent *r, s32 incr);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern void __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, struct stat *buf);

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
} f32_bits;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

extern f64 __pack_df(const fp_number_type_df *u);

extern void sdfUnpackDouble(const double *x, fp_number_type_df *u);

extern fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b, fp_number_type_df *r);

extern long __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);

void abort(void);

s32 atexit(void (*fn)(void));

s32 atoi(const char *s);

void *malloc(size_t size);

void free(void *ptr);

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

extern mallocArenaPrefix_t __malloc_av_;

extern mallocInfo_t __malloc_current_mallinfo;

extern void *__malloc_sbrk_base __attribute__((section(".data")));

extern u64 __malloc_trim_threshold __attribute__((section(".data")));

extern u64 __malloc_top_pad __attribute__((section(".data")));

s32 _malloc_trim_r(_reent *r, u32 pad);

typedef struct mallocFenceposts_t {
    u32 previousSize;
    u32 firstSize;
    u32 secondSize;
} mallocFenceposts_t;

extern u64 __malloc_max_sbrked_mem __attribute__((section(".data")));

extern u64 __malloc_max_total_mem __attribute__((section(".data")));

void __malloc_lock(_reent *r);

void __malloc_unlock(_reent *r);

s32 _printf_r(_reent *r, const char *fmt, ...);

s32 printf(const char *fmt, ...);

extern u32 strlen(const char *s);

/* The newline owns a padded 16-byte read-only block in retail. */
extern const char putsNewline[];

s32 _puts_r(_reent *r, const char *s);

s32 puts(const char *s);

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

void qsort(void *array, u32 count, u32 size, qsortCompare_t compare);

void *_sbrk_r(_reent *r, s32 incr);

s32 _init_signal_r(_reent *r);

void *_signal_r(_reent *r, s32 sig, void *handler);

extern s32 _getpid_r(_reent *r);

extern s32 _kill_r(_reent *r, s32 pid, s32 sig);

s32 _raise_r(_reent *r, s32 sig);

s32 __sigtramp_r(_reent *r, s32 sig);

s32 raise(s32 sig);

void *signal(s32 sig, void *handler);

s32 _init_signal(void);

s32 __sigtramp(s32 sig);

s32 _kill_r(_reent *r, s32 pid, s32 sig);

s32 _getpid_r(_reent *r);

s32 _sprintf_r(_reent *r, char *str, const char *fmt, ...);

s32 sprintf(char *str, const char *fmt, ...);

extern const u8 _ctype_[];

s32 strcasecmp(const char *left, const char *right);

char *strrchr(const char *s, s32 c);

char *strstr(const char *haystack, const char *needle);

extern const u8 _ctype_[];

#define STRTOL_MAX 0x7fffffffffffffffUL

#define STRTOL_MIN 0x8000000000000000UL

long _strtol_r(_reent *r, const char *input, char **end, s32 base);

long strtol(const char *s, char **end, s32 base);

s32 gcc2_compiled__0(FILE *fp, __suio *uio);

/* Buffer an unbuffered stream using only the fields the native path initializes. */
s32 __sbprintf(FILE *fp, const char *fmt, va_list ap);

s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

/* NON_MATCHING: formatter control flow, frame and allocation remain different. */
extern const u8 _ctype_[264];

extern const char D_0054B658[8] __attribute__((section(".rodata")));

extern const char putsNewline[16];

extern const char printfSpaces[16];

extern const char printfZeros[16];

extern char *_dtoa_r(_reent *r, f64 value, s32 mode, s32 digits,
                     s32 *decimalPoint, s32 *sign, char **end);

char *cvt(_reent *r, f64 value, s32 precision, s32 flags, char *sign,
          s32 *decimalPoint, s32 format, s32 *length);

s32 exponent(char *output, s32 value, s32 marker);

s32 vsprintf(char *str, const char *fmt, va_list ap);

s32 vsprintf_r(_reent *r, char *str, const char *fmt, va_list ap);

s32 __swsetup(FILE *fp);

s32 __mcmp(_Bigint *a, _Bigint *b);

/* Little-endian IEEE binary64 word view used by the numeric helpers. */
typedef union libcDoubleBits_t {
    f64 value;
    u32 word[2];
    u64 bits;
} libcDoubleBits_t;

void _Bfree(_reent *r, _Bigint *v);

s32 _hi0bits(u32 x);

_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut);

_Bigint *_i2b(_reent *r, s32 i);

_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a);

_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);

_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n);

_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift);

_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b);

s32 gcc2_compiled__1(_Bigint *dividend, _Bigint *divisor);

extern const char dtoaInfinityText[], dtoaNanText[];

extern const f64 dtoaLog10Slope __attribute__((section(".rodata")));

extern const f64 dtoaLog10Offset __attribute__((section(".rodata")));

extern const f64 dtoaLog10Of2 __attribute__((section(".rodata")));

extern const f64 dtoaPow10Big[];

extern const f64 dtoaPow10Limit __attribute__((section(".rodata")));

extern const char printfNanText[8] __attribute__((section(".rodata")));

extern const char printfHexLower[24];

extern const char printfNullText[8] __attribute__((section(".rodata")));

extern const char printfHexUpper[24];

extern const char printfBadBaseMsg[32];

extern const char printfZeroDigit[8] __attribute__((section(".rodata")));

extern const char printfDecimalPoint[16];

extern const char dtoaInfinityText[16];

extern const char dtoaNanText[8] __attribute__((section(".rodata")));

s32 fflush(void *stream);

void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data);

_glue *__sfmoreglue(_reent *r, s32 n);

FILE *__sfp(_reent *r);

void _cleanup_r(_reent *r);

void _cleanup(void);

void __sinit(_reent *r);

s32 _malloc_trim_r(_reent *r, u32 pad);

extern void *memmove(void *destination, const void *source, size_t size);

extern void *memchr(const void *source, s32 character, u32 size);

s32 __sfvwrite(FILE *fp, __suio *uio);

s32 _fwalk(_reent *r, s32 (*func)(void *));

extern const char localeEmptyStr[];

extern const char localeCName[];

extern s32 strcmp(const char *left, const char *right);

char *_setlocale_r(_reent *r, s32 category, const char *locale);

void *_localeconv_r(_reent *r);

char *setlocale(s32 category, const char *locale);

void *localeconv(void);

extern s32 _fstat_r(_reent *r, s32 fd, struct stat *buf);

extern s32 isatty(s32 fd);

void __smakebuf(FILE *fp);

s32 _mbtowc_r(_reent *r, s32 *pwc, const char *s, u32 n, s32 *state);

_Bigint *_Balloc(_reent *r, s32 k);

void _Bfree(_reent *r, _Bigint *v);

_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a);

/* Ordinary source reconstruction of the newlib decimal-string to Bigint helper. */
_Bigint *_s2b(_reent *r, const char *s, s32 nd0, s32 nd, u32 y9);

s32 _hi0bits(u32 x);

s32 _lo0bits(u32 *y);

_Bigint *_i2b(_reent *r, s32 i);

/* Native indexed constants span the separately labeled word at 0x0054B928. */
extern const s32 pow5MultTable[];

_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);

_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n);

/* Qualified allocator controls for the two count lifetimes are recorded in
 * docs/levers/cross-block-locals.md, "Retain bigint limb and bit counts". */
_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift);

s32 __mcmp(_Bigint *a, _Bigint *b);

/* The shared pointer and limb intermediates have qualified allocator controls;
 * see docs/levers/cross-block-locals.md, "Retain bigint subtraction values". */
_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b);

f64 _ulp(f64 x);

f64 _b2d(_Bigint *b, s32 *exponent);

_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut);

f64 _b2d(_Bigint *b, s32 *exponent);

f64 _ratio(_Bigint *a, _Bigint *b);

double _mprec_log10(s32 dig);

s32 __sread(void *cookie, char *buf, s32 n);

s32 __swrite(void *cookie, const char *buf, s32 n);

long __sseek(void *cookie, s32 offset, s32 whence);

extern s32 _close_r(_reent *r, s32 fd);

s32 __sclose(void *cookie);

long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

s32 _close_r(_reent *r, s32 fd);

s32 _fstat_r(_reent *r, s32 fd, struct stat *buf);

long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence);

long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern f64 dtoaLog10Slope __attribute__((section(".rodata")));

extern f64 dtoaLog10Offset __attribute__((section(".rodata")));

extern const f64 dtoaLog10Of2 __attribute__((section(".rodata")));

extern const char localeEmptyStr[8] __attribute__((section(".rodata")));

extern const char localeDecimalPoint[8] __attribute__((section(".rodata")));

extern const char localeCName[4] __attribute__((section(".rodata")));

extern const s32 pow5MultTable[2] __attribute__((section(".rodata")));

extern const f64 pow10Table[25];

extern const f64 dtoaPow10Big[4];

extern const f64 dtoaPow10Limit __attribute__((section(".rodata")));

extern const f64 dtoaPow10Tiny[5];

extern const char kernlInternalErrorMsg[40];

extern const char ttyMsgPacketTooLarge[40];

extern const char ttyMsgReceiveError[24];

extern const char ttyMsgSendError[24];

extern const char ttyMsgWlenError[24];
/* End of declarations from src/smt3/sdk/libc.c */

long _strtol_r(_reent *r, const char *input, char **end, s32 base)
{
    const char *s = input;
    unsigned long acc;
    s32 c;
    unsigned long cutoff;
    s32 negative = 0;
    s32 any;
    s32 cutlim;

    do {
        c = *s++;
    } while ((_ctype_ + 1)[c] & 8);
    if (c == '-') {
        negative = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((base == 0 || base == 16) && c == '0' &&
        (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = negative ? STRTOL_MIN : STRTOL_MAX;
    cutlim = cutoff % (unsigned long)base;
    cutoff /= (unsigned long)base;
    for (acc = 0, any = 0;; c = *s++) {
        if ((_ctype_ + 1)[c] & 4)
            c -= '0';
        else if ((_ctype_ + 1)[c] & 3)
            c -= ((_ctype_ + 1)[c] & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            goto done;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
done:
    if (any < 0) {
        acc = negative ? STRTOL_MIN : STRTOL_MAX;
        r->_errno = 34;
    } else if (negative) {
        acc = -acc;
    }
    if (end != 0)
        *end = (char *)(any ? s - 1 : input);
    return acc;
}

long strtol(const char *s, char **end, s32 base) {
    return _strtol_r(_impure_ptr, s, end, base);
}
