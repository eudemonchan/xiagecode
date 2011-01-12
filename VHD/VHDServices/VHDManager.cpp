#include "StdAfx.h"
#include "VHDManager.h"
#include <assert.h>
#include "shellapi.h"
#include "objbase.h"
//#include "atlcoll.h"

CVHDManager::CVHDManager(void)
{
	m_h = NULL;
}


CVHDManager::~CVHDManager(void)
{
}

DWORD CVHDManager::CreateFixed(PCWSTR path,
                      ULONGLONG size,
                      VIRTUAL_DISK_ACCESS_MASK accessMask,
                      __in_opt PCWSTR source,
                      __in_opt PSECURITY_DESCRIPTOR securityDescriptor,
                      __in_opt OVERLAPPED* overlapped)
 {
        assert(0 == m_h);
        assert(0 != path);
        assert(0 == size % 512);
		

		VIRTUAL_STORAGE_TYPE storageType;
		storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
		/*{
			VIRTUAL_STORAGE_TYPE_DEVICE_VHD,
			VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
		};*/
		//memcpy( &storageType.VendorId, &(VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT), sizeof(GUID));
		CLSIDFromString( L"{EC984AEC-A0F9-47e9-901F-71415A66345B}", &storageType.VendorId);


        CREATE_VIRTUAL_DISK_PARAMETERS parameters =
        {
            CREATE_VIRTUAL_DISK_VERSION_1
        };

		//parameters.

        parameters.Version1.MaximumSize = size;
        parameters.Version1.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
        parameters.Version1.SectorSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE;
        parameters.Version1.SourcePath = source;

        return ::CreateVirtualDisk(&storageType,
                                   path,
                                   accessMask,
                                   securityDescriptor,
                                   CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION,
                                   0, // no provider-specific flags
                                   &parameters,
                                   overlapped,
                                   &m_h);
    }    

DWORD CVHDManager::Open(PCWSTR path,
	VIRTUAL_DISK_ACCESS_MASK accessMask,
	OPEN_VIRTUAL_DISK_FLAG flags, // OPEN_VIRTUAL_DISK_FLAG_NONE
	ULONG readWriteDepth) // OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT
{
	if ( m_h != NULL )
	{
		return ERROR_SUCCESS;
	}
	assert(0 == m_h);
	assert(0 != path);

	VIRTUAL_STORAGE_TYPE storageType;
		storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
		/*{
			VIRTUAL_STORAGE_TYPE_DEVICE_VHD,
			VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
		};*/
		//memcpy( &storageType.VendorId, &(VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT), sizeof(GUID));
		CLSIDFromString( L"{EC984AEC-A0F9-47e9-901F-71415A66345B}", &storageType.VendorId);

	OPEN_VIRTUAL_DISK_PARAMETERS parameters =
	{
		OPEN_VIRTUAL_DISK_VERSION_1
	};

	parameters.Version1.RWDepth = readWriteDepth;

	return ::OpenVirtualDisk(&storageType,
		path,
		accessMask,
		flags,
		&parameters,
		&m_h);
}
DWORD CVHDManager::Attach(ATTACH_VIRTUAL_DISK_FLAG flags,
	__in_opt PSECURITY_DESCRIPTOR securityDescriptor,
	__in_opt OVERLAPPED* overlapped)
{
	assert(0 != m_h);


	return ::AttachVirtualDisk(m_h,
		securityDescriptor,
		flags,
		0, // no provider-specific flags
		0, // no parameters
		overlapped);
}

DWORD CVHDManager::GetSize(__out ULONGLONG& virtualSize,
	__out ULONGLONG& physicalSize,
	__out ULONG& blockSize,
	__out ULONG& sectorSize)
{
	assert(0 != m_h);

	GET_VIRTUAL_DISK_INFO info =
	{
		GET_VIRTUAL_DISK_INFO_SIZE
	};

	ULONG size = sizeof(GET_VIRTUAL_DISK_INFO);

	const DWORD result = ::GetVirtualDiskInformation(m_h,
		&size,
		&info,
		0); // fixed size

	if (ERROR_SUCCESS == result)
	{
		virtualSize = info.Size.VirtualSize;
		physicalSize = info.Size.PhysicalSize;
		blockSize = info.Size.BlockSize;
		sectorSize = info.Size.SectorSize;
	}

	return result;
}

DWORD CVHDManager::Detach( __in     DETACH_VIRTUAL_DISK_FLAG Flags,
	__in     ULONG                    ProviderSpecificFlags)
{
	if ( m_h == NULL )
	{
		return ERROR_SUCCESS;
	}
	DWORD ret = ::DetachVirtualDisk(m_h,Flags, ProviderSpecificFlags);
	m_h = NULL;
	return ret;
}
void CVHDManager::Close()
{
	if ( m_h != NULL )
	{
		CloseHandle(m_h);
		m_h = NULL;
	}
}
//DWORD CVHDManager::GetParentLocation(__out bool& resolved,
//	__out CAtlArray<CString>& paths)
//{
//	assert(0 != m_h);
//
//	GET_VIRTUAL_DISK_INFO info =
//	{
//		GET_VIRTUAL_DISK_INFO_PARENT_LOCATION
//	};
//
//	ULONG size = sizeof(GET_VIRTUAL_DISK_INFO);
//
//	DWORD result = ::GetVirtualDiskInformation(m_h,
//		&size,
//		&info,
//		0); // not used
//
//	if (ERROR_INSUFFICIENT_BUFFER != result)
//	{
//		return result;
//	}
//
//	CAtlArray<BYTE> buffer;
//
//	if (!buffer.SetCount(size))
//	{
//		return ERROR_NOT_ENOUGH_MEMORY;
//	}
//
//	GET_VIRTUAL_DISK_INFO* pInfo = reinterpret_cast<GET_VIRTUAL_DISK_    INFO*>(buffer.GetData());
//	pInfo->Version = GET_VIRTUAL_DISK_INFO_PARENT_LOCATION;
//
//	result = ::GetVirtualDiskInformation(m_h,
//		&size,
//		pInfo,
//		0); // not used
//
//	if (ERROR_SUCCESS == result)
//	{
//		resolved = 0 != pInfo->ParentLocation.ParentResolved;
//		PCWSTR path = pInfo->ParentLocation.ParentLocationBuffer;
//
//		while (0 != *path)
//		{
//			paths.Add(path);
//
//			path += paths[paths.GetCount() - 1].GetLength() + 1;
//		}
//	}
//
//	return result;
//}


