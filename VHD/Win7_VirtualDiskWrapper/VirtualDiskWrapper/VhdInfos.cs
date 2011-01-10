using System;

namespace VhdWrapper
{
    public struct VhdInfos
    {
        public ulong VirtualSize;
        public ulong PhysicalSize;
        public uint BlockSize;
        public uint SectorSize;
        public Guid Identifier;
        public Guid ParentIdentifier;
        public Int32 ParentTimestamp;
        public VHD_STORAGE_TYPE VirtualStorageType;
        public Int32 ProviderSubtype;

        public override string ToString()
        {
            return string.Format("VirtualSize:{0}\r\nPhysicalSize:{1}\r\nBlockSize:{2}\r\nSectorSize:{3}\r\nIdentifier:{4}\r\nParentIdentifier:{5}\r\nParentTimestamp:{6}\r\nVirtualStorageType{7}\r\nProviderSubtype{8}",
                VirtualSize, PhysicalSize, BlockSize, SectorSize, Identifier, ParentIdentifier, ParentTimestamp, VirtualStorageType, ProviderSubtype);
        }
    }
}
