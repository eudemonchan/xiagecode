using System;

namespace VhdWrapper
{
    public enum VHD_OPEN_FLAG : int
    {
        NONE = 0x00000000,
        NO_PARENTS = 0x00000001,
        BLANK_FILE = 0x00000002,
        BOOT_DRIVE = 0x00000004
    }
}