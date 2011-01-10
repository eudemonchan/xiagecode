using System;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Text;
using VhdWrapper.Interop;

namespace VhdWrapper
{
    /// <summary>
    /// This class allow you to use the Windows 7 Virtual Disk Image API.
    /// </summary>
    [SecurityPermission(SecurityAction.Demand)]
    public class VHD : IDisposable
    {
        const uint CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE = 0x200;
        const uint CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE = 0x80000;
        
        public const int ERROR_SUCCESS = 0;
        const int ERROR_IO_PENDING = 997;
        const int ERROR_FILE_EXISTS = 80;
        const int ERROR_INVALID_PARAMETER = 87;
        const int ERROR_FILE_NOT_FOUND = 2;
        const int ERROR_PATH_NOT_FOUND = 3;
        const int ERROR_FILE_CORRUPT = 1392;

        public delegate void CreateDiskOperationProgressHandler(VirtualDiskOperationProgress value);
        public delegate void CreateDiskAsyncCompleteHandler(VHD newCreatedVhd);
        public delegate void CompactDiskOperationProgressHandler(VirtualDiskOperationProgress value);
        public delegate void ExpandDiskOperationProgressHandler(VirtualDiskOperationProgress value);

        /// <summary>
        /// Progress infos event for the create VHD operation in asynchronous mode.
        /// </summary>
        public static event CreateDiskOperationProgressHandler CreateVirtualDiskProgress;

        /// <summary>
        /// Event indicates that the create process in asynchronous mode is complete.
        /// </summary>
        public static event CreateDiskAsyncCompleteHandler CreateVirtualDiskComplete;

        /// <summary>
        /// Progress infos event for the compact VHD operation in asynchronous mode.
        /// </summary>
        public event CompactDiskOperationProgressHandler CompactVirtualDiskProgress;

        /// <summary>
        /// Progress infos event for the expand VHD operation in asynchronous mode.
        /// </summary>
        public event ExpandDiskOperationProgressHandler ExpandVirtualDiskProgress;

        bool disposed = false;
        static int iLastError = 0;
        bool attached = false;
        IntPtr hVirtualDisk = IntPtr.Zero;
        string VhdFile = string.Empty;

        static VhdAsyncOperation AsyncCreationVhd;         //VHD async. create.
        VhdAsyncOperation AsyncCompactVhd;
        VhdAsyncOperation AsyncExpandVhd;

        public VHD()
        { }

        internal VHD(IntPtr vhdHandle, string path)
        {
            hVirtualDisk = vhdHandle;
            VhdFile = path;
            if (hVirtualDisk != IntPtr.Zero)
                IsOpen = true;
        }

        /// <summary>
        /// Gets or sets if a VHD file is open.
        /// </summary>
        public bool IsOpen { get; set; }

        /// <summary>
        /// Gets the handle of the VHD open file.
        /// </summary>
        public IntPtr Handle { get { return hVirtualDisk; } }

        /// <summary>
        /// Gets the filename of the VHD open file.
        /// </summary>
        public string FileName { get { return VhdFile; } }

        /// <summary>
        /// Create a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <returns>A <see cref="VHD"/> wrapper.</returns>
        public static VHD Create(string vhdPath, ulong size)
        {
            return Create(vhdPath, size, 0, 0, VHD_CREATE_FLAG.NONE, VHD_OPEN_ACCESS.ALL);
        }

        /// <summary>
        /// Create a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="flag">Creation mode of the virtual disk image.</param>
        /// <returns>A <see cref="VHD"/> wrapper.</returns>
        public static VHD Create(string vhdPath, ulong size, VHD_CREATE_FLAG flag)
        {
            return Create(vhdPath, size, 0, 0, flag, VHD_OPEN_ACCESS.ALL);
        }

        /// <summary>
        /// Create a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="blockSize">The internal size of the virtual disk object blocks.</param>
        /// <param name="sectorSize">The internal size of the virtual disk object sectors</param>
        /// <returns>A <see cref="VHD"/> wrapper.</returns>
        public static VHD Create(string vhdPath, ulong size, uint blockSize, uint sectorSize)
        {
            return Create(vhdPath, size, blockSize, sectorSize, VHD_CREATE_FLAG.NONE, VHD_OPEN_ACCESS.ALL);
        }

