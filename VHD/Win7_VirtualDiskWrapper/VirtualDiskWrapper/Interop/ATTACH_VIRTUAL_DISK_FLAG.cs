using System;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323693(VS.85).aspx
    /// </summary>
    [Flags()]
    internal enum ATTACH_VIRTUAL_DISK_FLAG : int
    {
        ATTACH_VIRTUAL_DISK_FLAG_NONE = 0x00000000,
        ATTACH_VIRTUAL_DISK_FLAG_READ_ONLY = 0x00000001,
        ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER = 0x00000002,
        ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME = 0x00000004,
        ATTACH_VIRTUAL_DISK_FLAG_NO_LOCAL_HOST = 0x00000008
    }
}
