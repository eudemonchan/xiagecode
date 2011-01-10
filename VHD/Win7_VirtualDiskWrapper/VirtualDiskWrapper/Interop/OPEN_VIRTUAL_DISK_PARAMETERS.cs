using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VhdWrapper.Interop
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/dd323682(VS.85).aspx
    /// </summary>
    internal struct OPEN_VIRTUAL_DISK_PARAMETERS
    {
        public OPEN_VIRTUAL_DISK_VERSION Version;
        public uint RWDepth;
    }
}
