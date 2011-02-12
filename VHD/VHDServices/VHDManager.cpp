#include "StdAfx.h"
#include "VHDManager.h"
#include <assert.h>
#include "shellapi.h"
#include "objbase.h"
#include "WinIoCtl.h."


#define _SafeRelease(x) {if (NULL != x) { x->Release(); x = NULL; } }
//#include "atlcoll.h"

CVHDManager::CVHDManager(void)
{
	m_h = NULL;
	m_dwLastError = 0;
	m_pVdsService = NULL;
	m_pOpenVdisk = NULL;
	m_pVdsDisk = NULL;
	m_pVdsVirDisk = NULL;
	m_pAdvDisk = NULL;
	m_pVdsVdProvider = NULL;
	m_pPack = NULL;
	m_pVolume = NULL;
	m_pVolMF = NULL;
	m_realStartPos = 0;
}


CVHDManager::~CVHDManager(void)
{
}

BOOL CVHDManager::CreateFixed(PCWSTR path,
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

        if( ERROR_SUCCESS != ::CreateVirtualDisk(&storageType,
                                   path,
                                   accessMask,
                                   securityDescriptor,
                                   CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION,
                                   0, // no provider-specific flags
                                   &parameters,
                                   overlapped,
                                   &m_h) )
		{
			m_dwLastError = GetLastError();
			return FALSE;
		}
		return TRUE;
  }    

