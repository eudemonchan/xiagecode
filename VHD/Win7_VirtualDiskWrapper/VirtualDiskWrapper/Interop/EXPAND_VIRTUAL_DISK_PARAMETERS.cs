using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323667(VS.85).aspx
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct EXPAND_VIRTUAL_DISK_PARAMETERS
    {
        public EXPAND_VIRTUAL_DISK_VERSION Version;
        public EXPAND_VIRTUAL_DISK_PARAMETERS_Version1 Version1;
    }
}
