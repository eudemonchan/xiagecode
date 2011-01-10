using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct STORAGE_DEPENDENCY_INFO_TYPE_1
    {
        public DEPENDENT_DISK_FLAG DependencyTypeFlags; 
        public uint ProviderSpecificFlags;
        public VIRTUAL_STORAGE_TYPE VirtualStorageType;
    }
}
