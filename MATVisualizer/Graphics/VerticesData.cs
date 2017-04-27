using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{
    public class VerticesData<T> : VerticesDataBase
    {
        public VerticesData(T[] _data)
        {
            if (_data != null)
            {
                data = _data;
                GCHandle handle = GCHandle.Alloc(data, GCHandleType.Pinned);
                Pointer = handle.AddrOfPinnedObject();
            }
        }

        private T[] data;
        private GCHandle handle;

        public override void Dispose()
        {
            if (handle != null)
                handle.Free();
        }
    }
}
