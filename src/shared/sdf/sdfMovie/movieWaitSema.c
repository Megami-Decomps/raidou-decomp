#include "shared/sdf/sdfMovie/decls.h"

u8 *movieWaitSema(s32 aSize)
{
    sdfMovieCb_t *mov = movieSemaId;
    sdfMovieStrmPac_t *obj = mov->readCb.pac;
    u8 *ptr;

    WaitSema(movieStreamSema);
    if (obj->scratchSize > 0) {
        ptr = obj->scratch;
        obj->scratchSize -= aSize;
        obj->scratch = ptr + aSize;
    } else {
        s32 offset = obj->ringOffset;
        ptr = obj->ringBuffer + offset;
        if (obj->ringLength < aSize) {
            /* Retail copies the old count before reusing its register for
             * the XOR predicate: countScratch has those two live phases.
             * Ordinary scalar reuse keeps the select; these local constraints
             * recover native allocation and XOR operand order. This is a
             * reconstruction aid, not original-source evidence. */
            FAKEMATCH_ASM_REG(s32, frame, "$2") = mov->decodeCb.frame;
            FAKEMATCH_ASM_REG(u32, countScratch, "$3") = movieSemaCount;
            u32 count;

            ptr = NULL;
            count = countScratch;
            countScratch = (u32)frame ^ countScratch;
            FAKEMATCH_ASM_LIVE(countScratch);
            if (countScratch != 0) {
                count = frame;
            }
            movieSemaCount = count;
        } else {
            obj->ringLength -= aSize;
            offset += aSize;
            if (offset > 0xFFFF) {
                offset -= 0x10000;
            }
            obj->ringOffset = offset;
        }
    }
    SignalSema(movieStreamSema);
    return ptr;
}
