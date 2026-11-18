#ifndef SDF_DIFFICULTY_H
#define SDF_DIFFICULTY_H

/* Battle rewards, drops, encounters, damage, status odds and capture. Each
 * macro expands to retail's code, so this build is unchanged. A host that
 * scales them defines them first. */

/* Called with the rstWork_t rstSetReward just stored, before the result
 * screen reads it. */
#ifndef SDF_DIFFICULTY_REWARD
#define SDF_DIFFICULTY_REWARD(reward) ((void)0)
#endif

/* An item drop's chance in percent, after luck and skills, compared with a
 * roll of 1 to 100. */
#ifndef SDF_DIFFICULTY_DROP_CHANCE
#define SDF_DIFFICULTY_DROP_CHANCE(chance) (chance)
#endif

/* What 100 units of walking add to the random encounter counter; an
 * encounter is due when the counter reaches the area's limit. */
#ifndef SDF_DIFFICULTY_ENCOUNTER_STEP
#define SDF_DIFFICULTY_ENCOUNTER_STEP(add) (add)
#endif

/* The chance in percent that a skill inflicts the status bst (a BST), after
 * luck, resistance and auto skills, compared with GetPercentage's roll. */
#ifndef SDF_DIFFICULTY_STATUS_CHANCE
#define SDF_DIFFICULTY_STATUS_CHANCE(chance, bst) (chance)
#endif

/* The damage a skill does to wk after every modifier and the 999 cap, before
 * counters and endure; -1 is a miss. The value applied and shown. */
#ifndef SDF_DIFFICULTY_DAMAGE
#define SDF_DIFFICULTY_DAMAGE(wk, damage) (damage)
#endif

/* One frame of the capture contest: step is sbtlCallMemset10, whose first
 * frame sets up the pushes in wk_sbtl (field4C2 to field4CC). */
#ifndef SDF_DIFFICULTY_CAPTURE_STEP
#define SDF_DIFFICULTY_CAPTURE_STEP(step, wk) step(wk)
#endif

#endif
