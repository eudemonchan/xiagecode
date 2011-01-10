using System;

namespace VhdWrapper
{
    public enum VHD_INFO_REQUEST : int
    {
        SIZE = 1,
        IDENTIFIER = 2,
        PARENT_IDENTIFIER = 4,
        PARENT_TIMESTAMP = 8,
        VIRTUAL_STORAGE_TYPE = 16,
        PROVIDER_SUBTYPE = 32
    }
}
