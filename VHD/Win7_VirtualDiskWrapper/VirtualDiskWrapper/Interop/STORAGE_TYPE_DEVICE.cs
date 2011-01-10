using System;

namespace VhdWrapper.Interop
{
    internal enum STORAGE_TYPE_DEVICE : uint
    {
        VIRTUAL_STORAGE_TYPE_DEVICE_UNKNOWN = 0,
        VIRTUAL_STORAGE_TYPE_DEVICE_ISO = 1,
        VIRTUAL_STORAGE_TYPE_DEVICE_VHD = 2
    }
}
