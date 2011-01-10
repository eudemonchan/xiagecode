using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323660(VS.85).aspx
    /// </summary>
    internal enum CREATE_VIRTUAL_DISK_FLAG: int
    {
        CREATE_VIRTUAL_DISK_FLAG_NONE = 0x00000000,
        CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION = 0x00000001
    }
}
