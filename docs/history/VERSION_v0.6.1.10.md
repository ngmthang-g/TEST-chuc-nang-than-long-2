# v0.6.1.10 — About roadmap bullet/CRLF formatting

## User request
Tab Giới thiệu phải xuống dòng rõ từng gạch đầu dòng; bản v0.6.1.9 hiển thị roadmap nối tiếp khó đọc.

## Root cause
Nội dung roadmap dùng LF trong raw string. Win32 multiline EDIT cần CRLF để hiển thị line break ổn định.

## Change
- Runtime logic giữ nguyên v0.6.1.9.
- Chỉ đổi chuỗi `kUpcomingFeaturesText` sang các literal có `\r\n`.
- Mỗi nhóm 1–5 có khoảng trắng riêng; mọi gạch đầu dòng nằm trên dòng riêng.
- Nội dung nhìn thấy giữ nguyên, chỉ thay bố cục.

## Validation
- `tools/verify_v06110_logic.py` kiểm version/protocol, giữ toàn bộ invariant v0.6.1.9 và bắt buộc CRLF/bullets/blank lines.
- Windows CI run #444 (`32452534680`): PASS verifier, MSVC x64 Release, all existing native tests including AutoFight retry timer, stage and artifact upload. Artifact ID `9436134827`.
