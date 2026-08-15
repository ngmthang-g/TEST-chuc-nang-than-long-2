#pragma once
#include <cstdint>

namespace cleanroute_heal {

constexpr std::uint32_t kTreatmentMask = 1u << 0;
constexpr std::uint32_t kConfirmMask = 1u << 1;
constexpr std::uint32_t kKnowMask = 1u << 2;

enum class Phase {
    Idle,
    Travel,
    OpenNpc,
    WaitTreatment,
    WaitConfirm,
    WaitKnow,
    Done,
    Failed,
};

enum class DialogAction {
    None,
    Treatment,
    Confirm,
    Know,
};

inline DialogAction DecideDialogAction(Phase phase, std::uint32_t mask) {
    if (phase == Phase::WaitTreatment && (mask & kTreatmentMask)) return DialogAction::Treatment;
    if (phase == Phase::WaitConfirm && (mask & kConfirmMask)) return DialogAction::Confirm;
    if (phase == Phase::WaitKnow && (mask & kKnowMask)) return DialogAction::Know;
    return DialogAction::None;
}

inline Phase AfterDialogAction(DialogAction action) {
    switch (action) {
        case DialogAction::Treatment: return Phase::WaitConfirm;
        case DialogAction::Confirm: return Phase::WaitKnow;
        case DialogAction::Know: return Phase::Done;
        default: return Phase::Failed;
    }
}

} // namespace cleanroute_heal
