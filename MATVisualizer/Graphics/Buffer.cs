using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{

    public class Buffer : DisposableObject
    {
        public Buffer(ref BufferDescription desc, object data)
        {
            GCHandle ptr = GCHandle.Alloc(data, GCHandleType.Pinned);
            handle = _Buffer.Create(ref desc, ptr.AddrOfPinnedObject());
            ptr.Free();
        }

        public IntPtr handle;

        public override void Dispose()
        {
            if (disposed)
                return;

            _Buffer.Release(handle);
            handle = IntPtr.Zero;

            base.Dispose();
        }
    }
}
