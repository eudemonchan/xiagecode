using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct STORAGE_DEPENDENCY_INFO
    {
        public STORAGE_DEPENDENCY_INFO_VERSION Version;
        public uint NumberEntries;
        public STORAGE_DEPENDENCY_INFO_Union Union;
    }
}
