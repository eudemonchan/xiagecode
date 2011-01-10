using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct SET_VIRTUAL_DISK_INFO
    {
        public SET_VIRTUAL_DISK_INFO_VERSION Version;
        public SET_VIRTUAL_DISK_INFO_Union Union;
    }
}
