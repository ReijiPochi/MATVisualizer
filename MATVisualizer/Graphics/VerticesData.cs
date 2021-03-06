﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{
    public class VerticesData<T> : VerticesData
    {
        public VerticesData(T[] _data)
        {
            if (_data != null)
            {
                data = _data;
                Type = VertexType.Null.FromVertexDataT(typeof(T));
                handle = GCHandle.Alloc(data, GCHandleType.Pinned);
                Pointer = handle.AddrOfPinnedObject();
                NumVertices = (uint)data.LongLength;
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
