#pragma once
#include "windows.h"
#include "VirtDisk.h"
#include "initguid.h"
#include "vds.h"
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"VirtDisk.lib")
class CVHDManager
{
private:	
	BOOL CreateFixed(PCWSTR path,
				ULONGLONG size,
				VIRTUAL_DISK_ACCESS_MASK accessMask,
				__in_opt PCWSTR source,
				__in_opt PSECURITY_DESCRIPTOR securityDescriptor,
				__in_opt OVERLAPPED* overlapped);
public:
	CVHDManager(void);
	~CVHDManager(void);

	BOOL CreateFixedAsync( PCWSTR path, ULONGLONG size);
	BOOL CreateFixedSync( PCWSTR path, ULONGLONG size);
	BOOL Open(PCWSTR path,
		VIRTUAL_DISK_ACCESS_MASK accessMask =VIRTUAL_DISK_ACCESS_ALL,
		OPEN_VIRTUAL_DISK_FLAG flags =OPEN_VIRTUAL_DISK_FLAG_NONE, // OPEN_VIRTUAL_DISK_FLAG_NONE
		ULONG readWriteDepth = OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT); // OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT
	BOOL Attach(ATTACH_VIRTUAL_DISK_FLAG flags=ATTACH_VIRTUAL_DISK_FLAG_NONE,
		__in_opt PSECURITY_DESCRIPTOR securityDescriptor=NULL,
		__in_opt OVERLAPPED* overlapped=NULL);
	BOOL GetSize(__out ULONGLONG& virtualSize,
		__out ULONGLONG& physicalSize,
		__out ULONG& blockSize,
		__out ULONG& sectorSize);
	BOOL Detach( __in     DETACH_VIRTUAL_DISK_FLAG Flags =DETACH_VIRTUAL_DISK_FLAG_NONE,
		__in     ULONG                    ProviderSpecificFlags=0);
	void Close();
	ULONG QuerySize();

	//DWORD GetParentLocation(__out bool& resolved,
	//	__out CAtlArray<CString>& paths);
	HANDLE m_h;
	DWORD GetLastErrorCode();

	BOOL GetProcessState(VIRTUAL_DISK_PROGRESS *pProgress);

	BOOL ConnectDiskService();
	BOOL SetMultipleInterface();
	BOOL SetVolInterface();
	BOOL MountDisk(LPCWSTR strDiskPath,BOOL bAttatch = TRUE);
	BOOL UnMountDisk();
	void ReleaseService();
	BOOL InitDisk();
	BOOL CreatePartition(ULONGLONG partSize);
	BOOL GetVolumeName(WCHAR *pVolName, ULONG nLen);
	int GetPartitionCount();
	BOOL FormatDisk();
	BOOL AddLetter(WCHAR letter);
	BOOL FormatDisk(LPCWSTR strDiskName);
	BOOL SetVdsPackInterface();
	WCHAR GetDiskLetter();
	IVdsService *m_pVdsService;
	IVdsVdProvider *m_pVdsVdProvider;
	IVdsOpenVDisk *m_pOpenVdisk;
	IVdsDisk *m_pVdsDisk;
	IVdsVDisk *m_pVdsVirDisk;
	IVdsPack *m_pPack;
	IVdsAdvancedDisk *m_pAdvDisk;
	IVdsVolume *m_pVolume;
	IVdsVolumeMF *m_pVolMF;
	ULONGLONG m_realStartPos;

private:
	OVERLAPPED m_ol;
	DWORD m_dwLastError;
};

