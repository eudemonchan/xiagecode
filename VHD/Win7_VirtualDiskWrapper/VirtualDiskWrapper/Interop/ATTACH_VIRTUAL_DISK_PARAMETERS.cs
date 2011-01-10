using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323694(VS.85).aspx
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct ATTACH_VIRTUAL_DISK_PARAMETERS
    {
        public ATTACH_VIRTUAL_DISK_VERSION Version;
        public int Reserved;
    }
}
