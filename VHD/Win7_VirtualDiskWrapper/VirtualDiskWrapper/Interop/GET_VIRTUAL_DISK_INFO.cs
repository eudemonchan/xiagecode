using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323674(VS.85).aspx
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct GET_VIRTUAL_DISK_INFO
    {
        public GET_VIRTUAL_DISK_INFO_VERSION Version;
        public GET_VIRTUAL_DISK_INFO_Union Union;

        public static explicit operator VhdInfos(GET_VIRTUAL_DISK_INFO diskInfo)
        {
            VhdInfos vhdInfo = new VhdInfos();

            vhdInfo.BlockSize = diskInfo.Union.Size.BlockSize;
            vhdInfo.Identifier = diskInfo.Union.Identifier;
            vhdInfo.ParentIdentifier = diskInfo.Union.ParentIdentifier;
            vhdInfo.ParentTimestamp = diskInfo.Union.ParentTimestamp;
            vhdInfo.PhysicalSize = diskInfo.Union.Size.PhysicalSize;
            vhdInfo.ProviderSubtype = diskInfo.Union.ProviderSubtype;
            vhdInfo.SectorSize = diskInfo.Union.Size.SectorSize;
            vhdInfo.VirtualSize = diskInfo.Union.Size.VirtualSize;
            vhdInfo.VirtualStorageType = diskInfo.Union.VirtualStorageType;

            return vhdInfo;
        }
    }
}
