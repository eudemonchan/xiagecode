using System;

namespace VhdWrapper
{
    public enum VHD_OPEN_ACCESS : int
    {
        ATTACH_RO = 0x00010000,
        ATTACH_RW = 0x00020000,
        DETACH = 0x00040000,
        GET_INFO = 0x00080000,
        CREATE = 0x00100000,
        METAOPS = 0x00200000,
        READ = 0x000d0000,
        ALL = 0x003f0000,
        WRITABLE = 0x00320000
    }
}
