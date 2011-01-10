using System;

namespace VhdWrapper
{
    public struct VirtualDiskOperationProgress
    {
        public int ProgressPercentage { get; private set; }
        public bool IsComplete { get; private set; }

        public VirtualDiskOperationProgress(int progressPercentage, bool complete)
            :this()
        {
            this.ProgressPercentage = progressPercentage;
            this.IsComplete = complete;
        }
    }
}
