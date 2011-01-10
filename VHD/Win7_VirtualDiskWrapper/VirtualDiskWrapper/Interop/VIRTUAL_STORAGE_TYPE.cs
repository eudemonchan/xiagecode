using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct VIRTUAL_STORAGE_TYPE
    {
        public STORAGE_TYPE_DEVICE DeviceId;
        public Guid VendorId;

        public static implicit operator VHD_STORAGE_TYPE(VIRTUAL_STORAGE_TYPE storageType)
        {
            VHD_STORAGE_TYPE storage = new VHD_STORAGE_TYPE();

            storage.DeviceId = (VHD_STORAGE_TYPE_DEVICE)storageType.DeviceId;
            storage.VendorId = storageType.VendorId;

            return storage;
        }
    }
}
