﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class IndicesData<T> : IndicesData
    {
        public IndicesData(T[] _data)
        {
            if (_data != null)
            {
                data = _data;
                handle = GCHandle.Alloc(data, GCHandleType.Pinned);
                Pointer = handle.AddrOfPinnedObject();
                NumIndices = (uint)data.LongLength;
            }
        }

        public T[] data;
        private GCHandle handle;

        public override void Dispose()
        {
            if (disposed)
                return;

            base.Dispose();

            if (handle != null && handle.IsAllocated)
                handle.Free();

            data = null;
        }
    }
}
