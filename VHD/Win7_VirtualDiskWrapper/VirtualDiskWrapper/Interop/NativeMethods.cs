using System;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

namespace VhdWrapper.Interop
{
    internal static class NativeMethods
    {
        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323680(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int OpenVirtualDisk(ref VIRTUAL_STORAGE_TYPE VirtualStorageType,
            [MarshalAs(UnmanagedType.LPWStr)]string Path, VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask,
            OPEN_VIRTUAL_DISK_FLAG Flags, ref OPEN_VIRTUAL_DISK_PARAMETERS Parameters, ref IntPtr Handle);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323680(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int OpenVirtualDisk(ref VIRTUAL_STORAGE_TYPE VirtualStorageType,
            [MarshalAs(UnmanagedType.LPWStr)]string Path, VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask,
            OPEN_VIRTUAL_DISK_FLAG Flags, IntPtr Parameters, ref IntPtr Handle);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323659(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Unicode)]
        internal static extern int CreateVirtualDisk(ref VIRTUAL_STORAGE_TYPE VirtualStorageType, String Path,
            VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, IntPtr SecurityDescriptor, CREATE_VIRTUAL_DISK_FLAG Flags,
            int ProviderSpecificFlags, ref CREATE_VIRTUAL_DISK_PARAMETERS Parameters, IntPtr Overlapped, 
            ref IntPtr Handle);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323659(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Unicode)]
        internal static extern int CreateVirtualDisk(ref VIRTUAL_STORAGE_TYPE VirtualStorageType, String Path,
            VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, IntPtr SecurityDescriptor, CREATE_VIRTUAL_DISK_FLAG Flags,
            int ProviderSpecificFlags, ref CREATE_VIRTUAL_DISK_PARAMETERS Parameters, ref NativeOverlapped Overlapped,
            ref IntPtr Handle);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323672(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Unicode)]
        internal static extern int GetVirtualDiskPhysicalPath(IntPtr VirtualDiskHandle, ref int DiskPathSizeInBytes,
            StringBuilder DiskPath);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323692(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int AttachVirtualDisk(IntPtr VirtualDiskHandle, IntPtr SecurityDescriptor,
            ATTACH_VIRTUAL_DISK_FLAG Flags, uint ProviderSpecificFlags, ref ATTACH_VIRTUAL_DISK_PARAMETERS Parameters,
            IntPtr Overlapped);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323696(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int DetachVirtualDisk(IntPtr VirtualDiskHandle, DETACH_VIRTUAL_DISK_FLAG Flags, uint ProviderSpecificFlags);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323655(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int CompactVirtualDisk(IntPtr VirtualDiskHandle, COMPACT_VIRTUAL_DISK_FLAG Flags, ref COMPACT_VIRTUAL_DISK_PARAMETERS Parameters, IntPtr Overlapped);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323655(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int CompactVirtualDisk(IntPtr VirtualDiskHandle, COMPACT_VIRTUAL_DISK_FLAG Flags, ref COMPACT_VIRTUAL_DISK_PARAMETERS Parameters, ref NativeOverlapped Overlapped);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323670(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int GetVirtualDiskInformation(IntPtr VirtualDiskHandle, ref int VirtualDiskInfoSize,
             ref GET_VIRTUAL_DISK_INFO VirtualDiskInfo, ref int SizeUsed);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323664(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int ExpandVirtualDisk(IntPtr VirtualDiskHandle, EXPAND_VIRTUAL_DISK_FLAG Flags,
            ref EXPAND_VIRTUAL_DISK_PARAMETERS Parameters, IntPtr Overlapped);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323664(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int ExpandVirtualDisk(IntPtr VirtualDiskHandle, EXPAND_VIRTUAL_DISK_FLAG Flags,
            ref EXPAND_VIRTUAL_DISK_PARAMETERS Parameters, ref NativeOverlapped Overlapped);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323669(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int GetStorageDependencyInformation(IntPtr ObjectHandle, GET_STORAGE_DEPENDENCY_FLAG Flags,
            uint StorageDependencyInfoSize, ref STORAGE_DEPENDENCY_INFO StorageDependencyInfo, ref uint SizeUsed);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323685(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int SetVirtualDiskInformation(IntPtr VirtualDiskHandle, ref SET_VIRTUAL_DISK_INFO VirtualDiskInfo);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323671(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int GetVirtualDiskOperationProgress(IntPtr VirtualDiskHandle, ref NativeOverlapped Overlapped, ref VIRTUAL_DISK_PROGRESS Progress);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323676(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        public static extern int MergeVirtualDisk(IntPtr VirtualDiskHandle, MERGE_VIRTUAL_DISK_FLAG Flags, 
            ref MERGE_VIRTUAL_DISK_PARAMETERS Parameters, IntPtr Overlapped);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/dd323676(VS.85).aspx
        /// </summary>
        [DllImport("Virtdisk.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        public static extern int MergeVirtualDisk(IntPtr VirtualDiskHandle, MERGE_VIRTUAL_DISK_FLAG Flags,
            ref MERGE_VIRTUAL_DISK_PARAMETERS Parameters, ref NativeOverlapped Overlapped);

        [DllImport("kernel32.dll")]
        internal static extern bool CloseHandle(IntPtr hObject);
    }
}
