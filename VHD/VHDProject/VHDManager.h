#pragma once
#include "windows.h"
#include "VirtDisk.h"
#pragma comment(lib,"VirtDisk.lib")
class CVHDManager
{
private:	
	DWORD CreateFixed(PCWSTR path,
				ULONGLONG size,
				VIRTUAL_DISK_ACCESS_MASK accessMask,
				__in_opt PCWSTR source,
				__in_opt PSECURITY_DESCRIPTOR securityDescriptor,
				__in_opt OVERLAPPED* overlapped);
public:
	CVHDManager(void);
	~CVHDManager(void);

	DWORD CreateFixedAsync( PCWSTR path, ULONGLONG size);
	DWORD CreateFixedSync( PCWSTR path, ULONGLONG size);
	DWORD Open(PCWSTR path,
		VIRTUAL_DISK_ACCESS_MASK accessMask =VIRTUAL_DISK_ACCESS_ATTACH_RW,
		OPEN_VIRTUAL_DISK_FLAG flags =OPEN_VIRTUAL_DISK_FLAG_NONE, // OPEN_VIRTUAL_DISK_FLAG_NONE
		ULONG readWriteDepth = OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT); // OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT
	DWORD Attach(ATTACH_VIRTUAL_DISK_FLAG flags=ATTACH_VIRTUAL_DISK_FLAG_NONE,
		__in_opt PSECURITY_DESCRIPTOR securityDescriptor=NULL,
		__in_opt OVERLAPPED* overlapped=NULL);
	DWORD GetSize(__out ULONGLONG& virtualSize,
		__out ULONGLONG& physicalSize,
		__out ULONG& blockSize,
		__out ULONG& sectorSize);
	DWORD  Detach( __in     DETACH_VIRTUAL_DISK_FLAG Flags =DETACH_VIRTUAL_DISK_FLAG_NONE,
		__in     ULONG                    ProviderSpecificFlags=0);
	void Close();

	//DWORD GetParentLocation(__out bool& resolved,
	//	__out CAtlArray<CString>& paths);
	HANDLE m_h;
	ULONGLONG GetTotalSize();
	ULONGLONG GetCurrentSize();
	DWORD GetLastErrorCode();
private:
	OVERLAPPED m_ol;
	DWORD m_dwLastError;
};

