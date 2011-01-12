// VDSTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "initguid.h"
#include "vds.h"
#include <stdio.h>
#pragma comment(lib,"Rpcrt4.lib")

#define _SafeRelease(x) {if (NULL != x) { x->Release(); x = NULL; } }

void EnumerateDisks(IVdsPack* pPack) 
{     
	HRESULT hResult;     
	ULONG ulFetched = 0;     
	IUnknown* ppObjUnk ;     
	IEnumVdsObject* pEnumVdsObject = NULL;       
	IVdsDisk* pVdsDisk = NULL;     
	IVdsDisk2* pVdsDisk2 = NULL;     
	IVdsAdvancedDisk* pVdsAdvancedDisk = NULL;      
	if (pPack == 0)         
		return;      
	hResult = pPack->QueryDisks(&pEnumVdsObject);      
	if (pEnumVdsObject == 0)         
		return;     
	int count = 0;
	while( true)     
	{         
		if (!pEnumVdsObject)             
			break;          
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) break;          
		count++;
		hResult = ppObjUnk->QueryInterface(IID_IVdsDisk, (void**)&pVdsDisk);          
		VDS_DISK_PROP diskProp;         
		pVdsDisk->GetProperties(&diskProp);          
		printf("----------------------------------------\n");         
		wprintf(L"disk: %d\n", diskProp.status);         
		wprintf(L"disk: %s\n", diskProp.pwszAdaptorName);         
		wprintf(L"disk: %s\n", diskProp.pwszDevicePath);         
		wprintf(L"disk: %s\n", diskProp.pwszFriendlyName);         
		wprintf(L"disk: %s\n", diskProp.pwszName);         
		wprintf(L"disk: %d\n", diskProp.dwDeviceType);         
		wprintf(L"disk: %d\n", diskProp.dwMediaType);         
		wprintf(L"disk: %d\n", diskProp.dwSignature);         
		wprintf(L"disk: %d\n", diskProp.PartitionStyle);        
		wprintf(L"disk: %d\n", diskProp.ReserveMode);         
		wprintf(L"disk: %d\n", diskProp.ulFlags);          
		VDS_PARTITION_PROP * pPropArray = NULL;         
		LONG pNumberOfPartitions = 0;          
		hResult = ppObjUnk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&pVdsAdvancedDisk);                 
	
		pVdsAdvancedDisk->QueryPartitions(&pPropArray, &pNumberOfPartitions);          
		VDS_PARTITION_PROP * tmp = pPropArray;         
	/*	if ( pNumberOfPartitions !=0 )
		{
			GUID diskID;
			CLSIDFromString( L"{E60C853F-7F00-4268-83B9-A42F14D05DD3}", &diskID );
			hResult = pPack->AddDisk( diskID, VDS_PST_MBR, TRUE );
			DWORD kk = HRESULT_FROM_WIN32(hResult);
			return;
		}*/
		for (int i = 0; i < pNumberOfPartitions;i++)
		{
			printf("Style : %d\n", tmp->PartitionStyle);             
			printf("Flags : %d\n", tmp->ulFlags);             
			printf("Offset: %ull\n", tmp->ullOffset);             
			printf("Size: %ull\n", tmp->ullSize);             
			printf("MBR type: %d\n", tmp->Mbr.partitionType);             
			printf("MBR type: %d\n", tmp->Mbr.bootIndicator);             
			printf("MBR type: %d\n", tmp->Mbr.recognizedPartition);             
			printf("MBR type: %d\n", tmp->Mbr.hiddenSectors);              
			++tmp;         
		}
		CoTaskMemFree(pPropArray);
	}       
	printf("disk num:%d", count);
} 


void EnumerateVolumes(IVdsPack* pPack) 
{     
	HRESULT hResult;     
	ULONG ulFetched = 0;     
	IUnknown* ppObjUnk ;     
	IEnumVdsObject* pEnumVdsObject = NULL;       
	IVdsVolume* pVdsVolume = NULL;      
	if (pPack == 0)         
		return;      
	hResult = pPack->QueryVolumes(&pEnumVdsObject);      
	if (pEnumVdsObject == 0)         
		return;      
	int count = 0;
	while( true)     
	{         
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) break;          
		count++;
		hResult = ppObjUnk->QueryInterface(IID_IVdsVolume, (void**)&pVdsVolume);         
		VDS_VOLUME_PROP volProp;         
		pVdsVolume->GetProperties(&volProp);         
		printf("Vol name  : %S\n", volProp.pwszName);         
		printf("Vol health: %d\n", volProp.health);     
	} 
	printf("vol nums:%d", count);
} 


