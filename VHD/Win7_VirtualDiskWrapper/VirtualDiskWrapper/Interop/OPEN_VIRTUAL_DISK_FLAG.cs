using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323681(VS.85).aspx
    /// </summary>
    [Flags()]
    internal enum OPEN_VIRTUAL_DISK_FLAG : int
    {
        OPEN_VIRTUAL_DISK_FLAG_NONE = 0x00000000,
        OPEN_VIRTUAL_DISK_FLAG_NO_PARENTS = 0x00000001,
        OPEN_VIRTUAL_DISK_FLAG_BLANK_FILE = 0x00000002,
        OPEN_VIRTUAL_DISK_FLAG_BOOT_DRIVE = 0x00000004
    }
}
