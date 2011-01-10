using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323675(VS.85).aspx
    /// </summary>
    internal enum GET_VIRTUAL_DISK_INFO_VERSION
    {
        GET_VIRTUAL_DISK_INFO_UNSPECIFIED = 0,
        GET_VIRTUAL_DISK_INFO_SIZE = 1,
        GET_VIRTUAL_DISK_INFO_IDENTIFIER = 2,
        GET_VIRTUAL_DISK_INFO_PARENT_LOCATION = 3,
        GET_VIRTUAL_DISK_INFO_PARENT_IDENTIFIER = 4,
        GET_VIRTUAL_DISK_INFO_PARENT_TIMESTAMP = 5,
        GET_VIRTUAL_DISK_INFO_VIRTUAL_STORAGE_TYPE = 6,
        GET_VIRTUAL_DISK_INFO_PROVIDER_SUBTYPE = 7
    }
}
