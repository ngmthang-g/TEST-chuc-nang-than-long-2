#include "heal_logic.h"
#include <cstdio>
using namespace cleanroute_heal;

static int g_fail = 0;
static void Check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++g_fail;
}

int main() {
    Check(DecideDialogAction(Phase::WaitTreatment, kTreatmentMask) == DialogAction::Treatment,
          "treatment-visible->click-treatment");
    Check(DecideDialogAction(Phase::WaitTreatment, kConfirmMask) == DialogAction::None,
          "wrong-dialog-do-not-click");
    Check(DecideDialogAction(Phase::WaitConfirm, kConfirmMask) == DialogAction::Confirm,
          "confirm-visible->click-confirm");
    Check(DecideDialogAction(Phase::WaitKnow, kKnowMask) == DialogAction::Know,
          "know-visible->click-know");
    Check(AfterDialogAction(DialogAction::Treatment) == Phase::WaitConfirm, "after-treatment");
    Check(AfterDialogAction(DialogAction::Confirm) == Phase::WaitKnow, "after-confirm");
    Check(AfterDialogAction(DialogAction::Know) == Phase::Done, "after-know");
    std::printf("RESULT %d/7 PASS\n", 7 - g_fail);
    return g_fail ? 1 : 0;
}
