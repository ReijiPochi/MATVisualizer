using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{
    public abstract class Object3D
    {
        public IntPtr objectHandle;

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

        public abstract void DownloadShape();

        public void SetBuffer(BufferResource buffer, int slot = 0)
        {
            GraphicsObject.SetBuffer(objectHandle, slot, buffer.handle);
        }
    }
}
