#include "shared/effect/effBattle/decls.h"

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void effBTLPartCreate(effBTL_t *pSkill, effBTL_t *pData) {
    u8 *sourceBytes = (u8 *)pData;
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        effBTL_SkillPartsList_t *entry = pSkill->banks[bankIndex].list.entries;
        u16 entryCountForBank = pSkill->banks[bankIndex].list.count;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->type != 0xFFFE) {
                    if (entry->type == 0xFFFF) {
                        u32 index = entry->value.sourceOffset;
                        u32 sourceBank = index / 0x3C;
                        u32 sourceEntryIndex = index % 0x3C;
                        effBTL_SkillPartsList_t *sourceEntry = &pSkill->banks[sourceBank].list.entries[sourceEntryIndex];
                        dds3EffectPart_t *duplicatedWork = dds3EffectPartCopy(sourceEntry->value.work);
                        u16 sourceKind = sourceEntry->type;

                        entry->value.work = duplicatedWork;
                        entry->type = sourceKind;
                    } else {
                        entry->value.work = dds3EffectPartCreate(entry->type, sourceBytes + entry->value.sourceOffset);
                    }
                    if (entry->type == EFF_PARAM_WORK_KIND_EFFECT_NODE) {
                        entry->timing++;
                    }
                }
                entry++;
                remainingEntries--;
            } while (remainingEntries != 0);
        }
    }
}
