using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct GET_VIRTUAL_DISK_INFO_ParentLocation
    {
        public bool ParentResolved;
        public char ParentLocationBuffer;
    }
}
