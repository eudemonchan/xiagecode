using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323661(VS.85).aspx
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct CREATE_VIRTUAL_DISK_PARAMETERS
    {
        public CREATE_VIRTUAL_DISK_VERSION Version;
        public CREATE_VIRTUAL_DISK_PARAMETERS_Version1 Version1;
    }
}
