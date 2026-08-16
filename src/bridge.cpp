#include <cstddef>
#include "protocol.h"

namespace {
bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap);
bool InspectHealDialog(cleanroute::Snapshot& s, wchar_t* detail, std::size_t cap);
}

#include "bridge_part01.inc"
#include "bridge_lua_array.inc"
#include "bridge_part02.inc"
#include "bridge_part03.inc"
#define FindButtonInUi FindButtonInUiLegacyV116
#define InspectHealDialog InspectHealDialogLegacyV116
#define ClickHealDialogChoice ClickHealDialogChoiceV113
#include "bridge_part04.inc"
#undef ClickHealDialogChoice
#undef InspectHealDialog
#undef FindButtonInUi
[[maybe_unused]] bool (*const kLegacyHealChoiceV113)(cleanroute::HealDialogChoice, wchar_t*, std::size_t) =
    &ClickHealDialogChoiceV113;
[[maybe_unused]] bool (*const kLegacyInspectHealV116)(cleanroute::Snapshot&, wchar_t*, std::size_t) =
    &InspectHealDialogLegacyV116;
#include "bridge_part05.inc"
#include "bridge_part06.inc"

// Preserve v1.1.8 UIRoot/button observer only as lineage/support for the old
// MainThread UIButton experiment. Runtime v1.1.8 proved this representation has
// zero clickable/text nodes for the live dynamic GameDialog.
#define FindButtonInUi FindButtonInUiLegacyV118
#define InspectHealDialog InspectHealDialogLegacyV118
#include "bridge_dialog_v1_1_8.inc"
#undef InspectHealDialog
#undef FindButtonInUi
[[maybe_unused]] bool (*const kLegacyInspectHealV118)(cleanroute::Snapshot&, wchar_t*, std::size_t) =
    &InspectHealDialogLegacyV118;

// Retain the harmless CTS/MainThread proof. Its old gameplay wrapper remains
// compiled only for lineage and is wired to the legacy v1.1.8 button resolver.
#define FindButtonInUi FindButtonInUiLegacyV118
#define ClickHealDialogChoice ClickHealDialogChoiceV116
#include "bridge_mainthread_v1_1_6.inc"
#undef ClickHealDialogChoice
#undef FindButtonInUi
[[maybe_unused]] bool (*const kLegacyHealChoiceV116)(cleanroute::HealDialogChoice, wchar_t*, std::size_t) =
    &ClickHealDialogChoiceV116;

// v1.1.9 active path: inspect live Lua GameDialogData.Selections rather than UIRoot.
#include "bridge_lua_dialog_v1_1_9.inc"
#include "bridge_action_v1_1_9.inc"
