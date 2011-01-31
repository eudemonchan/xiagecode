#include "windows.h"
#include ".\Detours\detours.h"
#include "dream_anticrack.h"
#include ".\SuperHook\VsyAlmightySupHookSDK.h"
#include <stdio.h>
#include <time.h>
//#pragma comment(lib,".\\SuperHook\\VsyAlmightySupHook.lib")

//#include "detours.h"
//#pragma comment(lib,"detours.lib")

#include "LDasm.h"

#define __malloc(_s)    VirtualAlloc(NULL, _s, MEM_COMMIT, PAGE_EXECUTE_READWRITE)
#define __free(_p)        VirtualFree(_p, 0, MEM_RELEASE)
#define JMP_SIZE        5

int WINAPI MyAddFun(int a, int b);
UINT _cdecl MyCreateUnit(UINT player, UINT type, float x, float y, float face);
typedef int (WINAPI* pFun)(int, int);
typedef int (WINAPI* pSub)(int, int);
typedef UINT (_cdecl* fp_CreateUnit)(UINT player, UINT type, float x, float y, float face);

pFun g_pAdd = NULL;
pFun g_realTarget = NULL;
pFun g_realDetour = NULL;

fp_CreateUnit g_gameRealTarget = NULL;
fp_CreateUnit g_gameRealDetour = NULL;
PDETOUR_TRAMPOLINE g_gameDetTramp = NULL;
PDETOUR_TRAMPOLINE g_pDetTramp;

HANDLE g_hThread = NULL;
BOOL g_bRunning;
HANDLE g_hEvent = NULL;

BOOL
WriteReadOnlyMemory(
    void *dest,
    void *src,
    size_t count
    )
/*++
    写只读内存, ring3下就不要锁了
--*/
{
    BOOL                bRet;
    DWORD                dwOldProtect;
    bRet = FALSE;

    // 使前几个字节的内存可写
    if (!VirtualProtect(dest, count, PAGE_READWRITE, &dwOldProtect))
    {
        return bRet;
    }

    memcpy(dest, src, count);

    bRet = VirtualProtect(dest, count, dwOldProtect, &dwOldProtect);

    return    bRet;
}



BOOL 
GetPatchSize(
    IN    void *pfnProc,            /* 需要Hook的函数地址 */
    IN    DWORD dwNeedSize,    /* Hook函数头部占用的字节大小 */
    OUT LPDWORD lpPatchSize    /* 返回根据函数头分析需要修补的大小 */
    )
/*++
    计算函数头需要Patch的大小
--*/
{
    DWORD    Length;
    PUCHAR    pOpcode;
    DWORD    PatchSize = 0;

    if (!pfnProc || !lpPatchSize)
    {
        return FALSE;
    }

    do
    {
        Length = SizeOfCode(pfnProc, &pOpcode);
        if ((Length == 1) && (*pOpcode == 0xC3)) break;
        if ((Length == 3) && (*pOpcode == 0xC2)) break;
        pfnProc = (PVOID)((DWORD)pfnProc + Length);

        PatchSize += Length;
        if (PatchSize >= dwNeedSize)
        {
            break;
        }

    } while (Length);

    *lpPatchSize = PatchSize;

    return    TRUE;
}

BOOL
InlineHook(
    IN    void *pfnOrgProc,            /* 需要Hook的函数地址 */
    IN    void *pfnNewProc,        /* 代替被Hook函数的地址 */
    OUT    void **pfnRealProc        /* 返回原始函数的入口地址 */
    )
/*++
    对函数进行Inline Hook
--*/
{
    DWORD    dwPatchSize;    // 得到需要patch的字节大小
    LPVOID    lpHookFunc;        // 分配的Hook函数的内存
    DWORD    dwBytesNeed;    // 分配的Hook函数的大小
    LPBYTE    lpPatchBuffer;    // jmp 指令的临时缓冲区

    if (!pfnOrgProc || !pfnNewProc || !pfnRealProc)
    {
        return FALSE;
    }
    // 得到需要patch的字节大小
    if (!GetPatchSize(pfnOrgProc, JMP_SIZE, &dwPatchSize))
    {
        return FALSE;
    }

    /*
    0x00000800                    0x00000800        sizeof(DWORD)    // dwPatchSize
    JMP    / FAR 0xAABBCCDD        E9 DDCCBBAA        JMP_SIZE
    ...                            ...                dwPatchSize        // Backup instruction
    JMP    / FAR 0xAABBCCDD        E9 DDCCBBAA        JMP_SIZE
    */

    dwBytesNeed = sizeof(DWORD) + JMP_SIZE + dwPatchSize + JMP_SIZE;

    lpHookFunc = __malloc(dwBytesNeed);

    // 备份dwPatchSize到lpHookFunc
    *(DWORD *)lpHookFunc = dwPatchSize;

    // 跳过开头的4个字节
    lpHookFunc = (LPVOID)((DWORD)lpHookFunc + sizeof(DWORD));


    // 开始backup函数开头的字    
    memcpy((BYTE *)lpHookFunc + JMP_SIZE, pfnOrgProc, dwPatchSize);

    lpPatchBuffer = (LPBYTE)__malloc(dwPatchSize);

    // NOP填充
    memset(lpPatchBuffer, 0x90, dwPatchSize);


    // jmp到Hook
    *(BYTE *)lpHookFunc = 0xE9;
    *(DWORD*)((DWORD)lpHookFunc + 1) = (DWORD)pfnNewProc - (DWORD)lpHookFunc - JMP_SIZE;

    // 跳回原始
    *(BYTE *)((DWORD)lpHookFunc + 5 + dwPatchSize) = 0xE9;
    *(DWORD*)((DWORD)lpHookFunc + 5 + dwPatchSize + 1) = ((DWORD)pfnOrgProc + dwPatchSize) - ((DWORD)lpHookFunc + JMP_SIZE + dwPatchSize) - JMP_SIZE;


    // jmp 
    *(BYTE *)lpPatchBuffer = 0xE9;
    // 注意计算长度的时候得用OrgProc
    *(DWORD*)(lpPatchBuffer + 1) = (DWORD)lpHookFunc - (DWORD)pfnOrgProc - JMP_SIZE;



    WriteReadOnlyMemory(pfnOrgProc, lpPatchBuffer, dwPatchSize);
    
    __free(lpPatchBuffer);


    *pfnRealProc = (void *)((DWORD)lpHookFunc + JMP_SIZE);

    return    TRUE;
}

