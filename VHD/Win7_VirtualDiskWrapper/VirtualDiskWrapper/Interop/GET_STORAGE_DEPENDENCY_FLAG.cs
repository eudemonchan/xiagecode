using System;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323673(VS.85).aspx
    /// </summary>
    internal enum GET_STORAGE_DEPENDENCY_FLAG : int
    {
        GET_STORAGE_DEPENDENCY_FLAG_NONE = 0x00000000,
        GET_STORAGE_DEPENDENCY_FLAG_HOST_VOLUMES = 0x00000001,
        GET_STORAGE_DEPENDENCY_FLAG_DISK_HANDLE = 0x00000002
    }
}
