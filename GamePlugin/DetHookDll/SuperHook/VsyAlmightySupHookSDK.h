// By CPMorn.dxmer  E_Mail :3.vc@163.com  WIN32版超级HOOK SDK

// 2010年3月

// 为WIN32应用程序提供类似MS Detour的函数库

// 高效稳定、安全性能高

// 仅提供WIN32使用 需要RING0版本此库 请联系洽谈

// 此版本不提供Detour恢复函数

// 有BUG情按照以上E_mail联系我

// 感谢使用 !更多精彩软件和算法 请到 http://www.vsysv.com !

#ifndef _VSYALMIGHTYSUPHOOKSDK_
#define _VSYALMIGHTYSUPHOOKSDK_


#ifdef WINAPI

#else
#define WINAPI __stdcall
#endif
#ifdef PBYTE

#else
#define PBYTE  unsigned char*
#endif

#define VSP_PAGE_READ (ULONG)0x1
#define VSP_PAGE_WRITE (ULONG)0x2
#define VSP_PAGE_READWRITE (VSP_PAGE_READ|VSP_PAGE_WRITE)

#define VSP_PAGE_DEREAD (ULONG)0x4
#define VSP_PAGE_DEWRITE (ULONG)0x8
#define VSP_PAGE_DEREADWRITE (VSP_PAGE_DEREAD|VSP_PAGE_DEWRITE)

typedef PVOID (WINAPI* pAllocateMemory)(ULONG _Size);
typedef VOID (WINAPI* pReleaseMemory)(PVOID _Address);
typedef ULONG (WINAPI* pRndNumber)();
typedef BOOLEAN (WINAPI* pProtectMemory)(PVOID _Address,ULONG _size,ULONG _access);
#define CODE_POSTION_CENTER (long)0x87654321

#ifdef __cplusplus
extern "C" 
{
#endif
BOOLEAN 
WINAPI 
SetupMemoryFunction(
					pAllocateMemory _pAllocateMemory,
					pReleaseMemory _pReleaseMemory,
					pRndNumber _RndProc,
					pProtectMemory _MemoryProtect);
/*
函数说明：设置内存分配、释放，随机函数获取，内存保护函数获取以便在执行过程中完成调用.
*/
BOOLEAN WINAPI 
VsyDetourFunction(PBYTE bTarget /* 自己的接管函数 */,
						PBYTE bDetour/*被HOOK的函数*/,
                  PBYTE *pbTrampoline,
                  LARGE_INTEGER *Limit);
                  
BOOLEAN WINAPI 
VspDetourFunction(PBYTE bTarget /* 自己的接管函数 */,
						PBYTE bDetour/*被HOOK的函数*/,
						PBYTE *pbTrampoline/* 真实的调用地址 */,
                  PBYTE pbTrapAddress,int loop/*跳板的数量*/,
						BOOLEAN Junkcode/*使用花指令*/,
                  ULONG Seed/*花指令的种子*/,
						int nEntry/*指定的入口样式*/,
                  long * Postion/*指定HOOK的位置*/,
                  long *OutSize/*输出的长度*/,
                  BOOLEAN Force,
                  LARGE_INTEGER * Limit,
                  BOOLEAN IsStartDetour);
/*
VspDetourFunction 函数说明:
bTarget  自己的接管函数
bDetour  被HOOK的函数
pbTrampoline 返回真实的调用地址
loop 设置多级跳板的数量
Junkcode 是否加花指令
Seed 花指令种子 1-6
nEntry 函数跳转的入口样式 1-6
Postion 指定在原函数的那个位置跳转 若指定的地址在指令中部 则修正为小于指定位置的最大正确跳转位置  返回修正后地址
Force 是否强制执行跳转 当指定的位置无效或者过小  将自动以最小跳转距离继续执行操作
返回值:TRUE DETOUR成功 FALSE 失败
*/
PBYTE WINAPI 
VspDetourCopy(PBYTE * bTarget,
				  PBYTE lAddress,
              PBYTE bDetour,
              PBYTE pAddress,
              long *pSize,
              long *OutSize,
              LARGE_INTEGER * Limit);
/*
VspDetourCopy 函数说明:
复制内存中的函数
bTarget 复制到的地址 传入未分配内存的指针  返回已复制的函数地址的指针地址
bDetour 要进行复制的函数
pSize   复制多少字节  若指定的地址在指令中部 则修正为小于指定位置的最大正确跳转位置  返回修正后地址 -1全部复制
*/
PBYTE WINAPI 
VspDetourCopyWithSubLink(PBYTE * bTarget,
								 PBYTE lAddress,
                         PBYTE bDetour,
                         PBYTE pAddress,
                         int Level,
                         ULONG * MemoryMode,
                         LARGE_INTEGER *Limit);
/*
VspDetourCopy 函数说明:
复制内存中的函数以及其调用到的函数链
bTarget 复制到的地址 传入未分配内存的指针  返回已复制的函数地址的指针地址
bDetour 要进行复制的函数
pSize   复制多少字节  若指定的地址在指令中部 则修正为小于指定位置的最大正确跳转位置  返回修正后地址 -1全部复制
Level  使用深度复制
Mode  重建内存模式 0，分布式重建 .1，连续模式重建
 */


BOOLEAN WINAPI ReleaseDetourCopy(PBYTE * Address);
/*
释放复制的函数及其函数链 传入已复制的函数地址指针
  */

PVOID WINAPI ReleaseRecord();
/*
释放所有被函数 传入已复制的函数地址指针 
*/

/*
此版本体不提供 Detour恢复 函数
  */
PBYTE WINAPI 
AllocateMemoryInSpace(PBYTE bTarget/*目标地址*/,
												PBYTE zBuffer,
                                    PULONG pOffset/*返回的偏移量*/,
                                    PULONG pSize/*要分配的长度和返回实际长度*/,
                                    PULONG pOutSize,
                                    BOOLEAN bForce,
                                    BOOLEAN bEnable);
PBYTE WINAPI 
AllocateMemoryInSpaceEx(PBYTE bFakeProc,
												 PBYTE bTarget/*目标地址*/,
                                     PBYTE zBuffer,
                                     PULONG pOffset/*返回的偏移量*/,
                                     PULONG pSize/*要分配的长度和返回实际长度*/,
                                     PULONG pOutSize,
                                     long loop,
                                     int nSeed,
                                     BOOLEAN bForce,
                                     BOOLEAN bEnable);
VOID WINAPI 
DeallocateMemoryInSpace(PBYTE * bDetour);
BOOLEAN WINAPI 
VspRemoveDetours(PBYTE pbCode,BOOLEAN bRelease);
BOOLEAN WINAPI 
VspCheckDetours(PBYTE pbCode);
BOOLEAN WINAPI 
VspRestoreDetours(PBYTE pbCode);
ULONG WINAPI 
GetCopyedSafePosition(PBYTE bDetour,PULONG pCopyedSize);
#ifdef __cplusplus
}
#endif
#endif