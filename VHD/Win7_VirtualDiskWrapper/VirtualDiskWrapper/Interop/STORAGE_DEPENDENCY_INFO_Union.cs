using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Explicit)]
    internal struct STORAGE_DEPENDENCY_INFO_Union
    {
        [FieldOffset(0)]
        public STORAGE_DEPENDENCY_INFO_TYPE_1 Version1Entries;
        [FieldOffset(0)]
        public STORAGE_DEPENDENCY_INFO_TYPE_2 Version2Entries;
    }
}
