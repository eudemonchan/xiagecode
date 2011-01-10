using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct SET_VIRTUAL_DISK_INFO_Union
    {
        public String ParentFilePath;
        public Guid UniqueIdentifier;
    }
}