void EnumeratePacks(IVdsSwProvider* pProvider)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown* ppObjUnk;
	IEnumVdsObject* pEnumVdsObject = NULL;
	IVdsPack* pVdsPack = NULL;
	if ( pProvider == 0 )
	{
		return;
	}
	hResult = pProvider->QueryPacks(&pEnumVdsObject); 
	if (pEnumVdsObject == 0)
	{
		return;
	}
	int count = 0;
	while (true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) 
			break;
		count++;
		if ( count == 2 )
		{
			IVdsPack *pPack;
			hResult = pProvider->CreatePack( &pPack);
			if ( SUCCEEDED(hResult) )
			{
				GUID diskID;
				CLSIDFromString( L"{E60C853F-7F00-4268-83B9-A42F14D05DD3}", &diskID );
				hResult = pPack->AddDisk( diskID, VDS_PST_MBR, FALSE );
				DWORD kk = HRESULT_FROM_WIN32(hResult);
				printf("%x", hResult);
				getchar();
				return;
			}
		}
		hResult = ppObjUnk->QueryInterface(IID_IVdsPack, (void**)&pVdsPack);         
		VDS_PACK_PROP packProp;         
		pVdsPack->GetProperties(&packProp);         
		if (packProp.status == VDS_PS_ONLINE) 
		{             
			printf("Pack name  : %S\n", packProp.pwszName);             
			printf("Pack status: %d\n", packProp.status);             
			printf("Pack flags : %d\n", packProp.ulFlags);             
			EnumerateDisks(pVdsPack);             
			EnumerateVolumes(pVdsPack);         
		} 
	}
	printf("packsNum:%d", count);
}
void exploreVDiskProvider(IVdsVdProvider *pVdProvider) 
{     
	HRESULT hResult;     
	ULONG ulFetched = 0;      
	IEnumVdsObject *pVDiskEnum = NULL;     
	IVdsVDisk *pVDisk = NULL;     
	IUnknown *pUnknown = NULL;     
	IVdsVolume *pVolume = NULL;     
	VDS_VDISK_PROPERTIES vdiskProperties = { 0 };     
	TCHAR *uuid = NULL;     
	IVdsDisk *pDisk = NULL;     
	VDS_DISK_PROP diskProperties = { 0 };      // Query the disks handled by the provider     
	hResult = pVdProvider->QueryVDisks(&pVDiskEnum);     
	if (FAILED(hResult)) goto bail;      
	printf("Querying virtual disks...\n");      // Iterate over virtual disks     
	while(1)      
	{         
		ulFetched = 0;         
		hResult = pVDiskEnum->Next(1, &pUnknown, &ulFetched);         
		if (hResult == S_FALSE) 
		{             
			break;         
		}          
		if (FAILED(hResult)) goto bail;          // Cast the current value to a disk         
		hResult = pUnknown->QueryInterface(IID_IVdsVDisk, (void **) &pVDisk);         
		if (FAILED(hResult)) goto bail;          
		printf("Virtual disk Found\n");          // Get the disk's properties and display some of them         
		hResult = pVDisk->GetProperties(&vdiskProperties);         
		if (FAILED(hResult)) goto bail;          // Convert the GUID to a string         
		UuidToString(&vdiskProperties.Id, (RPC_WSTR *) &uuid);          // Dump some properties         
		printf("-> Disk Id=%ws\n", uuid);         
		printf("-> Disk Device Name=%ws\n", vdiskProperties.pDeviceName);         
		printf("-> Disk Path=%ws\n", vdiskProperties.pPath);          // Get the disk instance from the virtual disk         
		hResult = pVdProvider->GetDiskFromVDisk(pVDisk, &pDisk);         
		if (FAILED(hResult)) goto bail;          
		_SafeRelease(pVDisk);          
		_SafeRelease(pUnknown);          // Get the disk's properties and display some of them         
		hResult = pDisk->GetProperties(&diskProperties);         
		if (FAILED(hResult)) goto bail;          
		printf("-> Disk Name=%ws\n", diskProperties.pwszName);         
		printf("-> Disk Friendly Name=%ws\n", diskProperties.pwszFriendlyName);     
	}      
	return;  
bail:     printf("Failed hr=%x\n", hResult); 
} 

void EnumerateSoftwareProviders(IVdsService *pService)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown *ppObjUnk;
	IEnumVdsObject *pEnumVdsObject = NULL;
	IVdsVdProvider *pVdsVdProvider = NULL;
	hResult = pService->QueryProviders(VDS_QUERY_VIRTUALDISK_PROVIDERS, &pEnumVdsObject);
	int count = 0;
	while (true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched); 
		if (ulFetched == 0 )
		{
			break;
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
				wcscpy( strVhdPath, L"d:\\11.vhd");
				hResult = pVdsVdProvider->AddVDisk( (PVIRTUAL_STORAGE_TYPE)&type, strVhdPath, (IVdsVDisk**)&pVdisk );
				if ( SUCCEEDED(hResult))
				{
					printf("dddd");
				}
				
			}

			exploreVDiskProvider(pVdsVdProvider);
			
		}
		//EnumeratePacks(pVdsVdProvider);
	}
	printf("SwProviderNum:%d", count);
}

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hResult;
	IVdsService *pService = NULL;
	IVdsServiceLoader *pLoader = NULL;

	// For this, you first get a pointer to the VDS Loader
	// Launch the VDS service. 
	//

	hResult = CoInitialize(NULL);

	if (SUCCEEDED(hResult)) 
	{
		hResult = CoCreateInstance(CLSID_VdsLoader,
			NULL,
			CLSCTX_LOCAL_SERVER,
			IID_IVdsServiceLoader,
			(void **) &pLoader);

		// 
		// And then load VDS on the local computer.
		//
		if (SUCCEEDED(hResult)) 
		{
			hResult = pLoader->LoadService(NULL, &pService);
		}

		//
		// You're done with the Loader interface at this point.
		//
		_SafeRelease(pLoader); 

		if (SUCCEEDED(hResult)) 
		{
			hResult = pService->WaitForServiceReady();
			if (SUCCEEDED(hResult)) 
			{
				printf("VDS Service Loaded");
			}
			EnumerateSoftwareProviders(pService); 
			getchar();
			return 0;
		} 
		else 
		{
			printf("VDS Service failed hr=%x\n",hResult);
		}
	}
	getchar();
}


