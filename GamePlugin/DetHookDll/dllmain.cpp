// dllmain.cpp : 定义 DLL 应用程序的入口点。
//#include "stdafx.h"
#include "windows.h"
#include "..\Detours\detours.h"
#include "dream_anticrack.h"


//#include "detours.h"
//#pragma comment(lib,"detours.lib")

int WINAPI MyAddFun(int a, int b);
typedef int (WINAPI* pFun)(int, int);
typedef int (WINAPI* pSub)(int, int);

//DETOUR_TRAMPOLINE(int WINAPI Real_AddFun(int a, int b), (pFun)0x004117f8);

//DETOUR_TRAMPOLINE( int WINAPI Real_AddFun( int a, int b),
//				  CreateProcessA);

HANDLE g_hThread = NULL;
BOOL g_bRunning;
HANDLE g_hEvent = NULL;



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
	/*	g_hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
		if ( g_hEvent != NULL )
		{
			g_hThread = ::CreateThread( NULL, 0, threadProc, 0, 0, NULL );
		}*/
		{

			::MessageBox(NULL,L"dasdf",L"cc",0);
			//DisableThreadLibraryCalls(hModule);
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			pFun FuncToDetour = (pFun)0x004117f8;
			DetourAttach(&(PVOID&)FuncToDetour, MyAddFun);

			DetourTransactionCommit();
			//dream::loaded_module_hide(hModule);
			pSub FunSub = (pSub)0x004117fd;
			int testabc = FunSub(100,12);
			TCHAR info[MAX_PATH] = {0};
			wsprintf( info, L"%d", testabc);
			MessageBox(NULL,info,L"dd",0);

		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	/*	if ( g_hThread != NULL )
		{

			g_bRunning = FALSE;
			SetEvent(g_hEvent);
			WaitForSingleObject(g_hThread, 1000);
			CloseHandle(g_hThread);
			CloseHandle(g_hEvent);
		}*/
		{
			DetourTransactionBegin();        
			DetourUpdateThread(GetCurrentThread());   
			pFun FuncToDetour = (pFun)0x004117f8;
			DetourDetach(&(PVOID&)FuncToDetour, MyAddFun);        
			DetourTransactionCommit(); 
		}
		

		break;
	}
	return TRUE;
}

int WINAPI MyAddFun(int a, int b)
{
	return 0;
}
//void loaded_module_hide(HMODULE module_handle)//{//	DWORD p_peb_ldr_data = 0;////	__asm//	{//		pushad;//		pushfd;//		mov eax, fs:[30h];			// PEB//		mov eax, [eax+0Ch];			// PEB->ProcessModuleInfo//		mov p_peb_ldr_data, eax;	// Save ProcessModuleInfo////in_load_order_module_list://		mov esi, [eax+0Ch];			// ProcessModuleInfo->in_load_order_module_list[FORWARD]//		mov edx, [eax+10h];			//  ProcessModuleInfo->in_load_order_module_list[BACKWARD]////loop_in_load_order_module_list: //		lodsd;						//  Load First Module//		mov esi, eax;		    	//  ESI points to Next Module//		mov ecx, [eax+18h];			//  LDR_MODULE->BaseAddress//		cmp ecx, module_handle;		//  Is it Our Module ?//		jne skip_a;		    		//  If Not, Next Please (@f jumps to nearest Unamed Lable @@:)//		mov ebx, [eax];				//  [FORWARD] Module //		mov ecx, [eax+4];    		//  [BACKWARD] Module//		mov [ecx], ebx;				//  Previous Module's [FORWARD] Notation, Points to us, Replace it with, Module++//		mov [ebx+4], ecx;			//  Next Modules, [BACKWARD] Notation, Points to us, Replace it with, Module--//		jmp in_memory_order_module_list;		//  Hidden, so Move onto Next Set//skip_a://		cmp edx, esi;							//  Reached End of Modules ?//		jne loop_in_load_order_module_list;		//  If Not, Re Loop////in_memory_order_module_list://		mov eax, p_peb_ldr_data;	  //  PEB->ProcessModuleInfo//		mov esi, [eax+14h];			  //  ProcessModuleInfo->in_memory_order_module_list[START]//		mov edx, [eax+18h];			  //  ProcessModuleInfo->in_memory_order_module_list[FINISH]////loop_in_memory_order_module_list: //		lodsd;//		mov esi, eax;//		mov ecx, [eax+10h];//		cmp ecx, module_handle;//		jne skip_b;//		mov ebx, [eax]; //		mov ecx, [eax+4];//		mov [ecx], ebx;//		mov [ebx+4], ecx;//		jmp in_initialization_order_module_list;//skip_b://		cmp edx, esi;//		jne loop_in_memory_order_module_list;////in_initialization_order_module_list://		mov eax, p_peb_ldr_data;		//  PEB->ProcessModuleInfo//		mov esi, [eax+1Ch];				//  ProcessModuleInfo->in_initialization_order_module_list[START]//		mov edx, [eax+20h];				//  ProcessModuleInfo->in_initialization_order_module_list[FINISH]////loop_in_initialization_order_module_list: //		lodsd;//		mov esi, eax;		//		mov ecx, [eax+08h];//		cmp ecx, module_handle;		//		jne skip_c;//		mov ebx, [eax]; //		mov ecx, [eax+4];//		mov [ecx], ebx;//		mov [ebx+4], ecx;//		jmp Finished;//skip_c://		cmp edx, esi;//		jne loop_in_initialization_order_module_list;////Finished://		popfd;//		popad;//	}//}