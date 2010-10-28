#include "stdafx.h"

#include <assert.h>

ULONG
WcsToStr(
         LPCWSTR  pwzInString,
         LPSTR    pszOutString,
         ULONG    cbOutStringSize
         )
{
    assert(pwzInString);
    if (!pwzInString || !pszOutString || !cbOutStringSize)
        return 0;

    ZeroMemory(pszOutString, cbOutStringSize);
    int nRlt = WideCharToMultiByte(CP_ACP,  // CodePage
        0,                   // dwFlags
        pwzInString,         // lpWideCharStr
        wcslen(pwzInString), // cchWideChar
        pszOutString,        // lpMultiByteStr
        cbOutStringSize,     // cbMultiByte
        NULL,                // lpDefaultChar
        NULL);               // lpUsedDefaultChar
    if (!nRlt)              
    {
        DWORD err = GetLastError();
    }

    return nRlt;
}

ULONG
StrToWcs(
         LPCSTR  pszInString,
         LPWSTR  pwzOutString,
         ULONG   cbOutStringSize
         )
{
    assert(pszInString);
    if (!pszInString || !pwzOutString || !cbOutStringSize)
        return 0;

    ZeroMemory(pwzOutString, cbOutStringSize);
    int nRlt = MultiByteToWideChar(CP_ACP,  // CodePage
        MB_USEGLYPHCHARS,                   // character-type options
        pszInString,                        // lpWideCharStr
        sizeof(char) * strlen(pszInString), // cchWideChar
        pwzOutString,                       // lpMultiByteStr
        cbOutStringSize / sizeof(WCHAR));     // cbMultiByte
    if (!nRlt)      
    {
        DWORD err = GetLastError();
    }

    return nRlt;
}

LPTSTR GetModuleDir(IN HMODULE hModule, OUT LPTSTR lpModuleDir, IN DWORD cchSize)
{
    ::GetModuleFileName(hModule, lpModuleDir, cchSize);

    LPTSTR lpStr = _tcsrchr(lpModuleDir, _T('\\'));
    if (lpStr) {
        lpStr[0] = _T('\0');
    }

    return lpModuleDir;
}


LPTSTR GetAppDir(OUT LPTSTR lpAppDir, IN DWORD cchSize)
{
    return GetModuleDir(NULL, lpAppDir, cchSize);
}