void UnInlineHook(
    void *pfnOrgProc,        /* 需要恢复Hook的函数地址 */
    void *pfnRealProc    /* 原始函数的入口地址 */
    )
/*++
    恢复对函数进行的Inline Hook
--*/
{
    DWORD    dwPatchSize;
    LPBYTE    lpBuffer;
    
    if (!pfnOrgProc || !pfnRealProc)
    {
        return;
    }

    // 找到分配的空间
    lpBuffer = (LPBYTE)((DWORD)pfnRealProc - (sizeof(DWORD) + JMP_SIZE));
    // 得到dwPatchSize
    dwPatchSize = *(DWORD *)lpBuffer;

    WriteReadOnlyMemory(pfnOrgProc, pfnRealProc, dwPatchSize);

    // 释放分配的跳转函数的空间
    __free(lpBuffer);
}

/*
    jmp far 0008:0000指令的实际效果是设置cs为0x0008，
    设置eip为0x0000，
    这里的0x0008即为保护模式下的段选择符，
    写成二进制形式0000000000001000，前两位00表示特权级0，第三位0表示该选择符用于选择全局描述符表，
    高13位0000000000001表示使用全局描述符的第一项，即内核代码段选择符
*/




ULONG WINAPI GetRnd()
{
	srand((unsigned)time(NULL)*rand()); 
	return rand();
}

PVOID WINAPI _mallocmemory(ULONG size)
{
	return malloc(size);
}

void WINAPI _freecmemory(PVOID buffer)
{
	free(buffer);
}

BOOLEAN WINAPI _protect(PVOID _Address,ULONG _size,ULONG _access)
{
	if (_access&VSP_PAGE_WRITE)
		::VirtualProtectEx(GetCurrentProcess(),_Address,_size,PAGE_READWRITE,0);
	if (_access&VSP_PAGE_DEWRITE)
		::VirtualProtectEx(GetCurrentProcess(),_Address,_size,PAGE_READONLY,0);
	return TRUE;
}
DWORD WINAPI threadProc(LPVOID lParam)
{
	g_bRunning = TRUE;
	while(g_bRunning)
	{
		OutputDebugString(L"11111111111111");
		WaitForSingleObject( g_hEvent, 5000);
	}
	return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{

			//DisableThreadLibraryCalls(hModule);

			//Detours挂钩
			
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			//pFun FuncToDetour = (pFun)0x004117f8;
			fp_CreateUnit FuncToDetour = (fp_CreateUnit)0x6F3C5CB0;
			DetourAttachEx(&(PVOID&)FuncToDetour, MyCreateUnit, &g_gameDetTramp, (PVOID*)&g_gameRealTarget, (PVOID*)&g_gameRealDetour);
			DetourTransactionCommit();

			//dream::loaded_module_hide(hModule);//隐藏dll模块

			//函数地址调用示例
			/*
			pSub FunSub = (pSub)0x004117fd;
			int testabc = FunSub(100,12);
			TCHAR info[MAX_PATH] = {0};
			wsprintf( info, L"%d", testabc);
			OutputDebugString(info);*/

			//超级挂钩
			//SetupMemoryFunction(_mallocmemory,_freecmemory,GetRnd,_protect);
			//LARGE_INTEGER nLimit={5,10};
			//if ( VsyDetourFunction( (PBYTE)MyAddFun, (PBYTE)0x004117f8, (PBYTE*)&g_pAdd, &nLimit))
			//{
			//	OutputDebugString(L"挂钩成功！");
			//}
			//else
			//{
			//	OutputDebugString(L"挂钩失败！");
			//}
			/*InlineHook( (void*)0x004117f8, (void*)MyAddFun, (void**)&g_pAdd );
			TCHAR info[MAX_PATH] = {0};
			wsprintf( info, L"0x%X", (DWORD)g_pAdd);
			OutputDebugString(info);*/
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			/*DetourTransactionBegin();        
			DetourUpdateThread(GetCurrentThread());   
			pFun FuncToDetour = (pFun)0x004117f8;
			DetourDetach((PVOID*)&g_realTarget, (PVOID)g_realDetour);        
			DetourTransactionCommit();*/ 
		/*	if ( g_pAdd != NULL )
			{
				VspRestoreDetours((PBYTE)g_pAdd);
			}*/
			//UnInlineHook( (void*)0x004117f8, (void*)g_pAdd );
		}
		

		break;
	}
	return TRUE;
}

UINT _cdecl MyCreateUnit(UINT player, UINT type, float x, float y, float face)
{
	TCHAR info[MAX_PATH] = {0};
	wsprintf(info, L"player:%d  type:%d  x:0x%08X  y:0x%08X  face:0x%08X", player, type, x, y , face);
	OutputDebugString(info);
	return ((fp_CreateUnit)g_gameDetTramp)(player, type,x,y,face);
}

int WINAPI MyAddFun(int a, int b)
{
	a = 7;
	b = 9;
	return ((pFun)g_pDetTramp)(a,b);
	//return g_pAdd(a,b);
}
