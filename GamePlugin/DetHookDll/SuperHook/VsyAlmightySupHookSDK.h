// By CPMorn.dxmer  E_Mail :3.vc@163.com  WIN32�泬��HOOK SDK

// 2010��3��

// ΪWIN32Ӧ�ó����ṩ����MS Detour�ĺ�����

// ��Ч�ȶ�����ȫ���ܸ�

// ���ṩWIN32ʹ�� ��ҪRING0�汾�˿� ����ϵǢ̸

// �˰汾���ṩDetour�ָ�����

// ��BUG�鰴������E_mail��ϵ��

// ��лʹ�� !���ྫ��������㷨 �뵽 http://www.vsysv.com !

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
����˵���������ڴ���䡢�ͷţ����������ȡ���ڴ汣��������ȡ�Ա���ִ�й�������ɵ���.
*/
BOOLEAN WINAPI 
VsyDetourFunction(PBYTE bTarget /* �Լ��Ľӹܺ��� */,
						PBYTE bDetour/*��HOOK�ĺ���*/,
                  PBYTE *pbTrampoline,
                  LARGE_INTEGER *Limit);
                  
BOOLEAN WINAPI 
VspDetourFunction(PBYTE bTarget /* �Լ��Ľӹܺ��� */,
						PBYTE bDetour/*��HOOK�ĺ���*/,
						PBYTE *pbTrampoline/* ��ʵ�ĵ��õ�ַ */,
                  PBYTE pbTrapAddress,int loop/*���������*/,
						BOOLEAN Junkcode/*ʹ�û�ָ��*/,
                  ULONG Seed/*��ָ�������*/,
						int nEntry/*ָ���������ʽ*/,
                  long * Postion/*ָ��HOOK��λ��*/,
                  long *OutSize/*����ĳ���*/,
                  BOOLEAN Force,
                  LARGE_INTEGER * Limit,
                  BOOLEAN IsStartDetour);
/*
VspDetourFunction ����˵��:
bTarget  �Լ��Ľӹܺ���
bDetour  ��HOOK�ĺ���
pbTrampoline ������ʵ�ĵ��õ�ַ
loop ���ö༶���������
Junkcode �Ƿ�ӻ�ָ��
Seed ��ָ������ 1-6
nEntry ������ת�������ʽ 1-6
Postion ָ����ԭ�������Ǹ�λ����ת ��ָ���ĵ�ַ��ָ���в� ������ΪС��ָ��λ�õ������ȷ��תλ��  �����������ַ
Force �Ƿ�ǿ��ִ����ת ��ָ����λ����Ч���߹�С  ���Զ�����С��ת�������ִ�в���
����ֵ:TRUE DETOUR�ɹ� FALSE ʧ��
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
VspDetourCopy ����˵��:
�����ڴ��еĺ���
bTarget ���Ƶ��ĵ�ַ ����δ�����ڴ��ָ��  �����Ѹ��Ƶĺ�����ַ��ָ���ַ
bDetour Ҫ���и��Ƶĺ���
pSize   ���ƶ����ֽ�  ��ָ���ĵ�ַ��ָ���в� ������ΪС��ָ��λ�õ������ȷ��תλ��  �����������ַ -1ȫ������
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
VspDetourCopy ����˵��:
�����ڴ��еĺ����Լ�����õ��ĺ�����
bTarget ���Ƶ��ĵ�ַ ����δ�����ڴ��ָ��  �����Ѹ��Ƶĺ�����ַ��ָ���ַ
bDetour Ҫ���и��Ƶĺ���
pSize   ���ƶ����ֽ�  ��ָ���ĵ�ַ��ָ���в� ������ΪС��ָ��λ�õ������ȷ��תλ��  �����������ַ -1ȫ������
Level  ʹ����ȸ���
Mode  �ؽ��ڴ�ģʽ 0���ֲ�ʽ�ؽ� .1������ģʽ�ؽ�
 */


BOOLEAN WINAPI ReleaseDetourCopy(PBYTE * Address);
/*
�ͷŸ��Ƶĺ������亯���� �����Ѹ��Ƶĺ�����ַָ��
  */

PVOID WINAPI ReleaseRecord();
/*
�ͷ����б����� �����Ѹ��Ƶĺ�����ַָ�� 
*/

/*
�˰汾�岻�ṩ Detour�ָ� ����
  */
PBYTE WINAPI 
AllocateMemoryInSpace(PBYTE bTarget/*Ŀ���ַ*/,
												PBYTE zBuffer,
                                    PULONG pOffset/*���ص�ƫ����*/,
                                    PULONG pSize/*Ҫ����ĳ��Ⱥͷ���ʵ�ʳ���*/,
                                    PULONG pOutSize,
                                    BOOLEAN bForce,
                                    BOOLEAN bEnable);
PBYTE WINAPI 
AllocateMemoryInSpaceEx(PBYTE bFakeProc,
												 PBYTE bTarget/*Ŀ���ַ*/,
                                     PBYTE zBuffer,
                                     PULONG pOffset/*���ص�ƫ����*/,
                                     PULONG pSize/*Ҫ����ĳ��Ⱥͷ���ʵ�ʳ���*/,
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