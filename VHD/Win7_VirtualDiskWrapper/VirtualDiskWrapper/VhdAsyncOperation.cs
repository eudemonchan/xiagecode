using System;
using System.Threading;

namespace VhdWrapper
{
    public struct VhdAsyncOperation
    {
        public ManualResetEvent OverlappedEvent;
        public NativeOverlapped Overlapped;

        public VhdAsyncOperation(bool initState, int offsetLow, int offsetHigh)
        {
            OverlappedEvent = new ManualResetEvent(initState);
            Overlapped = new NativeOverlapped();
            Overlapped.EventHandle = OverlappedEvent.Handle;    //SafeWaitHandle???
            Overlapped.OffsetHigh = offsetHigh;
            Overlapped.OffsetLow = offsetLow;
        }
    }
}
