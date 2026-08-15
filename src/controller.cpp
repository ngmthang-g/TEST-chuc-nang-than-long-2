#include "controller_part01.inc"

// v1.1.5 targeted runtime test:
// Keep the v1.1.4 source intact, but override only the active test NPC after its
// legacy constants have been parsed. This isolates the variable under test:
// NPC 339/Lau Lan versus NPC 463/Lac Duong.
static_assert(kHealNpcID == 339, "legacy baseline changed unexpectedly");
static_assert(sizeof(kTitle) > 0, "legacy title missing");

#define kTitle L"Than Long TEST v1.1.5 • Auto tri lieu • Long Pha Thien 463"
#include "controller_part02.inc"
#undef kTitle

// Do not auto-load the old Lau Lan capture in this version. The user must stand
// at Long Pha Thien in Lac Duong and press TU LAY TOA DO NPC again. Keep the
// legacy loader referenced so -Werror does not treat it as dead code.
(void)&LoadHealTarget;
Log(L"v1.1.5 ACTIVE NPC TEST • Long Pha Thien • ResID 463 • MapID expected 3 (Lac Duong) • BAT BUOC TU LAY TOA DO LAI");
#define LoadHealTarget(out) false
#include "controller_part03.inc"
#undef LoadHealTarget

// All runtime ClickNpc calls in the heal FSM below use 463. Coordinates remain
// exactly the raw MapID/X/Y captured by the user; no X/Y is hardcoded here.
#define kHealNpcID 463
#include "controller_part04.inc"

// Add explicit runtime breadcrumbs when the existing UI handlers are invoked.
// The underlying v1.1.4 semantic GameDialog action is intentionally unchanged.
#define CaptureHealTarget() (Log(L"v1.1.5: dang tu lay toa do cho Long Pha Thien 463 tai Lac Duong"), CaptureHealTarget())
#define StartAutoHeal() (Log(L"v1.1.5: AUTO TRI LIEU dung NPC Long Pha Thien 463; bo qua nhan cu 339 trong log legacy"), StartAutoHeal())
#include "controller_part05.inc"
#undef StartAutoHeal
#undef CaptureHealTarget
#undef kHealNpcID

#include "controller_part06.inc"
