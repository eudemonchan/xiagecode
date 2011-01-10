using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode)]
    internal struct GET_VIRTUAL_DISK_INFO_Union
    {
        [FieldOffset(0)]
        public GET_VIRTUAL_DISK_INFO_Size Size;
        [FieldOffset(0)]
        public Guid Identifier;
        [FieldOffset(0)]
        public GET_VIRTUAL_DISK_INFO_ParentLocation ParentLocation;
        [FieldOffset(0)]
        public Guid ParentIdentifier;
        [FieldOffset(0)]
        public Int32 ParentTimestamp;
        [FieldOffset(0)]
        public VIRTUAL_STORAGE_TYPE VirtualStorageType;
        [FieldOffset(0)]
        public Int32 ProviderSubtype;
    }
}
