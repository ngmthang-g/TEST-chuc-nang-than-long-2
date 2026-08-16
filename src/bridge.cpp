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

// v1.1.7 active observer is defined after the legacy split body. ProcessRequest
// sees its forward declaration above; the later MainThread action helper sees
// the v1.1.7 FindButtonInUi implementation directly.
#include "bridge_dialog_v1_1_7.inc"

// Historical v1.1.4 packet helper remains preserved in source/history but is not active.
// v1.1.7 keeps the v1.1.6 MainThread action boundary and changes the shared dialog discovery layer.
#include "bridge_mainthread_v1_1_6.inc"
