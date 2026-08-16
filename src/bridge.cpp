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
#define ClickHealDialogChoice ClickHealDialogChoiceV114
#include "bridge_heal_packet_v1_1_4.inc"
#undef ClickHealDialogChoice
#include "bridge_mainthread_v1_1_6.inc"
