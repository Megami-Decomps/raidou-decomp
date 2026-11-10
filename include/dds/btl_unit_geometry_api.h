#ifndef DDS_BTL_UNIT_GEOMETRY_API_H

#define DDS_BTL_UNIT_GEOMETRY_API_H
struct BtlUnit;

/* Evaluates the actor's body position into VU registers without changing it. */
void btlUnitGetBodyPosVU(const struct BtlUnit *unit);

#endif /* DDS_BTL_UNIT_GEOMETRY_API_H */
