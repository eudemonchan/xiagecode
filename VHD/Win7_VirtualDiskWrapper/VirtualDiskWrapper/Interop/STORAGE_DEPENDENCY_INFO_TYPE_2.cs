using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct STORAGE_DEPENDENCY_INFO_TYPE_2
    {
        public DEPENDENT_DISK_FLAG DependencyTypeFlags;

        public uint ProviderSpecificFlags;

        public VIRTUAL_STORAGE_TYPE VirtualStorageType;

        public uint AncestorLevel;

        [MarshalAs(UnmanagedType.LPWStr)]
        public String DependencyDeviceName; 

        public IntPtr HostVolumeName;

        public IntPtr DependentVolumeName;

        public IntPtr DependentVolumeRelativePath;
    }
}
