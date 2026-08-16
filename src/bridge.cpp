#include <cstddef>
#include "protocol.h"

namespace {
bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap);
}

#include "bridge_part01.inc"
#include "bridge_lua_array.inc"
#include "bridge_part02.inc"
#include "bridge_part03.inc"
#define ClickHealDialogChoice ClickHealDialogChoiceV113
#include "bridge_part04.inc"
#undef ClickHealDialogChoice
#include "bridge_part05.inc"
#include "bridge_part06.inc"
// v1.1.4 semantic packet helper remains in source/history but is intentionally
// not compiled into the active v1.1.6 bridge after runtime failure on v1.1.4/v1.1.5.
#include "bridge_mainthread_v1_1_6.inc"
