#ifndef _UTILITY_H
#define _UTILITY_H


ULONG
WcsToStr(
         LPCWSTR  pwzInString,
         LPSTR    pszOutString,
         ULONG    cbOutStringSize
         );

ULONG
StrToWcs(
         LPCSTR  pszInString,
         LPWSTR  pwzOutString,
         ULONG   cbOutStringSize
         );

LPTSTR GetModuleDir(IN HMODULE hModule, OUT LPTSTR lpModuleDir, IN DWORD cchSize);
LPTSTR GetAppDir(OUT LPTSTR lpAppDir, IN DWORD cchSize);


#endif