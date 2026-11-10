#ifndef DDS_BTL_CUSTOM_FADE_TASK_ARGS_H

#define DDS_BTL_CUSTOM_FADE_TASK_ARGS_H
/* Packed RGB word and duration consumed by the custom battle fade task. */
typedef struct BtlCustomFadeTaskArgs {
    union {
        u32 packedColor; /* 0x00: byte 3 is not read by this task. */
        struct {
            u8 red;       /* 0x00 */
            u8 green;     /* 0x01 */
            u8 blue;      /* 0x02 */
            u8 unused03;  /* 0x03 */
        } rgb;
    };
    s32 duration; /* 0x04 */
} BtlCustomFadeTaskArgs;

typedef char BtlCustomFadeTaskArgsSizeCheck[
    sizeof(BtlCustomFadeTaskArgs) == 0x08 ? 1 : -1];

#endif /* DDS_BTL_CUSTOM_FADE_TASK_ARGS_H */
