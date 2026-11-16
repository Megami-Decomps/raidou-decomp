#ifndef MVI_MANAGER_H
#define MVI_MANAGER_H

#include "common.h"
#include "sdf_movie_types.h"

typedef struct
{
    /* 0x00 */ u8 unk_00[0x3A];
    /* 0x3A */ u8 viewerStep;
    /* 0x3B */ u8 unk_3B[0x29];
    /* 0x64 */ s32 frameCount;
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 frameIndex;
} movieDecodeWork_t;

extern movieDecodeWork_t movieDecodeWork asm("movieCb");
extern sdfMovieCb_t movieCb;
extern s32 movieData;

void movieStop(void);
void moviePlay(s32 aNum);
s32 mviValidateMovieSub(void);
s32 mviDrawMessageSub(void);
void movieViewerFree(void);
void *dds3StartMovieManager(void);

#endif
