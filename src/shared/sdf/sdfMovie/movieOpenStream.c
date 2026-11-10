#include "shared/sdf/sdfMovie/decls.h"

void movieOpenStream(sdfMovieCb_t *stream, sdfMovieInitParm_t *desc, const char *path)
{
    sdfIpuInitParm_t fmt;
    sdfMovieStrmLinear_t *linear;
    sdfMovieStrmPac_t *pac;
    char *ext;
    u8 isPac;

    memset(stream, 0, sizeof(sdfMovieCb_t));
    stream->stat = 1;
    ext = strrchr(path, '.');
    isPac = 0;
    if (ext != NULL) {
        isPac = strcasecmp(ext, D_00551B50) == 0;
    }
    stream->mode = isPac;
    fmt.loopMode = 0;
    stream->readRemain = 0x7FFFFFFF;
    stream->x = desc->x;
    stream->z = desc->z;
    stream->y = desc->y;
    stream->width = desc->width;
    stream->height = desc->height;
    fmt.rgbMode = desc->rgbMode;
    fmt.ditherMode = desc->ditherMode;
    fmt.fps = desc->fps;
    if (isPac == 0) {
        sdfMemHandle_t *handle;
        u8 *base;

        linear = sdfChipAllocZeroDirect(sizeof(sdfMovieStrmLinear_t));
        stream->readCb.linear = linear;
        handle = sdfMemAllocDirect(0x20000);
        linear->allocation = handle;
        base = sdfMemLock(handle);
        linear->bufferStart = base;
        linear->readCursor = base;
        linear->writeCursor = base;
        stream->readStat = 0;
        stream->fileHandle = sdfFileCmdReadOpen(path, (SdfStreamRead)movieGetStreamSize, stream);
        movieAttachDecodeTask(&stream->decodeCb, &fmt, movieCopyChunk, stream, desc->resourceWord);
    } else {
        sdfMemHandle_t *handle;
        u8 *base;
        u8 *ring;
        u8 *body;
        u8 *scr;
        s32 n;

        if (movieStreamSema < 0) {
            movieStreamSema = sdfCreateSema(1, 1, 0);
        }
        movieSemaCount = -1;
        pac = sdfChipAllocZeroDirect(sizeof(sdfMovieStrmPac_t));
        stream->readCb.pac = pac;
        handle = sdfMemAllocDirect(0x24000);
        pac->allocation = handle;
        base = sdfMemLock(handle);
        n = 0x20;
        ring = base + 0x14000;
        body = base + 0x4000;
        scr = pac->scratchBuffer;
        pac->pacBuffer = body;
        pac->scratch = scr;
        pac->ringBuffer = ring;
        pac->scratchSize = n;
        pac->pacInputBuffer = base;
        stream->readStat = 0;
        stream->fileHandle = sdfFileCmdReadOpen(path, (SdfStreamRead)movieReadChunk, stream);
        stream->sndActive = 1;
        stream->sndVolume = 0x7F;
        movieAttachDecodeTask(&stream->decodeCb, &fmt, movieParseHeader, stream, desc->resourceWord);
    }
}
