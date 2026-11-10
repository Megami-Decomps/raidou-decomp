#ifndef DDS_MDL_LIGHT_EDITOR_STATE_H

#define DDS_MDL_LIGHT_EDITOR_STATE_H
#include "common.h"

struct MdlPadState;

struct sdfDrawTag_t;

typedef struct MdlLightEditorState {
    f32 (**lights)[4];
    f32 *ambient;
    struct MdlPadState *pad;
    u8 editing;
    u8 blinkTick;
    s16 selection;
    struct sdfDrawTag_t *packetList;
} MdlLightEditorState;

typedef char MdlLightEditorState_size[(sizeof(MdlLightEditorState) == 0x14) ? 1 : -1];

#endif
