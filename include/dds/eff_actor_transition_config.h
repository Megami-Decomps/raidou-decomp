#ifndef DDS_EFF_ACTOR_TRANSITION_CONFIG_H

#define DDS_EFF_ACTOR_TRANSITION_CONFIG_H
#include "eff.h"

/* Kind-specific payload copied immediately after EffActiveResource. */
typedef struct EffActorLightConfig {
    u32 duration;
    s32 colorFadeIn;
    s32 colorFadeOut;
    s32 directionFadeIn;
    s32 directionFadeOut;
    u32 firstColor;
    u32 secondColor;
    u8 actorSelection;
    u8 pad1D[3]; /* Copied bytes with no confirmed field interpretation. */
    EffectVectorRequest direction;
} EffActorLightConfig;

typedef struct EffActorTintConfig {
    u32 duration;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u8 actorSelection;
    u8 pad11[3]; /* Copied bytes with no confirmed field interpretation. */
} EffActorTintConfig;

typedef char EffActorLightConfig_size_must_be_0x28[
    (sizeof(EffActorLightConfig) == 0x28) ? 1 : -1];

typedef char EffActorTintConfig_size_must_be_0x14[
    (sizeof(EffActorTintConfig) == 0x14) ? 1 : -1];

#endif /* DDS_EFF_ACTOR_TRANSITION_CONFIG_H */
