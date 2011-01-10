using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323657(VS.85).aspx
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct COMPACT_VIRTUAL_DISK_PARAMETERS
    {
        public COMPACT_VIRTUAL_DISK_VERSION Version;
        public COMPACT_VIRTUAL_DISK_PARAMETERS_Version1 Version1;
    }
}
