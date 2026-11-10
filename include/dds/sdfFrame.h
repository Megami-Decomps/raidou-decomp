#ifndef DDS_SDF_FRAME_H

#define DDS_SDF_FRAME_H
#include "../sdfFrame.h"
/* Frame timers, written in the retail logic tick: one tick per two vblanks,
 * dds3VblanksPerFrame's default. Each macro expands to its argument, so this
 * build is unchanged. A host that runs more ticks per second defines them
 * first, scaling a duration in ticks up and a per-tick step down by the same
 * ratio, so a timer lasts as long and a walk covers as much ground. */

/* A duration in ticks, an integer. A count of vblanks scales by the same
 * ratio, since a host keeps two vblanks to a tick. */

#ifndef SDF_FRAMES

#endif

/* A duration in ticks held in a float. Its counters step by exactly 1.0f and
 * are compared with ==, so a host must answer a whole number. */

#ifndef SDF_FRAMES_F

#endif

/* A countdown decremented at the end of each tick and acted on at 0, set
 * mid-tick. Its value less one counts the ticks still run, so a host scales
 * that part and keeps 1 meaning "ends with this tick". */

#ifndef SDF_FRAMES_COUNTDOWN

#endif

/* An advance per tick, in whatever unit the counter moves. */

#ifndef SDF_FRAME_STEP

#endif

/* A float counter x raised or lowered by the step per tick d to a limit that
 * retail's whole steps land on exactly. A host scales d and stops x at the
 * limit. */

#ifndef SDF_FRAME_STEP_UP

#endif

#ifndef SDF_FRAME_STEP_DOWN

#endif

/* An advance per tick added to an integer, given a count of host ticks from
 * 0. A host splits d into whole steps that add up to exactly d over each
 * retail tick, where a rounded SDF_FRAME_STEP would stall or run fast. */

#ifndef SDF_FRAME_STEP_INT

#endif

/* Runs where a floor face's edge band reached a sphere, before the sphere is
 * moved to that clearance. Below retail's step the move pulls a sphere above
 * the floor into the next face, which pushes it back each tick, so a host
 * that shortens the step keeps it where it is. Expands to nothing here. */

#ifndef SDF_FRAME_FLOOR_EDGE

#endif

/* A factor applied once per tick, such as a velocity's decay. A host raises
 * it to the power it scales a step by, so the decay over a second holds. */

#ifndef SDF_FRAME_DECAY

#endif

/* A divisor applied once per tick, as x += (target - x) / d: the decay
 * 1 - 1/d, so a host's SDF_FRAME_DECAY scales it. Folds to d here. */

#ifndef SDF_FRAME_DIVISOR

#endif

/* The retail tick a host tick count falls in, an integer: the inverse of
 * SDF_FRAMES, for a key or a parity that must advance at the retail rate. */

#ifndef SDF_FRAME_TICK

#endif

/* True on the first host tick of each retail tick, given a count of host
 * ticks from 0. Gates a step that cannot be scaled, such as a record into a
 * history ring or an integer ramp, so it runs at the retail rate. */

#ifndef SDF_FRAME_DUE

#endif

/* The random state for a draw that code retail runs once per tick makes each
 * host tick, such as a jitter drawn while drawing. A host answers the shared
 * stream on one host tick of each retail tick and a stream of its own on the
 * rest, so the shared stream advances at the retail rate. */

#ifndef SDF_FRAME_RAND

#endif

/* Runs one statement on the first host tick of each retail tick. */

#ifndef SDF_FRAME_WHEN

#endif

/* The bits of a key mask, read only on the first host tick of each retail
 * tick, so a held key repeats at the retail rate. */

#ifndef SDF_FRAME_MASK

#endif

/* A value drawn each host tick from state its owner updates once per retail
 * tick, given a count of host ticks from 0 and a key no other interpolated value
 * uses. A host draws it between its values at the ends of the last two retail
 * ticks by the share of this tick run, so it moves every host tick and lands
 * on the retail value at the tick's last host tick. The state is unchanged,
 * so retail draws the value itself. */

#ifndef SDF_FRAME_INTERPOLATE

#endif

/* SDF_FRAME_INTERPOLATE of a value read before its owner updates it in the
 * same retail tick, so the value changes between that tick's host ticks. A
 * host samples it on the first host tick of each retail tick only. */

#ifndef SDF_FRAME_INTERPOLATE_DUE

#endif

/* Forgets what SDF_FRAME_INTERPOLATE drew for key, where its owner starts the
 * value over, so the next draw starts at the new value instead of moving from
 * the old one. Expands to nothing here. */

#ifndef SDF_FRAME_INTERPOLATE_RESET

#endif

/* SDF_FRAME_INTERPOLATE_RESET of key when cond holds, such as a camera cut.
 * Expands to nothing here, cond included. */

#ifndef SDF_FRAME_INTERPOLATE_RESET_IF

#endif

/* Sets lvalue to SDF_FRAME_INTERPOLATE(age, key, value). Expands to nothing here. */

#ifndef SDF_FRAME_INTERPOLATE_SET

#endif

/* Sets the n floats at dst to SDF_FRAME_INTERPOLATE of the n floats at src, the
 * i-th keyed by key and i. Expands to nothing here. */

#ifndef SDF_FRAME_INTERPOLATE_VEC

#endif

/* Four floats drawn each host tick from the four at src, which their owner
 * updates once per retail tick: a host points at a copy whose first n lanes
 * are SDF_FRAME_INTERPOLATE of src's, keyed by src. Retail points at src. */

#ifndef SDF_FRAME_INTERPOLATE_PTR

#endif

/* Sets basic's position to the vector at pos, which its owner sets once per
 * retail tick. A host sets SDF_FRAME_INTERPOLATE of it, keyed by pos, and sets
 * it again at each SDF_FRAME_INTERPOLATE_POSES on the rest of the retail tick. */

#ifndef SDF_FRAME_INTERPOLATE_POSE

#endif

/* Sets basic's rotation to the quaternion at rot, which its owner sets once per
 * retail tick. A host interpolates it the short way round, keyed by rot, and
 * sets it again at each SDF_FRAME_INTERPOLATE_POSES as it does positions. */

#ifndef SDF_FRAME_INTERPOLATE_ROT

#endif

/* Sets again each position SDF_FRAME_INTERPOLATE_POSE and each rotation
 * SDF_FRAME_INTERPOLATE_ROT set in this retail tick.
 * Expands to nothing here. */

#ifndef SDF_FRAME_INTERPOLATE_POSES

#endif

/* Brackets the reads of positions recorded once per retail tick by parts
 * that follow a moving mark: pos is the mark at the last retail tick and
 * posOld the one before. A host running more ticks than retail may move the
 * followers between retail ticks, reading the mark itself from root's node
 * mark, and must put pos and posOld back at END so the next record still
 * spans one retail tick. Retail reads the recorded positions, so both expand
 * to nothing. */

#ifndef SDF_FRAME_FOLLOW_BEGIN

#endif

#ifndef SDF_FRAME_FOLLOW_END

#endif

/* Storage for a tick count. A host may widen one only where C defines the
 * storage: a symbol the data sections define keeps its retail width. */

#ifndef SDF_FRAME_U8

#endif

#ifndef SDF_FRAME_U16

#endif

#endif
