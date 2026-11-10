#ifndef DDS_BTL_PARTY_COMMAND_USE_RESULT_H

#define DDS_BTL_PARTY_COMMAND_USE_RESULT_H
#include "common.h"

/* Only ALLOW proceeds to field-skill use. The nonzero values identify
 * distinct gate predicates: the marked-member requirement lacks a match,
 * the helper's counter/flag check is positive, or a special field flag is set. */
typedef u16 BtlPartyCommandUseGateResult;

enum {
    BTL_PARTY_COMMAND_USE_GATE_ALLOW = 0,
    BTL_PARTY_COMMAND_USE_GATE_MARKED_MEMBER_REQUIRED = 2,
    BTL_PARTY_COMMAND_USE_GATE_COUNTER_CHECK_MATCHED = 3,
    BTL_PARTY_COMMAND_USE_GATE_SPECIAL_FIELD_FLAG_SET = 4,
};

#ifdef VERSION_DDS1

BtlPartyCommandUseGateResult btlCheckPartyCommandCounterEligibility(
    s32 commandId, u32 memberMask);

#endif

#ifdef VERSION_DDS2

BtlPartyCommandUseGateResult btlCheckSelectedPartyCommandEligibility(
    s32 commandId, u32 memberMask);

#endif

#endif /* DDS_BTL_PARTY_COMMAND_USE_RESULT_H */
