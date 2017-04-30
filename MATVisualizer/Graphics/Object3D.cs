using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{
    public class Object3D
    {
        protected Object3D()
        {

        }

        public Object3D(GraphicsObjectDescription desc)
        {
            Create(desc);
        }

        public IntPtr objectHandle;

        public VerticesData Vertices { get; set; }

        public IndicesData Indices { get; set; }

        protected void Create(GraphicsObjectDescription desc)
        {
            objectHandle = GraphicsObject.Create(desc);
        }

        public void Lock()
        {
            if (objectHandle != IntPtr.Zero)
                GraphicsObject.SetLock(objectHandle, true);
        }

        public void Unlock()
        {
            if (objectHandle != IntPtr.Zero)
                GraphicsObject.SetLock(objectHandle, false);
        }

        /// <summary>
        /// 頂点データをグラフィックメモリに転送します。
        /// </summary>
        public void DownloadVerticesToGPU()
        {
            if (Vertices != null)
                GraphicsObject.SetVertices(objectHandle, Vertices.Pointer, Vertices.NumVertices);
        }

        public void DownloadIndicesToGPU()
        {
            if (Indices != null)
            {
                GraphicsObject.SetIndices(objectHandle, Indices.Pointer, Indices.NumIndices);
            }
        }

        public void SetBuffer(BufferResource buffer, int slot = 0)
        {
            GraphicsObject.SetBuffer(objectHandle, slot, buffer.handle);
        }
    }
}
