#ifndef FCLDRAWBACK_H
#define FCLDRAWBACK_H

#include "common.h"
#include "code_r1_0021CE60.h"

struct sprHandle_t;
struct sdfMemHandle_t;

/* Process data of the fclMsg process, allocated by fclProcessStart. */
typedef struct fclMsgWork_t {
    struct sdfMemHandle_t *hMem;  /* 0x00: this block */
    struct sdfMemHandle_t *hData; /* 0x04 */
    void *data;                   /* 0x08 */
    u8 unk0C[0xC];
    s32 unk18;                    /* 0x18: fclUpdateEx's result */
    u8 unk1C[0x4];
} fclMsgWork_t;
SIZE_CHECK(fclMsgWorkSizeCheck, fclMsgWork_t, 0x20);

void _BackSprite(s32 a_moveType, s32 a_x, s32 a_y, s32 a_z, u8 a_alphaRate, struct sprHandle_t *a_hSpr, s32 a_sprNo, u32 a_otNo);
void fclBackSprite(s32 a_type, u8 a_alphaRate1, u8 a_alphaRate2, struct sprHandle_t *a_hSpr);
void fclCommandHelp(s32 a_cmdID, u8 a_alphaRate);
void fclCallMemFreeDirect(fclMsgWork_t *wk);

#endif