BOOL CVHDManager::Open(PCWSTR path,
	VIRTUAL_DISK_ACCESS_MASK accessMask,
	OPEN_VIRTUAL_DISK_FLAG flags, // OPEN_VIRTUAL_DISK_FLAG_NONE
	ULONG readWriteDepth) // OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT
{
	if ( m_h != NULL )
	{
		return TRUE;
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

	if ( ERROR_SUCCESS !=  ::OpenVirtualDisk(&storageType,
		path,
		accessMask,
		flags,
		&parameters,
		&m_h))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}
BOOL CVHDManager::Attach(ATTACH_VIRTUAL_DISK_FLAG flags,
	__in_opt PSECURITY_DESCRIPTOR securityDescriptor,
	__in_opt OVERLAPPED* overlapped)
{
	assert(0 != m_h);


	if( ERROR_SUCCESS != ::AttachVirtualDisk(m_h,
		securityDescriptor,
		flags,
		0, // no provider-specific flags
		0, // no parameters
		overlapped))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

BOOL CVHDManager::GetSize(__out ULONGLONG& virtualSize,
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
	else
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

BOOL CVHDManager::Detach( __in     DETACH_VIRTUAL_DISK_FLAG Flags,
	__in     ULONG                    ProviderSpecificFlags)
{
	if ( m_h == NULL )
	{
		return ERROR_SUCCESS;
	}
	if( ERROR_SUCCESS != ::DetachVirtualDisk(m_h,Flags, ProviderSpecificFlags) )
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}
void CVHDManager::Close()
{
	if ( m_h != NULL )
	{
		CloseHandle(m_h);
		m_h = NULL;
	}
}
DWORD CVHDManager::GetLastErrorCode()
{
	return m_dwLastError;
}

BOOL CVHDManager::CreateFixedAsync( PCWSTR path, ULONGLONG size)
{
	ZeroMemory( &m_ol, sizeof(OVERLAPPED));
	if( !CreateFixed( path, size, VIRTUAL_DISK_ACCESS_CREATE, NULL, NULL, &m_ol) )
	{
		if ( GetLastErrorCode() == ERROR_IO_PENDING )
		{
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}
BOOL CVHDManager::CreateFixedSync( PCWSTR path, ULONGLONG size)
{
	return CreateFixed( path, size, VIRTUAL_DISK_ACCESS_CREATE, NULL, NULL, NULL );
}

BOOL CVHDManager::GetProcessState(VIRTUAL_DISK_PROGRESS *pProgress)
{
	if( ERROR_SUCCESS != GetVirtualDiskOperationProgress(m_h, &m_ol, pProgress) )
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

ULONG CVHDManager::QuerySize()
{
	if ( m_h == NULL )
	{
		return 0;
	}
	GET_VIRTUAL_DISK_INFO info;
	info.Version = GET_VIRTUAL_DISK_INFO_SIZE;
	ULONG usedSize = 0;
	ULONG size = sizeof(GET_VIRTUAL_DISK_INFO);
	ULONG errorCode = 0;
	errorCode = GetVirtualDiskInformation(m_h, &size, &info, &usedSize);
	if( ERROR_SUCCESS ==  errorCode )
	{
		return usedSize;
	}
	else
	{
		m_dwLastError = errorCode;
		return 0;
	}
	return 0;
}

BOOL CVHDManager::ConnectDiskService()
{
	HRESULT hResult;     
	IVdsService* pService = NULL;     
	IVdsServiceLoader *pLoader = NULL;     //Launch the VDS Service     
	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);     // Initialize COM security     
	CoInitializeSecurity(  NULL,       // Allow *all* VSS writers to communicate back!         
		-1,        // Default COM authentication service         
		NULL,       // Default COM authorization service         
		NULL,       // reserved parameter         
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY, // Strongest COM authentication level         
		RPC_C_IMP_LEVEL_IMPERSONATE,  // Minimal impersonation abilities          
		NULL,       // Default COM authentication settings         
		EOAC_NONE,      // No special options         
		NULL       // Reserved parameter         
		);      
	if( SUCCEEDED(hResult) )     
	{         
		hResult = CoCreateInstance( 
			CLSID_VdsLoader, 
			NULL, 
			CLSCTX_LOCAL_SERVER,  
			IID_IVdsServiceLoader, 
			(void**) &pLoader             
			);          //if succeeded load VDS on local machine         
		if( SUCCEEDED(hResult) )             
		{	
			hResult =	pLoader->LoadService(NULL, &pService);         
			if( SUCCEEDED(hResult) )         
			{             
				hResult = pService->WaitForServiceReady();             
				if ( SUCCEEDED(hResult) )             
				{       
					m_pVdsService = pService;               
					return TRUE;                    
				}
				
			}
		}
		//Done with Loader now release VDS Loader interface         
		         
		
	}
	_SafeRelease(pService);
	_SafeRelease(pLoader); 
	CoUninitialize();
	return FALSE;
}

BOOL CVHDManager::UnMountDisk()
{
	if ( m_pOpenVdisk == NULL )
	{
		return FALSE;
	}
	HRESULT hr;
	hr = m_pOpenVdisk->Detach(DETACH_VIRTUAL_DISK_FLAG_NONE, 0 );
	return SUCCEEDED(hr)?TRUE:FALSE;
}
BOOL CVHDManager::MountDisk( LPCWSTR strDiskPath, BOOL bAttatch /*= TRUE*/)
{
	if ( m_pVdsService == NULL )
	{
		return FALSE;
	}
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown *ppObjUnk;
	IEnumVdsObject *pEnumVdsObject = NULL;
	IVdsVdProvider *pVdsVdProvider = NULL;
	IVdsAdvancedDisk *pAdvanceDisk= NULL;
	hResult = m_pVdsService->QueryProviders(VDS_QUERY_VIRTUALDISK_PROVIDERS, &pEnumVdsObject);
	int count = 0;
	if (true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched); 
		if (ulFetched == 0 )
		{
			return FALSE;
		}
		count++;
		hResult = ppObjUnk->QueryInterface(IID_IVdsVdProvider,(void**)&pVdsVdProvider); 
		if ( SUCCEEDED(hResult))
		{
			VIRTUAL_STORAGE_TYPE type;
			type.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
			hResult = CLSIDFromString( L"{EC984AEC-A0F9-47e9-901F-71415A66345B}", &type.VendorId);
			if ( SUCCEEDED(hResult))
			{
				IVdsVDisk *pVdisk = NULL;
				WCHAR strVhdPath[MAX_PATH] = {0};
				wcscpy( strVhdPath, LPCTSTR(strDiskPath));
				hResult = pVdsVdProvider->AddVDisk( (PVIRTUAL_STORAGE_TYPE)&type, strVhdPath, (IVdsVDisk**)&pVdisk );
				if ( SUCCEEDED(hResult))
				{

					IVdsOpenVDisk *pOpenVdisk = NULL;
					hResult = pVdisk->Open(VIRTUAL_DISK_ACCESS_ALL, OPEN_VIRTUAL_DISK_FLAG_NONE, 1, &pOpenVdisk);
					if ( SUCCEEDED(hResult))
					{
						if ( !bAttatch )
						{
							m_pVdsVdProvider = pVdsVdProvider;
							m_pOpenVdisk = pOpenVdisk;
							m_pVdsVirDisk = pVdisk;
							return TRUE;
						}
						IVdsAsync *pWait = NULL;
						hResult = pOpenVdisk->Attach(NULL,ATTACH_VIRTUAL_DISK_FLAG_NONE,0,0, &pWait);
						if ( SUCCEEDED(hResult) )
						{
							HRESULT res;
							VDS_ASYNC_OUTPUT kk;
							pWait->Wait(&res, &kk);
							_SafeRelease(pWait);
							if ( SUCCEEDED(res))
							{
								m_pVdsVdProvider = pVdsVdProvider;
								m_pOpenVdisk = pOpenVdisk;
								m_pVdsVirDisk = pVdisk;
								return TRUE;
							}
							else
							{
								_SafeRelease(pOpenVdisk);
								_SafeRelease(pVdisk);
								_SafeRelease(pVdsVdProvider);
							}
						}
					}
					
				}
			}
		}
	}
	return FALSE;
}

BOOL CVHDManager::SetMultipleInterface()
{
	if ( m_pVdsVdProvider == NULL )
	{
		return FALSE;
	}
	HRESULT hr;
	hr = m_pVdsVdProvider->GetDiskFromVDisk(m_pVdsVirDisk, &m_pVdsDisk);
	if ( FAILED(hr))
	{
		return FALSE;
	}
	hr = m_pVdsDisk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&m_pAdvDisk);
	if ( FAILED(hr))
	{
		return FALSE;
	}
	return TRUE;

}

WCHAR CVHDManager::GetDiskLetter()
{
	HRESULT hr;
	if ( m_pPack == NULL )
	{
		if( !SetMultipleInterface() )
		{
			return L'\0';
		}
		if( !SetVdsPackInterface() )
		if ( FAILED(hr))
		{
			return L'\0';
		}
	}
	if( !SetVolInterface() )
	{
		return L'\0';
	}
	LPWSTR *pDrvLetter = NULL;
	LONG num;
	hr = m_pVolMF->QueryAccessPaths( &pDrvLetter, &num);
	if (SUCCEEDED(hr))
	{
		WCHAR dr = *((WCHAR*)(*pDrvLetter));
		CoTaskMemFree(pDrvLetter);
		return dr;
	}
	return L'\0';

}
BOOL CVHDManager::InitDisk()
{
	if ( m_pVdsVirDisk == NULL )
	{
		return FALSE;
	}
	LPWSTR pDiskName = NULL;
	HRESULT hResult;
	hResult = m_pVdsVirDisk->GetDeviceName(&pDiskName);
	if ( FALSE(hResult))
	{
		return FALSE;
	}
	HANDLE hDisk = ::CreateFile(pDiskName, FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hDisk == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	else
	{
		CREATE_DISK createDisk;
		createDisk.PartitionStyle = PARTITION_STYLE_MBR;
		createDisk.Mbr.Signature = 55;
		DWORD retSize = 0;
		if( ::DeviceIoControl(hDisk, IOCTL_DISK_CREATE_DISK, &createDisk, sizeof(createDisk), NULL, 0, &retSize, NULL ) )
		{
			CloseHandle(hDisk);
			return TRUE;
		}
		else
		{
			CloseHandle(hDisk);
			return FALSE;
		}
	}
}

int CVHDManager::GetPartitionCount()
{
	if ( m_pAdvDisk == NULL )
	{
		return -2;//´íÎó
	}
	LONG numPartitions = 0;
	VDS_PARTITION_PROP *Prop = NULL;
	HRESULT hResult;
	hResult = m_pAdvDisk->QueryPartitions(&Prop, &numPartitions);
	if ( FAILED(hResult) )
	{
		return -1;//
	}
	VDS_PARTITION_PROP *pTemp = Prop;
	CoTaskMemFree(Prop);
	return numPartitions;
}

BOOL CVHDManager::CreatePartition(ULONGLONG partSize)
{
	if ( m_pAdvDisk == NULL )
	{
		return FALSE;
	}
	CREATE_PARTITION_PARAMETERS params;
	params.style = VDS_PST_MBR;
	params.MbrPartInfo.partitionType = PARTITION_IFS;
	params.MbrPartInfo.bootIndicator = FALSE;
	IVdsAsync *pAsyOpera = NULL;
	IVdsAdvancedDisk *pTempDisk = NULL;
	HRESULT hResult;
	//hResult = m_pVdsDisk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&pTempDisk);
	//if ( FAILED(hResult))
	//{
	//	return FALSE;
	//}
	hResult = m_pAdvDisk->CreatePartition( 0, partSize, &params, &pAsyOpera);
	if ( SUCCEEDED(hResult))
	{
		HRESULT res;
		VDS_ASYNC_OUTPUT kk;

		pAsyOpera->Wait(&res, &kk);
		_SafeRelease(pAsyOpera);
		if(SUCCEEDED(res))
		{
			if ( kk.type == VDS_ASYNCOUT_CREATEPARTITION)
			{
				m_realStartPos = kk.cp.ullOffset;
			}
			
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CVHDManager::SetVdsPackInterface()
{
	HRESULT hResult;
	hResult = m_pVdsDisk->GetPack(&m_pPack);
	if ( FAILED(hResult))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CVHDManager::SetVolInterface()
{
	HRESULT hResult;     
	ULONG ulFetched = 0;     
	IUnknown* ppObjUnk ;     
	IEnumVdsObject* pEnumVdsObject = NULL;       
	IVdsVolume* pVdsVolume = NULL;      
	if (m_pPack == NULL)         
		return FALSE;
	hResult = m_pPack->QueryVolumes(&pEnumVdsObject);      
	if (pEnumVdsObject == NULL)         
		return FALSE;
	if( true)     
	{         
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched != 0)
		{
			hResult = ppObjUnk->QueryInterface(IID_IVdsVolume, (void**)&pVdsVolume);    
			if ( SUCCEEDED(hResult))
			{
				m_pVolume = pVdsVolume;
				IVdsVolumeMF *pVolFormat = NULL;
				hResult = pVdsVolume->QueryInterface(IID_IVdsVolumeMF, (void**)&pVolFormat);
				if ( SUCCEEDED(hResult))
				{
					m_pVolMF = pVolFormat;
					_SafeRelease(ppObjUnk);
					_SafeRelease(pEnumVdsObject);
					return TRUE;
				}
			}
		}
	}
	_SafeRelease(ppObjUnk);
	_SafeRelease(pEnumVdsObject);
	return FALSE;
}
BOOL CVHDManager::GetVolumeName(WCHAR *pVolName, ULONG nLen)
{
	LPWSTR pName = NULL;
	if( SUCCEEDED(m_pVdsVirDisk->GetDeviceName(&pName) ))
	{
		if ( wcslen(pName) > nLen -1)
		{
			CoTaskMemFree(pName);
			return FALSE;
		}
		ZeroMemory(pVolName, nLen*2);
		wcscpy( pVolName, pName);
		return TRUE;
	}
	return FALSE;
}

BOOL CVHDManager::AddLetter(WCHAR letter)
{
	if ( m_pVolMF == NULL )
	{
		return FALSE;
	}
	WCHAR volPath[MAX_PATH] = {0};
	*volPath = letter;
	wcscat( volPath, L":\\");
	HRESULT hResult;
	hResult = m_pVolMF->AddAccessPath(volPath);
	if(SUCCEEDED(hResult))
	{
		return TRUE;
	}
	return FALSE;
}


BOOL CVHDManager::FormatDisk(LPCWSTR strDiskName)
{
	if ( m_pVolMF == NULL )
	{
		return FALSE;
	}
	WCHAR volName[MAX_PATH] = {0};
	wcscpy( volName, strDiskName);
	HRESULT hResult;
	IVdsAsync *pFormatopera = NULL;
	hResult = m_pVolMF->Format(VDS_FST_NTFS, volName, 512, FALSE, TRUE, FALSE, &pFormatopera);
	if ( SUCCEEDED(hResult) )
	{
		HRESULT hFormatRes;
		VDS_ASYNC_OUTPUT kk;
		pFormatopera->Wait(&hFormatRes, &kk);
		_SafeRelease(pFormatopera);
		if ( SUCCEEDED(hFormatRes))
		{
			return TRUE;
		}
	}
	return FALSE;
}
			
			
				

void CVHDManager::ReleaseService()
{
	//IVdsService *m_pVdsService;
	//IVdsVdProvider *m_pVdsVdProvider;
	//IVdsOpenVDisk *m_pOpenVdisk;
	//IVdsDisk *m_pVdsDisk;
	//IVdsVDisk *m_pVdsVirDisk;
	//IVdsPack *m_pPack;
	//IVdsAdvancedDisk *m_pAdvDisk;
	//IVdsVolume *m_pVolume;
	//IVdsVolumeMF *m_pVolMF;
	
	_SafeRelease(m_pVolMF);
	_SafeRelease(m_pVolume);
	_SafeRelease(m_pOpenVdisk);
	_SafeRelease(m_pAdvDisk);
	_SafeRelease(m_pVdsVirDisk);
	_SafeRelease(m_pVdsDisk);
	_SafeRelease(m_pPack);;
	_SafeRelease(m_pVdsService);
	CoUninitialize();
}



