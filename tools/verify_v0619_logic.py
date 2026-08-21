from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]

def r(path):
    return (ROOT / path).read_text(encoding="utf-8-sig")

def req(cond, name):
    if not cond:
        raise SystemExit(f"FAIL: {name}")

c = r("src/controller.cpp")
p = r("src/protocol.h")
w = r(".github/workflows/build.yml")
cm = r("CMakeLists.txt")
retry = r("src/auto_fight_retry_logic.h")
retry_test = r("src/auto_fight_retry_logic_test.cpp")
about_source = r("src/controller.cpp")

req(r("VERSION.txt").strip() == "v0.6.1.9", "version")
req("kProtocolVersion = 0x00010619u" in p, "protocol")
req("ThanLongItemConsolidator-v0.6.1.9-win-x64" in w and "verify_v0619_logic.py" in w, "workflow")
req("auto_fight_retry_logic_tests" in cm and "auto_fight_retry_logic_tests.exe" in w, "retry-test-wired")

# Bug proof: after two failed P3 attempts the wait anchor must be separate and stable.
req("DWORD fightRetryWaitTick = 0;" in c, "dedicated-retry-timer")
req("DecideExhaustedRetry" in c and "kImmediateAttemptLimit" in c, "retry-policy-used")
req("chờ chu kỳ check Auto tiếp theo" not in c, "old-infinite-wait-status-removed")
req("fightRetryWaitTick = now" in c and "retry sau " in c and "P3 AUTO RETRY 60s" in c, "stable-retry-wait")
# The exhausted branch must not touch lastAutoFightCheckTick every tick anymore.
m = re.search(r"if \(rt\.fightAttempts >= auto_fight_retry_logic::kImmediateAttemptLimit\)(.*?)if \(rt\.fightPhase == 3\)", c, re.S)
req(m is not None, "exhausted-branch-found")
req("lastAutoFightCheckTick = now" not in m.group(1), "no-periodic-timer-reset-inside-exhausted-wait")

# User requested coordinate monitoring reduction: 180s -> 60s. Auto recheck was already 60s.
req("constexpr DWORD kTrainPositionCheckMs = 60000;" in c, "position-check-60s")
req("constexpr DWORD kAutoFightRecheckMs = 60000;" in c, "auto-recheck-still-60s")
req("kTrainPositionCheckMs = 180000" not in c, "old-position-180s-removed")
req("CHECK 3 PHÚT" not in c and "check tọa độ train 3 phút" not in c, "old-3min-status-removed")

# About tab: large roadmap heading + complete supplied text embedded in scrollable read-only area.
req('L"CÁC TÍNH NĂNG SẮP RA MẮT"' in about_source, "about-roadmap-heading")
req("aboutUpcomingFont_ = CreateFontW(-32" in about_source, "about-roadmap-large-font")
req("ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL" in about_source, "about-roadmap-scrollable")
for snippet in [
    "Các chức năng/ tính năng của AUTO thần long",
    "Các acc được quản lý bởi bộ não ảo thông minh",
    "Cùng nhau đi boss tự phân chia nhiệm vụ",
    "Tính năng PK",
    "Check trạng thái nhân vật theo real time thời gian thực",
    "Rất nhiều tình năng sắp ra mắt. hihi",
]:
    req(snippet in about_source, "about-full-text:" + snippet[:24])

print("PASS: v0.6.1.9 P3 retry deadlock fix + 60s position check + About roadmap audit")
