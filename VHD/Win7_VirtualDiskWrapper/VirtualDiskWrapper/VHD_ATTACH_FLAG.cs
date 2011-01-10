using System;

namespace VhdWrapper
{
    public enum VHD_ATTACH_FLAG : int
    {
        NONE = 0x00000000,
        READ_ONLY = 0x00000001,
        NO_DRIVE_LETTER = 0x00000002,
        PERMANENT_LIFETIME = 0x00000004,
        NO_LOCAL_HOST = 0x00000008
    }
}