        /// <summary>
        /// Create a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="flag">Creation mode of the virtual disk image.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        /// <returns>A <see cref="VHD"/> wrapper.</returns>
        public static VHD Create(string vhdPath, ulong size, VHD_CREATE_FLAG flag, VHD_OPEN_ACCESS mask)
        {
            return Create(vhdPath, size, 0, 0, flag, mask);
        }

        /// <summary>
        /// Create a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="blockSize">The internal size of the virtual disk object blocks.</param>
        /// <param name="sectorSize">The internal size of the virtual disk object sectors</param>
        /// <param name="flag">Creation mode of the virtual disk image.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        /// <returns>A <see cref="VHD"/> wrapper.</returns>
        public static VHD Create(string vhdPath, ulong size, uint blockSize, uint sectorSize, VHD_CREATE_FLAG flag,
            VHD_OPEN_ACCESS mask)
        {
            IntPtr hNewVhd = IntPtr.Zero;

            //Define the parameters.
            CREATE_VIRTUAL_DISK_PARAMETERS param = new CREATE_VIRTUAL_DISK_PARAMETERS();
            param.Version = CREATE_VIRTUAL_DISK_VERSION.CREATE_VIRTUAL_DISK_VERSION_1;

            param.Version1.MaximumSize = size;
            param.Version1.BlockSizeInBytes = blockSize;
            param.Version1.SectorSizeInBytes = sectorSize == 0 ? CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE : sectorSize;

            param.Version1.ParentPath = IntPtr.Zero;
            param.Version1.SourcePath = IntPtr.Zero;
            param.Version1.UniqueId = Guid.Empty;

            //And the right storage type.
            VIRTUAL_STORAGE_TYPE storageType = new VIRTUAL_STORAGE_TYPE();
            storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR.VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;
            storageType.DeviceId = STORAGE_TYPE_DEVICE.VIRTUAL_STORAGE_TYPE_DEVICE_VHD;

            //Create disk.
            iLastError = NativeMethods.CreateVirtualDisk(ref storageType, vhdPath,
                (VIRTUAL_DISK_ACCESS_MASK)mask, IntPtr.Zero, (CREATE_VIRTUAL_DISK_FLAG)flag, 0, ref param, IntPtr.Zero, ref hNewVhd);

            if (iLastError == ERROR_SUCCESS)
                return new VHD(hNewVhd, vhdPath);
            else
                return null;
        }

        /// <summary>
        /// Create a VHD file in asynchronous operation.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        public static void CreateAsync(string vhdPath, ulong size)
        {
            CreateAsync(vhdPath, size, 0, 0, VHD_CREATE_FLAG.NONE, VHD_OPEN_ACCESS.ALL);
        }

        /// <summary>
        /// Create a VHD file in asynchronous operation.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="flag">Creation mode of the virtual disk image.</param>
        public static void CreateAsync(string vhdPath, ulong size, VHD_CREATE_FLAG flag)
        {
            CreateAsync(vhdPath, size, 0, 0, flag, VHD_OPEN_ACCESS.ALL);
        }

        /// <summary>
        /// Create a VHD file in asynchronous operation.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="blockSize">The internal size of the virtual disk object blocks.</param>
        /// <param name="sectorSize">The internal size of the virtual disk object sectors</param>
        public static void CreateAsync(string vhdPath, ulong size, uint blockSize, uint sectorSize)
        {
            CreateAsync(vhdPath, size, blockSize, sectorSize, VHD_CREATE_FLAG.NONE, VHD_OPEN_ACCESS.ALL);
        }

        /// <summary>
        /// Create a VHD file in asynchronous operation.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="flag">Creation mode of the virtual disk image.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        public static void CreateAsync(string vhdPath, ulong size, VHD_CREATE_FLAG flag, VHD_OPEN_ACCESS mask)
        {
            CreateAsync(vhdPath, size, 0, 0, flag, mask);
        }

