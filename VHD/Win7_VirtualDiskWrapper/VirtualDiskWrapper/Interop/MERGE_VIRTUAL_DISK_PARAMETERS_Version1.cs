using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323678(VS.85).aspx
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct MERGE_VIRTUAL_DISK_PARAMETERS_Version1
    {
        public uint RWDepth;
    }
}
