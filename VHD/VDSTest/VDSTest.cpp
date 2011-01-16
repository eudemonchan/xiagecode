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
		IVdsVolumeMF *pVolFormat = NULL;
		hResult = pVdsVolume->QueryInterface(IID_IVdsVolumeMF, (void**)&pVolFormat);
		if ( SUCCEEDED(hResult))
		{
			WCHAR volName[MAX_PATH] = {0};
			wcscpy( volName, L"TestDisk");

			IVdsAsync *pFormatopera = NULL;
			hResult = pVolFormat->Format(VDS_FST_NTFS, volName, 512, FALSE, TRUE, FALSE, &pFormatopera);
			if ( SUCCEEDED(hResult) )
			{
				HRESULT hFormatRes;
				VDS_ASYNC_OUTPUT kk;
				pFormatopera->Wait(&hFormatRes, &kk);
				_SafeRelease(pFormatopera);
				if ( SUCCEEDED(pFormatopera))
				{
					printf("格式化成功！\n");
					WCHAR volPath[MAX_PATH] = {0};
					wcscpy( volPath, L"X:\\");
					hResult = pVolFormat->AddAccessPath(volPath);
					if(SUCCEEDED(hResult))
					{
						printf("设置盘符成功！\n");
					}
				}
				/*if ( SUCCEEDED(hFormatRes))
				{
				printf("format success!\n");
				hResult = pAdvanceDisk->AssignDriveLetter(0,L'X');
				if ( SUCCEEDED(hResult))
				{
				printf("good ,you can see result in explorer!\n");
				}
				}*/
			}
			_SafeRelease(pVolFormat);

		}
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
		/*if ( count == 2 )
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
		}*/
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
		IVdsOpenVDisk *pOpen;
		hResult = pVDisk->Open(VIRTUAL_DISK_ACCESS_ALL,OPEN_VIRTUAL_DISK_FLAG_NONE,1,&pOpen);
		if ( SUCCEEDED(hResult))
		{
			pOpen->Detach(DETACH_VIRTUAL_DISK_FLAG_NONE,0);
		}
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

void exploreVDiskProvider2(IVdsVdProvider *pVdProvider) 
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
	IVdsAdvancedDisk *pAdvanceDisk = NULL;
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
		if (FAILED(hResult)) continue;
		
		
		hResult = pVdProvider->GetDiskFromVDisk(pVDisk, &pDisk);         
		if (FAILED(hResult)) goto bail;  
		hResult = pDisk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&pAdvanceDisk);
		if ( SUCCEEDED(hResult) )
		{
			CREATE_PARTITION_PARAMETERS params;
			params.style = VDS_PST_MBR;
			params.MbrPartInfo.partitionType = PARTITION_IFS;
			params.MbrPartInfo.bootIndicator = FALSE;
			IVdsAsync *pAsyOpera = NULL;
			hResult = pAdvanceDisk->CreatePartition( 0, 80*1024*1024, &params, &pAsyOpera);
			if ( SUCCEEDED(hResult))
			{
				HRESULT res;
				VDS_ASYNC_OUTPUT kk;

				pAsyOpera->Wait(&res, &kk);
				_SafeRelease(pAsyOpera);
				if(SUCCEEDED(res))
				{
					printf("create partition succeed!");
				}
			}
		}
		
		//IVdsVolume *pVol = 
		_SafeRelease(pAdvanceDisk);
		_SafeRelease(pDisk);
		_SafeRelease(pVDisk);          
		_SafeRelease(pUnknown);          // Get the disk's properties and display some of them         
		//hResult = pDisk->GetProperties(&diskProperties);         
		//if (FAILED(hResult)) goto bail;          
		//printf("-> Disk Name=%ws\n", diskProperties.pwszName);         
		//printf("-> Disk Friendly Name=%ws\n", diskProperties.pwszFriendlyName);     
	}      
	return;  