        /// <summary>
        /// Create a VHD file in asynchronous operation.
        /// </summary>
        /// <param name="vhdPath">The path to the new virtual disk image file.</param>
        /// <param name="size">The size of the virtual disk image.</param>
        /// <param name="blockSize">The internal size of the virtual disk object blocks.</param>
        /// <param name="sectorSize">The internal size of the virtual disk object sectors</param>
        /// <param name="flag">Creation mode of the virtual disk image.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        public static void CreateAsync(string vhdPath, ulong size, uint blockSize, uint sectorSize, VHD_CREATE_FLAG flag,
            VHD_OPEN_ACCESS mask)
        {
            IntPtr hNewVhd = IntPtr.Zero;

            //Define the parameters.
            CREATE_VIRTUAL_DISK_PARAMETERS param = new CREATE_VIRTUAL_DISK_PARAMETERS();
            param.Version = CREATE_VIRTUAL_DISK_VERSION.CREATE_VIRTUAL_DISK_VERSION_1;

            param.Version1.MaximumSize = size;
            param.Version1.BlockSizeInBytes = blockSize;
            param.Version1.SectorSizeInBytes = sectorSize == 0 ? CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE : sectorSize;

            param.Version1.ParentPath = IntPtr.Zero;
            param.Version1.SourcePath = IntPtr.Zero;
            param.Version1.UniqueId = Guid.Empty;

            //And the right storage type.
            VIRTUAL_STORAGE_TYPE storageType = new VIRTUAL_STORAGE_TYPE();
            storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR.VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;
            storageType.DeviceId = STORAGE_TYPE_DEVICE.VIRTUAL_STORAGE_TYPE_DEVICE_VHD;

            //We need to use an overlapped structure with the asynchronous operation.
            AsyncCreationVhd = new VhdAsyncOperation(false, 0, 0);

            //Starting Create disk.
            iLastError = NativeMethods.CreateVirtualDisk(ref storageType, vhdPath, (VIRTUAL_DISK_ACCESS_MASK)mask, IntPtr.Zero,
                (CREATE_VIRTUAL_DISK_FLAG)flag, 0, ref param, ref AsyncCreationVhd.Overlapped, ref hNewVhd);

            //Report progress for the async operation.
            if ((iLastError == ERROR_SUCCESS) || (iLastError == ERROR_IO_PENDING))
            {
                var progress = GetCreateProgress(hNewVhd);
                while (!progress.IsComplete)
                {
                    if (CreateVirtualDiskProgress != null)
                    {
                        CreateVirtualDiskProgress(progress);

                        System.Threading.Thread.Sleep(10);
                        progress = GetCreateProgress(hNewVhd);
                    }
                }

                //Return the new VHD object created.
                if (progress.IsComplete && CreateVirtualDiskComplete != null)
                    CreateVirtualDiskComplete(new VHD(hNewVhd, vhdPath));

            }
            else
            {
                //Errors
                if ((iLastError == ERROR_FILE_NOT_FOUND) || (iLastError == ERROR_PATH_NOT_FOUND))
                    throw new System.IO.FileNotFoundException("File not found.");
                else if (iLastError == ERROR_FILE_CORRUPT)
                    throw new System.IO.InvalidDataException("File format not recognized.");
                else if (iLastError == ERROR_FILE_EXISTS)
                    throw new System.IO.IOException("File already exists.");
                else if (iLastError == ERROR_INVALID_PARAMETER)
                    throw new System.ArgumentException("Invalid parameter.", "size");
                else
                    throw new InvalidOperationException(string.Format(
                        CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
            }
        }

        /// <summary>
        /// Opens a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the virtual disk image file to use.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Open(string vhdPath)
        {
            return this.Open(vhdPath, VHD_OPEN_ACCESS.ALL,
                 VHD_OPEN_FLAG.NONE, 1);
        }

        /// <summary>
        /// Opens a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the virtual disk image file to use.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Open(string vhdPath, VHD_OPEN_ACCESS mask)
        {
            return this.Open(vhdPath, mask, VHD_OPEN_FLAG.NONE, 1);
        }

        /// <summary>
        /// Opens a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the virtual disk image file to use.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        /// <param name="flag">Open mode of the virtual disk image.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Open(string vhdPath, VHD_OPEN_ACCESS mask, VHD_OPEN_FLAG flag)
        {
            return this.Open(vhdPath, mask, flag, 1);
        }

        /// <summary>
        /// Opens a VHD file.
        /// </summary>
        /// <param name="vhdPath">The path to the virtual disk image file to use.</param>
        /// <param name="mask">Mask for specifying access rights to the virtual disk.</param>
        /// <param name="flag">Open mode of the virtual disk image.</param>
        /// <param name="depth">??</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Open(string vhdPath, VHD_OPEN_ACCESS mask,
            VHD_OPEN_FLAG flag, uint depth)
        {
            //Close opened handle before open an other one.
            Close();

            //Storage type
            VIRTUAL_STORAGE_TYPE storageType = new VIRTUAL_STORAGE_TYPE();
            storageType.DeviceId = STORAGE_TYPE_DEVICE.VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
            storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR.VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;

            //Disk params
            OPEN_VIRTUAL_DISK_PARAMETERS diskParams = new OPEN_VIRTUAL_DISK_PARAMETERS();
            diskParams.Version = OPEN_VIRTUAL_DISK_VERSION.OPEN_VIRTUAL_DISK_VERSION_1;
            diskParams.RWDepth = depth;

            //Open vhd.
            iLastError = NativeMethods.OpenVirtualDisk(ref storageType, vhdPath, (VIRTUAL_DISK_ACCESS_MASK) mask,
                (OPEN_VIRTUAL_DISK_FLAG) flag, ref diskParams, ref hVirtualDisk);

            IsOpen = (hVirtualDisk == IntPtr.Zero) ? false : true;
            VhdFile = (hVirtualDisk == IntPtr.Zero) ? vhdPath : string.Empty;

            return iLastError == 0 ? true : false;
        }

        /// <summary>
        /// Expand an existing VHD file.
        /// </summary>
        /// <param name="newSize">The new size in bytes.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Expand(ulong newSize)
        {
            if (hVirtualDisk != IntPtr.Zero)
            {
                //Test if the VHD is not attached.
                if (attached)
                    throw new System.IO.IOException("Expanding a virtual disk requires that the virtual disk be detached during the operation.");

                //Set parameters
                EXPAND_VIRTUAL_DISK_PARAMETERS param = new EXPAND_VIRTUAL_DISK_PARAMETERS();
                param.Version = EXPAND_VIRTUAL_DISK_VERSION.EXPAND_VIRTUAL_DISK_VERSION_1;
                param.Version1.NewSize = newSize;

                //Expand disk.
                iLastError = NativeMethods.ExpandVirtualDisk(hVirtualDisk, EXPAND_VIRTUAL_DISK_FLAG.EXPAND_VIRTUAL_DISK_FLAG_NONE,
                    ref param, IntPtr.Zero);
            }

            if (iLastError == ERROR_INVALID_PARAMETER)
                throw new System.IO.IOException("The new size is less than the current size of the virtual disk.");

            return iLastError == 0 ? true : false;
        }

        /// <summary>
        /// Expand an existing VHD file.
        /// </summary>
        /// <param name="newSize">The new size in bytes.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public void ExpandAsync(ulong newSize)
        {
            if (hVirtualDisk != IntPtr.Zero)
            {
                //Test if the VHD is not attached.
                if (attached)
                {
                    iLastError = -1;
                    throw new System.IO.IOException("Expanding a virtual disk requires that the virtual disk be detached during the operation.");
                }

                //Set parameters
                EXPAND_VIRTUAL_DISK_PARAMETERS param = new EXPAND_VIRTUAL_DISK_PARAMETERS();
                param.Version = EXPAND_VIRTUAL_DISK_VERSION.EXPAND_VIRTUAL_DISK_VERSION_1;
                param.Version1.NewSize = newSize;

                AsyncExpandVhd = new VhdAsyncOperation(false, 0, 0);

                //Starting Expand disk.
                iLastError = NativeMethods.ExpandVirtualDisk(hVirtualDisk, EXPAND_VIRTUAL_DISK_FLAG.EXPAND_VIRTUAL_DISK_FLAG_NONE,
                    ref param, ref AsyncExpandVhd.Overlapped);

                //Report progress for the async operation.
                if ((iLastError == ERROR_SUCCESS) || (iLastError == ERROR_IO_PENDING))
                {
                    var progress = this.GetExpandProgress();
                    while (!progress.IsComplete)
                    {
                        if (ExpandVirtualDiskProgress != null)
                        {
                            ExpandVirtualDiskProgress(progress);

                            System.Threading.Thread.Sleep(10);
                            progress = this.GetExpandProgress();
                        }
                    }
                }
                else
                {
                    //Errors.
                    if ((iLastError == ERROR_FILE_NOT_FOUND) || (iLastError == ERROR_PATH_NOT_FOUND))
                        throw new System.IO.FileNotFoundException("File not found.");
                    else if (iLastError == ERROR_FILE_CORRUPT)
                        throw new System.IO.InvalidDataException("File format not recognized.");
                    else if (iLastError == ERROR_FILE_EXISTS)
                        throw new System.IO.IOException("File already exists.");
                    else if (iLastError == ERROR_INVALID_PARAMETER)
                        throw new System.ArgumentException("Invalid parameter.", "size");
                    else
                        throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
                }
            }
        }

        /// <summary>
        /// Close the VHD file.
        /// </summary>
        /// <returns></returns>
        public bool Close()
        {
            attached = false;

            if (hVirtualDisk != IntPtr.Zero)
                return NativeMethods.CloseHandle(hVirtualDisk);
            else
                return false;
        }

        /// <summary>
        /// Retrieves information about a virtual hard disk (VHD).
        /// </summary>
        /// <param name="request">Info to retrieve.</param>
        /// <returns>A <see cref="VhdInfos"/>.</returns>
        public VhdInfos GetVirtualDiskInfo(VHD_INFO_REQUEST request)
        {
            GET_VIRTUAL_DISK_INFO diskInfo = new GET_VIRTUAL_DISK_INFO();

            if (hVirtualDisk != IntPtr.Zero)
            {
                int vhdInfoSize = Marshal.SizeOf(typeof(GET_VIRTUAL_DISK_INFO));
                int vhdSizeUsed = 0;

                diskInfo.Version = (GET_VIRTUAL_DISK_INFO_VERSION)request;

                int infoRet = NativeMethods.GetVirtualDiskInformation(hVirtualDisk,
                    ref vhdInfoSize, ref diskInfo, ref vhdSizeUsed);
            }

            return (VhdInfos)diskInfo;
        }

        /// <summary>
        /// Retrieves information about a virtual hard disk (VHD).
        /// </summary>
        /// <param name="request">Info to retrieve.</param>
        /// <returns>An internal <see cref="GET_VIRTUAL_DISK_INFO"/> structure.</returns>
        GET_VIRTUAL_DISK_INFO GetVirtualDiskInfo(GET_VIRTUAL_DISK_INFO_VERSION request)
        {
            GET_VIRTUAL_DISK_INFO diskInfo = new GET_VIRTUAL_DISK_INFO();

            if (hVirtualDisk != IntPtr.Zero)
            {
                int vhdInfoSize = Marshal.SizeOf(typeof(GET_VIRTUAL_DISK_INFO));
                int vhdSizeUsed = 0;

                diskInfo.Version = request;

                int infoRet = NativeMethods.GetVirtualDiskInformation(hVirtualDisk,
                    ref vhdInfoSize, ref diskInfo, ref vhdSizeUsed);
            }

            return diskInfo;
        }

        /// <summary>
        /// Retrieves the path to the physical device object that contains a virtual hard disk (VHD).
        /// </summary>
        /// <returns>A string wich contains the path.</returns>
        public string GetPhysicalPath()
        {
            if (hVirtualDisk != IntPtr.Zero)
            {
                int pathSize = 255;
                StringBuilder szb = new StringBuilder(255);

                int ret = NativeMethods.GetVirtualDiskPhysicalPath(hVirtualDisk, ref pathSize, szb);

                return szb.ToString();
            }
            else
                return string.Empty;
        }

        /// <summary>
        /// Attaches a VHD.
        /// </summary>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Attach()
        {
            return this.Attach(VHD_ATTACH_FLAG.NONE);
        }

        /// <summary>
        ///  Attaches a VHD.
        /// </summary>
        /// <param name="readOnly">Attach the virtual disk as read-only.</param>
        /// <param name="noDriveLetter">Will cause all volumes on the attached virtual disk to be mounted without assigning drive letters to them.</param>
        /// <param name="noLocalHost">Do not use. Reserved</param>
        /// <param name="permanentLifetime">Will decouple the virtual disk lifetime from that of the open handle. The virtual disk will be attached until the DetachVirtualDisk function is called, even if all open handles to the virtual disk are closed.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Attach(bool readOnly, bool noDriveLetter, bool noLocalHost, bool permanentLifetime)
        {
            VHD_ATTACH_FLAG flag = VHD_ATTACH_FLAG.NONE;
            if (readOnly) flag |= VHD_ATTACH_FLAG.READ_ONLY;
            if (noDriveLetter) flag |= VHD_ATTACH_FLAG.NO_DRIVE_LETTER;
            if (noLocalHost) flag |= VHD_ATTACH_FLAG.NO_LOCAL_HOST;
            if (permanentLifetime) flag |= VHD_ATTACH_FLAG.PERMANENT_LIFETIME;

            return this.Attach(flag);
        }

        /// <summary>
        /// Attaches a VHD.
        /// </summary>
        /// <param name="flag">Virtual disk attach request flags.</param>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Attach(VHD_ATTACH_FLAG flag)
        {
            if (hVirtualDisk != IntPtr.Zero)
            {
                ATTACH_VIRTUAL_DISK_PARAMETERS param = new ATTACH_VIRTUAL_DISK_PARAMETERS();
                param.Version = ATTACH_VIRTUAL_DISK_VERSION.ATTACH_VIRTUAL_DISK_VERSION_1;

                iLastError = NativeMethods.AttachVirtualDisk(hVirtualDisk, IntPtr.Zero, (ATTACH_VIRTUAL_DISK_FLAG)flag,
                    0, ref param, IntPtr.Zero);

                attached = iLastError == 0 ? true : false;
                return iLastError == 0 ? true : false;
            }
            else
                return false;
        }

        /// <summary>
        /// Detaches a VHD.
        /// </summary>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Detach()
        {
            if (attached && hVirtualDisk != IntPtr.Zero)
            {
                iLastError = NativeMethods.DetachVirtualDisk(hVirtualDisk, DETACH_VIRTUAL_DISK_FLAG.DETACH_VIRTUAL_DISK_FLAG_NONE, 0);

                attached = iLastError == 0 ? true : false;
                return iLastError == 0 ? true : false;
            }
            else
                return false;
        }

        /// <summary>
        /// Compact a Virtual Disk Image file.
        /// </summary>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public bool Compact()
        {
            if (hVirtualDisk != IntPtr.Zero)
            {
                //Tests if the compact operation is valid on this VHD.
                if (attached)
                    throw new System.IO.IOException("Compaction of a virtual disk requires that the virtual disk be detached during the operation.");

                GET_VIRTUAL_DISK_INFO infoSize = GetVirtualDiskInfo(GET_VIRTUAL_DISK_INFO_VERSION.GET_VIRTUAL_DISK_INFO_SIZE);
                if (infoSize.Union.Size.PhysicalSize > infoSize.Union.Size.VirtualSize)
                    throw new System.IO.IOException("Compaction of a virtual disk requires that the virtual is dynamically expandable or differencing.");

                //Set parameters.
                COMPACT_VIRTUAL_DISK_PARAMETERS param = new COMPACT_VIRTUAL_DISK_PARAMETERS();
                param.Version = COMPACT_VIRTUAL_DISK_VERSION.COMPACT_VIRTUAL_DISK_VERSION_1;

                //Compact VHD.
                iLastError = NativeMethods.CompactVirtualDisk(hVirtualDisk, COMPACT_VIRTUAL_DISK_FLAG.COMPACT_VIRTUAL_DISK_FLAG_NONE,
                    ref param, IntPtr.Zero);

                return iLastError == 0 ? true : false;
            }
            else
                return false;
        }

        /// <summary>
        /// Compact a Virtual Disk Image file in asynchronous operation.
        /// </summary>
        /// <returns><b>True</b> if open;<b>False</b> if not.</returns>
        public void CompactAsync()
        {
            if (hVirtualDisk != IntPtr.Zero)
            {
                //Tests if the compact operation is valid on this VHD.
                if (attached)
                {
                    iLastError = -1;
                    throw new System.IO.IOException("Compaction of a virtual disk requires that the virtual disk be detached during the operation.");
                }

                GET_VIRTUAL_DISK_INFO infoSize = GetVirtualDiskInfo(GET_VIRTUAL_DISK_INFO_VERSION.GET_VIRTUAL_DISK_INFO_SIZE);
                if (infoSize.Union.Size.PhysicalSize > infoSize.Union.Size.VirtualSize)
                {
                    iLastError = -1;
                    throw new System.IO.IOException("Compaction of a virtual disk requires that the virtual is dynamically expandable or differencing.");
                }

                //Set parameters
                COMPACT_VIRTUAL_DISK_PARAMETERS param = new COMPACT_VIRTUAL_DISK_PARAMETERS();
                param.Version = COMPACT_VIRTUAL_DISK_VERSION.COMPACT_VIRTUAL_DISK_VERSION_1;

                //Overlapped structure for the asynchronous operation.
                AsyncCompactVhd = new VhdAsyncOperation(false, 0, 0);

                //Starting Compact VHD.
                iLastError = NativeMethods.CompactVirtualDisk(hVirtualDisk, COMPACT_VIRTUAL_DISK_FLAG.COMPACT_VIRTUAL_DISK_FLAG_NONE,
                    ref param, ref AsyncCompactVhd.Overlapped);

                //Report progress for the async operation.
                if ((iLastError == ERROR_SUCCESS) || (iLastError == ERROR_IO_PENDING))
                {
                    var progress = this.GetCompactProgress();
                    while (!progress.IsComplete)
                    {
                        if (CompactVirtualDiskProgress != null)
                        {
                            CompactVirtualDiskProgress(progress);

                            System.Threading.Thread.Sleep(10);
                            progress = this.GetCompactProgress();
                        }
                    }
                }
                else
                {
                    if ((iLastError == ERROR_FILE_NOT_FOUND) || (iLastError == ERROR_PATH_NOT_FOUND))
                        throw new System.IO.FileNotFoundException("File not found.");
                    else if (iLastError == ERROR_FILE_CORRUPT)
                        throw new System.IO.InvalidDataException("File format not recognized.");
                    else if (iLastError == ERROR_FILE_EXISTS)
                        throw new System.IO.IOException("File already exists.");
                    else if (iLastError == ERROR_INVALID_PARAMETER)
                        throw new System.ArgumentException("Invalid parameter.", "size");
                    else
                        throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
                }
            }
        }

        /// <summary>
        /// Retrieves the last error code in <see cref="VHD"/>.
        /// </summary>
        /// <returns>Error code.</returns>
        public static int GetLastError()
        {
            return iLastError;
        }

        /// <summary>
        /// Retrieves the Create operation progress infos (async.)
        /// </summary>
        /// <returns>A <see cref="VirtualDiskOperationProgress"/>.</returns>
        private static VirtualDiskOperationProgress GetCreateProgress(IntPtr newVhdHandle)
        {
            VIRTUAL_DISK_PROGRESS progress = new VIRTUAL_DISK_PROGRESS();

            //Get progress.
            iLastError = NativeMethods.GetVirtualDiskOperationProgress(newVhdHandle,
                ref AsyncCreationVhd.Overlapped, ref progress);

            if (iLastError == ERROR_SUCCESS)
            {
                //Report it.
                iLastError = progress.OperationStatus;
                if (iLastError == ERROR_SUCCESS)
                    return new VirtualDiskOperationProgress(100, true);
                else if (iLastError == ERROR_IO_PENDING)
                {
                    if (AsyncCreationVhd.OverlappedEvent.WaitOne(0))
                        return new VirtualDiskOperationProgress(100, true);
                    else
                        return new VirtualDiskOperationProgress((int)((progress.CurrentValue * 100) / progress.CompletionValue), false);
                }
                else
                {
                    //...on error.
                    if ((iLastError == ERROR_FILE_NOT_FOUND) || (iLastError == ERROR_PATH_NOT_FOUND))
                        throw new System.IO.FileNotFoundException("File not found.");
                    else if (iLastError == ERROR_FILE_CORRUPT)
                        throw new System.IO.InvalidDataException("File format not recognized.");
                    else if (iLastError == ERROR_FILE_EXISTS)
                        throw new System.IO.IOException("File already exists.");
                    else
                        throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
                }
            }
            else
            {
                throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
            }
        }

        /// <summary>
        /// Retrieves the Compact operation progress infos (async.)
        /// </summary>
        /// <returns>A <see cref="VirtualDiskOperationProgress"/>.</returns>
        private VirtualDiskOperationProgress GetCompactProgress()
        {
            VIRTUAL_DISK_PROGRESS progress = new VIRTUAL_DISK_PROGRESS();

            //Get progress.
            iLastError = NativeMethods.GetVirtualDiskOperationProgress(hVirtualDisk,
                ref AsyncCompactVhd.Overlapped, ref progress);

            if (iLastError == ERROR_SUCCESS)
            {
                //Report it.
                iLastError = progress.OperationStatus;
                if (iLastError == ERROR_SUCCESS)
                    return new VirtualDiskOperationProgress(100, true);
                else if (iLastError == ERROR_IO_PENDING)
                {
                    if (AsyncCompactVhd.OverlappedEvent.WaitOne(0))
                        return new VirtualDiskOperationProgress(100, true);
                    else
                        return new VirtualDiskOperationProgress((int)((progress.CurrentValue * 100) / progress.CompletionValue), false);
                }
                else
                {
                    //...on error.
                    if ((iLastError == ERROR_FILE_NOT_FOUND) || (iLastError == ERROR_PATH_NOT_FOUND))
                        throw new System.IO.FileNotFoundException("File not found.");
                    else if (iLastError == ERROR_FILE_CORRUPT)
                        throw new System.IO.InvalidDataException("File format not recognized.");
                    else if (iLastError == ERROR_FILE_EXISTS)
                        throw new System.IO.IOException("File already exists.");
                    else
                        throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
                }
            }
            else
            {
                throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
            }
        }

        /// <summary>
        /// Retrieves the Expand operation progress infos (async.)
        /// </summary>
        /// <returns>A <see cref="VirtualDiskOperationProgress"/>.</returns>
        private VirtualDiskOperationProgress GetExpandProgress()
        {
            VIRTUAL_DISK_PROGRESS progress = new VIRTUAL_DISK_PROGRESS();

            //Get progress.
            iLastError = NativeMethods.GetVirtualDiskOperationProgress(hVirtualDisk,
                ref AsyncExpandVhd.Overlapped, ref progress);

            if (iLastError == ERROR_SUCCESS)
            {
                //Report it.
                iLastError = progress.OperationStatus;
                if (iLastError == ERROR_SUCCESS)
                    return new VirtualDiskOperationProgress(100, true);
                else if (iLastError == ERROR_IO_PENDING)
                {
                    if (AsyncExpandVhd.OverlappedEvent.WaitOne(0))
                        return new VirtualDiskOperationProgress(100, true);
                    else
                        return new VirtualDiskOperationProgress((int)((progress.CurrentValue * 100) / progress.CompletionValue), false);
                }
                else
                {
                    //...on error.
                    if ((iLastError == ERROR_FILE_NOT_FOUND) || (iLastError == ERROR_PATH_NOT_FOUND))
                        throw new System.IO.FileNotFoundException("File not found.");
                    else if (iLastError == ERROR_FILE_CORRUPT)
                        throw new System.IO.InvalidDataException("File format not recognized.");
                    else if (iLastError == ERROR_FILE_EXISTS)
                        throw new System.IO.IOException("File already exists.");
                    else
                        throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
                }
            }
            else
            {
                throw new InvalidOperationException(string.Format(
                            CultureInfo.InvariantCulture, "Native error {0}.", iLastError));
            }
        }

        #region IDisposable Members

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            // Check to see if Dispose has already been called.
            if (!this.disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.
                if (disposing)
                {
                    // Dispose managed resources.
                }

                // Call the appropriate methods to clean up
                // unmanaged resources here.
                // If disposing is false,
                // only the following code is executed.
                Close();

                // Note disposing has been done.
                disposed = true;
            }
        }

        #endregion
    }
}
