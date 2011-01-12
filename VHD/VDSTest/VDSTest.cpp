// VDSTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "initguid.h"
#include "vds.h"
#include <stdio.h>

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
	while( true)     
	{         
		if (!pEnumVdsObject)             
			break;          
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) break;          
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
		if ( pNumberOfPartitions !=0 )
		{
			GUID diskID;
			CLSIDFromString( L"{E60C853F-7F00-4268-83B9-A42F14D05DD3}", &diskID );
			hResult = pPack->AddDisk( diskID, VDS_PST_MBR, TRUE );
			DWORD kk = HRESULT_FROM_WIN32(hResult);
			return;
		}
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
	while( true)     
	{         
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) break;          
		hResult = ppObjUnk->QueryInterface(IID_IVdsVolume, (void**)&pVdsVolume);         
		VDS_VOLUME_PROP volProp;         
		pVdsVolume->GetProperties(&volProp);         
		printf("Vol name  : %S\n", volProp.pwszName);         
		printf("Vol health: %d\n", volProp.health);     
	} 
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
	while (true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) 
			break;          
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
}

void EnumerateSoftwareProviders(IVdsService *pService)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown *ppObjUnk;
	IEnumVdsObject *pEnumVdsObject = NULL;
	IVdsSwProvider *pVdsSwProvider = NULL;
	hResult = pService->QueryProviders(VDS_QUERY_SOFTWARE_PROVIDERS, &pEnumVdsObject);
	while (true)
	{
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched); 
		if (ulFetched == 0 )
		{
			break;
		}
		hResult = ppObjUnk->QueryInterface(IID_IVdsSwProvider,(void**)&pVdsSwProvider); 
		EnumeratePacks(pVdsSwProvider);
	}
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


