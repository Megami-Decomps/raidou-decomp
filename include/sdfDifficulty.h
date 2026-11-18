#ifndef SDF_DIFFICULTY_H
#define SDF_DIFFICULTY_H

/* Battle rewards, drops and encounters. Each macro expands to retail's code,
 * so this build is unchanged. A host that scales them defines them first. */

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

#endif
