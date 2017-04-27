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
        public Object3D(ref GraphicsObjectDescription desc)
        {
            GraphicsCore.GetDevice();
            handle = GraphicsObject.Create(ref desc);
            GraphicsCore.AddToRenderingList(handle);
        }

        public IntPtr handle;

        public VerticesDataBase Vertices { get; set; }

        /// <summary>
        /// 頂点データをグラフィックメモリに転送します。
        /// </summary>
        public void DownloadVerticesToGPU()
        {
            if (Vertices != null)
                GraphicsObject.SetVertices(handle, Vertices.Pointer, 3);
        }

        public void SetBuffer(BufferResource buffer, int slot = 0)
        {
            GraphicsObject.SetBuffer(handle, slot, buffer.handle);
        }
    }
}
