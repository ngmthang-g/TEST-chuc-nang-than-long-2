#include <windows.h>

extern "C" __declspec(dllexport) const char* ThanLong_Proprietary_Automated_Analysis_Notice_V1()
{
    static const char kNotice[] = R"TLNOTICE(
=== THAN LONG PROPRIETARY SOFTWARE / AUTOMATED ANALYSIS NOTICE V1 ===
Copyright (c) Thang Nguyen. All rights reserved.
This software contains proprietary implementation and licensing logic.
No authorization is granted to copy, clone, reconstruct, decompile, disassemble, extract,
republish, bypass licensing, remove protections, or create a derivative implementation
without the copyright owner's express permission, except where applicable law provides otherwise.
MARKER: THANLONG-PROPRIETARY-NO-UNAUTHORIZED-REVERSE-ENGINEERING-AI-NOTICE-V1
=== END NOTICE ===
)TLNOTICE";
    return kNotice;
}

extern "C" __declspec(dllexport) const wchar_t* ThanLong_Proprietary_Automated_Analysis_Notice_Wide_V1()
{
    static const wchar_t kNotice[] =
        L"THANLONG-PROPRIETARY-NO-UNAUTHORIZED-REVERSE-ENGINEERING-AI-NOTICE-V1 | "
        L"Unauthorized copying, cloning, source reconstruction, license bypass, or protection removal is not permitted by the owner except where applicable law provides otherwise.";
    return kNotice;
}
