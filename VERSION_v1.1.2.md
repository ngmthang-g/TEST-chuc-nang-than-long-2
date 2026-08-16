# v1.1.2-test

Runtime fix for Auto trị liệu UI discovery.

The game already opened NPC 339 correctly. This version only fixes the Lua UI root resolver so a visible `GameDialog` is searched via `FindUI` first, then `MainFindUI`, then executor fallback.
