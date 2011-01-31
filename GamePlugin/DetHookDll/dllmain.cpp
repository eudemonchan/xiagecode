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
    дֻ���ڴ�, ring3�¾Ͳ�Ҫ����
--*/
{
    BOOL                bRet;
    DWORD                dwOldProtect;
    bRet = FALSE;

    // ʹǰ�����ֽڵ��ڴ��д
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
    IN    void *pfnProc,            /* ��ҪHook�ĺ�����ַ */
    IN    DWORD dwNeedSize,    /* Hook����ͷ��ռ�õ��ֽڴ�С */
    OUT LPDWORD lpPatchSize    /* ���ظ��ݺ���ͷ������Ҫ�޲��Ĵ�С */
    )
/*++
    ���㺯��ͷ��ҪPatch�Ĵ�С
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
    IN    void *pfnOrgProc,            /* ��ҪHook�ĺ�����ַ */
    IN    void *pfnNewProc,        /* ���汻Hook�����ĵ�ַ */
    OUT    void **pfnRealProc        /* ����ԭʼ��������ڵ�ַ */
    )
/*++
    �Ժ�������Inline Hook
--*/
{
    DWORD    dwPatchSize;    // �õ���Ҫpatch���ֽڴ�С
    LPVOID    lpHookFunc;        // �����Hook�������ڴ�
    DWORD    dwBytesNeed;    // �����Hook�����Ĵ�С
    LPBYTE    lpPatchBuffer;    // jmp ָ�����ʱ������

    if (!pfnOrgProc || !pfnNewProc || !pfnRealProc)
    {
        return FALSE;
    }
    // �õ���Ҫpatch���ֽڴ�С
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

    // ����dwPatchSize��lpHookFunc
    *(DWORD *)lpHookFunc = dwPatchSize;

    // ������ͷ��4���ֽ�
    lpHookFunc = (LPVOID)((DWORD)lpHookFunc + sizeof(DWORD));


    // ��ʼbackup������ͷ����    
    memcpy((BYTE *)lpHookFunc + JMP_SIZE, pfnOrgProc, dwPatchSize);

    lpPatchBuffer = (LPBYTE)__malloc(dwPatchSize);

    // NOP���
    memset(lpPatchBuffer, 0x90, dwPatchSize);


    // jmp��Hook
    *(BYTE *)lpHookFunc = 0xE9;
    *(DWORD*)((DWORD)lpHookFunc + 1) = (DWORD)pfnNewProc - (DWORD)lpHookFunc - JMP_SIZE;

    // ����ԭʼ
    *(BYTE *)((DWORD)lpHookFunc + 5 + dwPatchSize) = 0xE9;
    *(DWORD*)((DWORD)lpHookFunc + 5 + dwPatchSize + 1) = ((DWORD)pfnOrgProc + dwPatchSize) - ((DWORD)lpHookFunc + JMP_SIZE + dwPatchSize) - JMP_SIZE;


    // jmp 
    *(BYTE *)lpPatchBuffer = 0xE9;
    // ע����㳤�ȵ�ʱ�����OrgProc
    *(DWORD*)(lpPatchBuffer + 1) = (DWORD)lpHookFunc - (DWORD)pfnOrgProc - JMP_SIZE;



    WriteReadOnlyMemory(pfnOrgProc, lpPatchBuffer, dwPatchSize);
    
    __free(lpPatchBuffer);


    *pfnRealProc = (void *)((DWORD)lpHookFunc + JMP_SIZE);

    return    TRUE;
}

void UnInlineHook(
    void *pfnOrgProc,        /* ��Ҫ�ָ�Hook�ĺ�����ַ */
    void *pfnRealProc    /* ԭʼ��������ڵ�ַ */
    )
/*++
    �ָ��Ժ������е�Inline Hook
--*/
{
    DWORD    dwPatchSize;
    LPBYTE    lpBuffer;
    
    if (!pfnOrgProc || !pfnRealProc)
    {
        return;
    }

    // �ҵ�����Ŀռ�
    lpBuffer = (LPBYTE)((DWORD)pfnRealProc - (sizeof(DWORD) + JMP_SIZE));
    // �õ�dwPatchSize
    dwPatchSize = *(DWORD *)lpBuffer;

    WriteReadOnlyMemory(pfnOrgProc, pfnRealProc, dwPatchSize);

    // �ͷŷ������ת�����Ŀռ�
    __free(lpBuffer);
}

/*
    jmp far 0008:0000ָ���ʵ��Ч��������csΪ0x0008��
    ����eipΪ0x0000��
    �����0x0008��Ϊ����ģʽ�µĶ�ѡ�����
    д�ɶ�������ʽ0000000000001000��ǰ��λ00��ʾ��Ȩ��0������λ0��ʾ��ѡ�������ѡ��ȫ����������
    ��13λ0000000000001��ʾʹ��ȫ���������ĵ�һ����ں˴����ѡ���
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

			//Detours�ҹ�
			
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			//pFun FuncToDetour = (pFun)0x004117f8;
			fp_CreateUnit FuncToDetour = (fp_CreateUnit)0x6F3C5CB0;
			DetourAttachEx(&(PVOID&)FuncToDetour, MyCreateUnit, &g_gameDetTramp, (PVOID*)&g_gameRealTarget, (PVOID*)&g_gameRealDetour);
			DetourTransactionCommit();

			//dream::loaded_module_hide(hModule);//����dllģ��

			//������ַ����ʾ��
			/*
			pSub FunSub = (pSub)0x004117fd;
			int testabc = FunSub(100,12);
			TCHAR info[MAX_PATH] = {0};
			wsprintf( info, L"%d", testabc);
			OutputDebugString(info);*/

			//�����ҹ�
			//SetupMemoryFunction(_mallocmemory,_freecmemory,GetRnd,_protect);
			//LARGE_INTEGER nLimit={5,10};
			//if ( VsyDetourFunction( (PBYTE)MyAddFun, (PBYTE)0x004117f8, (PBYTE*)&g_pAdd, &nLimit))
			//{
			//	OutputDebugString(L"�ҹ��ɹ���");
			//}
			//else
			//{
			//	OutputDebugString(L"�ҹ�ʧ�ܣ�");
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
