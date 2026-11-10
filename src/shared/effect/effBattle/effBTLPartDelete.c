#include "shared/effect/effBattle/decls.h"

/* Release each initialized parameter-work entry in the handle's two banks. */
void effBTLPartDelete(effBTL_t *pSkill) {
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        u16 entryCount = pSkill->banks[bankIndex].list.count;
        effBTL_SkillPartsList_t *entry = pSkill->banks[bankIndex].list.entries;

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->type != 0xFFFE) {
                    dds3EffectPartDelete(entry->value.work);
                }
                entry++;
                remaining--;
            } while (remaining != 0);
        }
    }
}
