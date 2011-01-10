using System;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct VIRTUAL_DISK_PROGRESS
    {
        public int OperationStatus;
        public ulong CurrentValue;
        public ulong CompletionValue;
    }
}
