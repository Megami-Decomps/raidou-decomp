#ifndef DDS_EFF_BATTLE_VOICE_KIND_H

#define DDS_EFF_BATTLE_VOICE_KIND_H
#include "common.h"

/* Mixer-bank selector for battle voices. Values below two use bank zero;
 * values two and above use bank one. Kind two also uses triggerFrame as the
 * draw endpoint instead of colorRampEndFrame. */
typedef u16 EffBattleVoiceKind;

#endif /* DDS_EFF_BATTLE_VOICE_KIND_H */