bail:     printf("Failed hr=%x\n", hResult); 
} 
void EnumerateSoftwareProviders2(IVdsService* pService) 
{     
	HRESULT hResult;     
	ULONG ulFetched = 0;     
	IUnknown* ppObjUnk ;     
	IEnumVdsObject* pEnumVdsObject = NULL;     
	IVdsSwProvider* pVdsSwProvider = NULL;      
	hResult = pService->QueryProviders(VDS_QUERY_SOFTWARE_PROVIDERS, &pEnumVdsObject);     
	while( true)     
	{         
		hResult = pEnumVdsObject->Next(1, &ppObjUnk, &ulFetched);         
		if (ulFetched == 0) break;          
		hResult = ppObjUnk->QueryInterface(IID_IVdsSwProvider,(void**)&pVdsSwProvider);         
		EnumeratePacks(pVdsSwProvider);     
	} 
}

void EnumerateSoftwareProviders(IVdsService *pService)
{
	HRESULT hResult;
	ULONG ulFetched = 0;
	IUnknown *ppObjUnk;
	IEnumVdsObject *pEnumVdsObject = NULL;
	IVdsVdProvider *pVdsVdProvider = NULL;
	IVdsAdvancedDisk *pAdvanceDisk= NULL;
	IVdsDisk *pDisk = NULL;
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

					IVdsOpenVDisk *pOpenVdisk = NULL;
					hResult = pVdisk->Open(VIRTUAL_DISK_ACCESS_ALL, OPEN_VIRTUAL_DISK_FLAG_NONE, 1, &pOpenVdisk);
					if ( SUCCEEDED(hResult) || FAILED(hResult))
					{
						IVdsAsync *pWait = NULL;
						hResult = pOpenVdisk->Attach(NULL,ATTACH_VIRTUAL_DISK_FLAG_NONE,0,0, &pWait);
						if ( SUCCEEDED(hResult) )
						{
						HRESULT res;
						VDS_ASYNC_OUTPUT kk;

						pWait->Wait(&res, &kk);
						_SafeRelease(pWait);
						}
						//pVdsVdProvider->QueryVDisks()
						
						if ( SUCCEEDED(hResult) )
						{
							//获取VsDisk
							hResult = pVdsVdProvider->GetDiskFromVDisk( pVdisk, &pDisk);
							if ( SUCCEEDED(hResult))
							{
								//获取AdvancedDisk
								hResult = pDisk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&pAdvanceDisk);
								if ( SUCCEEDED(hResult) )
								{
									LPWSTR pDiskName2 = NULL;
									hResult = pVdisk->GetDeviceName(&pDiskName2);
									wprintf(pDiskName2);

									ULARGE_INTEGER a,b,c;
									GetDiskFreeSpaceEx(pDiskName2, &a, &b, &c);
									CREATE_PARTITION_PARAMETERS params;
									params.style = VDS_PST_MBR;
									params.MbrPartInfo.partitionType = PARTITION_IFS;
									params.MbrPartInfo.bootIndicator = FALSE;
									IVdsAsync *pAsyOpera = NULL;
									hResult = pAdvanceDisk->CreatePartition( 0, 100*1024*1024, &params, &pAsyOpera);
									if ( SUCCEEDED(hResult))
									{
										HRESULT res;
										VDS_ASYNC_OUTPUT kk;

										pAsyOpera->Wait(&res, &kk);
										_SafeRelease(pAsyOpera);
										if(SUCCEEDED(res))
										{
											printf("create partition succeed!");
										}
									}

									//pAdvanceDisk->GetPartitionProperties()

									LONG numPartitions = 0;
									VDS_PARTITION_PROP *Prop = NULL;
									hResult = pAdvanceDisk->QueryPartitions(&Prop, &numPartitions);
									VDS_PARTITION_PROP *pTemp = Prop;
									if ( SUCCEEDED(hResult))
									{
										for ( int i = 0; i < numPartitions; i++ )
										{
											printf("分区号码：%d\n", pTemp->ulPartitionNumber);
											pTemp++;
										}
									}
									CoTaskMemFree(Prop);

									LPWSTR pDiskName = NULL;
									hResult = pVdisk->GetDeviceName(&pDiskName);
									wprintf(pDiskName);

									printf("\n");
									if( SetVolumeMountPoint( L"X:\\", pDiskName) )
									{
										printf("设置成功！\n");
									}
									else
									{
										printf("设置vol失败：%d\n", GetLastError());
									}

									IVdsPack *pVdsPack = NULL;
									hResult = pDisk->GetPack(&pVdsPack);
									if ( SUCCEEDED(hResult) )
									{

										printf("GetPack Succeed!\n");
										EnumerateVolumes(pVdsPack);


										/*VDS_INPUT_DISK diskArray;
										CLSIDFromString( L"{EC984AEC-A0F9-47e9-901F-71415A66345B}", &diskArray.diskId );
										diskArray.memberIdx = 0;
										CLSIDFromString( L"{DE38D56A-77C6-4FBC-8336-949E9090523B}", &diskArray.plexId );
										diskArray.ullSize = 80*1024*1024;
										IVdsAsync *pCVol = NULL;
										hResult = pVdsPack->CreateVolume(VDS_VT_SIMPLE, &diskArray, 1, 0, &pCVol);
										if ( SUCCEEDED(hResult))
										{
											HRESULT res;
											VDS_ASYNC_OUTPUT kk;
											pCVol->Wait(&res, &kk);
											if ( SUCCEEDED(res))
											{
												printf("Create Vol Succeed!\n");

												}
										}*/
									}
									else
									{
										printf("GetPack failed!\n");
									}
									hResult = pAdvanceDisk->AssignDriveLetter(0,L'X');
									if ( SUCCEEDED(hResult))
									{
										printf("good ,you can see result in explorer!\n");
									}

									IVdsVolume *pVol = NULL;
									hResult = pVdisk->GetHostVolume(&pVol);
									if ( SUCCEEDED(hResult) )
									{
										/*GUID kkID;
										CLSIDFromString(L"{C2BEB7D8-1D9A-4FA0-A90A-3A67B2245BCB}", &kkID);
										IVdsAsync *pWait2 = NULL;
										hResult = pVol->AddPlex(kkID, &pWait2);
										if ( SUCCEEDED(hResult) )
										{
											printf("ddddd");
										}
										_SafeRelease(pWait2);*/
										IVdsVolumeMF *pVolFormat = NULL;
										hResult = pVdisk->QueryInterface( IID_IVdsVolumeMF, (void**)&pVolFormat);
										if ( SUCCEEDED(hResult))
										{
											WCHAR volName[MAX_PATH] = {0};
											wcscpy( volName, L"TestDisk");

											IVdsAsync *pFormatopera = NULL;
											hResult = pVolFormat->Format(VDS_FST_NTFS, volName, 512, FALSE, TRUE, FALSE, &pFormatopera);
											if ( SUCCEEDED(hResult) )
											{
												HRESULT hFormatRes;
												VDS_ASYNC_OUTPUT kk;
												pFormatopera->Wait(&hFormatRes, &kk);
												_SafeRelease(pFormatopera);
												if ( SUCCEEDED(hFormatRes))
												{
													printf("format success!\n");
													hResult = pAdvanceDisk->AssignDriveLetter(0,L'X');
													if ( SUCCEEDED(hResult))
													{
														printf("good ,you can see result in explorer!\n");
													}
												}
											}
										}

										_SafeRelease(pVolFormat);
										printf("ddddddddddd");
									}
									_SafeRelease(pVol);
									/*WCHAR volName[MAX_PATH] = {0};
									wcscpy( volName, L"TestDisk");

									IVdsAsync *pFormatopera = NULL;
									hResult = pAdvanceDisk->FormatPartition( 1, VDS_FST_NTFS, volName, 512, FALSE, TRUE, FALSE, &pFormatopera);
									if ( SUCCEEDED(hResult) )
									{
									HRESULT hFormatRes;
									VDS_ASYNC_OUTPUT kk;
									pFormatopera->Wait(&hFormatRes, &kk);
									_SafeRelease(pFormatopera);
									if ( SUCCEEDED(hFormatRes))
									{
									printf("format success!\n");
									hResult = pAdvanceDisk->AssignDriveLetter(0,L'X');
									if ( SUCCEEDED(hResult))
									{
									printf("good ,you can see result in explorer!\n");
									}
									}
									}*/
									
								}
								_SafeRelease(pAdvanceDisk);
							}
							_SafeRelease(pDisk);
							/*printf("name:\n");
							LPWSTR pDiskName = NULL;
							hResult = pVdisk->GetDeviceName(&pDiskName);
							wprintf(pDiskName);
							printf("\n");
							HANDLE hDisk = ::CreateFile(pDiskName, FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);
							if ( hDisk == INVALID_HANDLE_VALUE )
							{
							printf("\n错误是:%d\n", GetLastError());
							}
							else
							{
							CREATE_DISK createDisk;
							createDisk.PartitionStyle = PARTITION_STYLE_MBR;
							createDisk.Mbr.Signature = 55;
							DWORD retSize = 0;
							if( ::DeviceIoControl(hDisk, IOCTL_DISK_CREATE_DISK, &createDisk, sizeof(createDisk), NULL, 0, &retSize, NULL ) )
							{
							printf("DIC OK");
							}
							else
							{
							printf("DOC Error:%d",GetLastError());
							}

							printf("\n成功打开设备！\n");

							CloseHandle(hDisk);
							}*/
						}
						
						////////这里应该初始化
						//exploreVDiskProvider2(pVdsVdProvider);
						//IVdsVolume *pVol = NULL;
						//hResult = pVdisk->GetHostVolume(&pVol);
						//if ( SUCCEEDED(hResult) )
						//{
						//	/*GUID kkID;
						//	CLSIDFromString(L"{C2BEB7D8-1D9A-4FA0-A90A-3A67B2245BCB}", &kkID);
						//	IVdsAsync *pWait2 = NULL;
						//	hResult = pVol->AddPlex(kkID, &pWait2);
						//	if ( SUCCEEDED(hResult) )
						//	{
						//		printf("ddddd");
						//	}
						//	_SafeRelease(pWait2);*/
						//	printf("ddddddddddd");
						//}
						//_SafeRelease(pVol);
						//EnumerateSoftwareProviders2(pService);
						getchar();
						
						pOpenVdisk->Detach(DETACH_VIRTUAL_DISK_FLAG_NONE,0);
						
					}
					//pOpenVdisk->Detach(DETACH_VIRTUAL_DISK_FLAG_NONE,0);

					_SafeRelease(pOpenVdisk);
					_SafeRelease(pVdisk);
					_SafeRelease(pVdsVdProvider);
					return;
					printf("dddd");
				}
				
			}
			
			
		}
		//EnumeratePacks(pVdsVdProvider);
	}
	printf("SwProviderNum:%d", count);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//HRESULT hResult;
	//IVdsService *pService = NULL;
	//IVdsServiceLoader *pLoader = NULL;

	//// For this, you first get a pointer to the VDS Loader
	//// Launch the VDS service. 
	////

	//hResult = CoInitialize(NULL);

	//if (SUCCEEDED(hResult)) 
	//{
	//	hResult = CoCreateInstance(CLSID_VdsLoader,
	//		NULL,
	//		CLSCTX_LOCAL_SERVER,
	//		IID_IVdsServiceLoader,
	//		(void **) &pLoader);

	//	// 
	//	// And then load VDS on the local computer.
	//	//
	//	if (SUCCEEDED(hResult)) 
	//	{
	//		hResult = pLoader->LoadService(NULL, &pService);
	//	}

	//	//
	//	// You're done with the Loader interface at this point.
	//	//
	//	_SafeRelease(pLoader); 

	//	if (SUCCEEDED(hResult)) 
	//	{
	//		hResult = pService->WaitForServiceReady();
	//		if (SUCCEEDED(hResult)) 
	//		{
	//			printf("VDS Service Loaded");
	//		}
	//		EnumerateSoftwareProviders(pService); 
	//		getchar();
	//		return 0;
	//	} 
	//	else 
	//	{
	//		printf("VDS Service failed hr=%x\n",hResult);
	//	}
	//}
	//getchar();

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
		pLoader->LoadService(NULL, &pService);         
		//Done with Loader now release VDS Loader interface         
		_SafeRelease(pLoader);          
		if( SUCCEEDED(hResult) )         
		{             
			hResult = pService->WaitForServiceReady();             
			if ( SUCCEEDED(hResult) )             
			{                 
				EnumerateSoftwareProviders(pService);                 
				getchar();
				return 0;                    
			}         
		}
	}
	return -1; 
}


