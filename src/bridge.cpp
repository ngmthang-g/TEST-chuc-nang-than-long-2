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

// Preserve v1.1.8 UIRoot/button observer only as lineage/support for old experiments.
#define FindButtonInUi FindButtonInUiLegacyV118
#define InspectHealDialog InspectHealDialogLegacyV118
#include "bridge_dialog_v1_1_8.inc"
#undef InspectHealDialog
#undef FindButtonInUi
[[maybe_unused]] bool (*const kLegacyInspectHealV118)(cleanroute::Snapshot&, wchar_t*, std::size_t) =
    &InspectHealDialogLegacyV118;

// Retain the harmless CTS/MainThread proof. Its old gameplay wrapper is lineage only.
#define FindButtonInUi FindButtonInUiLegacyV118
#define ClickHealDialogChoice ClickHealDialogChoiceV116
#include "bridge_mainthread_v1_1_6.inc"
#undef ClickHealDialogChoice
#undef FindButtonInUi
[[maybe_unused]] bool (*const kLegacyHealChoiceV116)(cleanroute::HealDialogChoice, wchar_t*, std::size_t) =
    &ClickHealDialogChoiceV116;

// Preserve v1.1.9 Lua probe/action as lineage. Runtime v1.1.9 proved the exact failure
// is earlier: LuaSystemManager instance resolution, before LuaEnv/DoString/Selections.
#define InspectHealDialog InspectHealDialogLegacyV119
#include "bridge_lua_dialog_v1_1_9.inc"
#undef InspectHealDialog
[[maybe_unused]] bool (*const kLegacyInspectHealV119)(cleanroute::Snapshot&, wchar_t*, std::size_t) =
    &InspectHealDialogLegacyV119;

#define ClickHealDialogChoice ClickHealDialogChoiceLegacyV119
#include "bridge_action_v1_1_9.inc"
#undef ClickHealDialogChoice
[[maybe_unused]] bool (*const kLegacyHealChoiceV119)(cleanroute::HealDialogChoice, wchar_t*, std::size_t) =
    &ClickHealDialogChoiceLegacyV119;

// v1.1.10 active path: resolve LuaEnv without assuming a named singleton. First try
// static get_LuaEnv; then semantic singleton/static-reference/Unity-object fallbacks.
#include "bridge_lua_manager_v1_1_10.inc"
#include "bridge_action_v1_1_10.inc"
