using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{
    /// <summary>
    /// Direct3Dで描画されるオブジェクトの抽象クラスです。
    /// </summary>
    public abstract class Object3D
    {
        /// <summary>
        /// GraphicsCore によって生成されたハンドルです。
        /// </summary>
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

        public void SetShape(Shape shape, int slot)
        {
            GraphicsObject.SetShape(objectHandle, slot, shape.shapeHandle);
        }

        public void SetBuffer(BufferResource buffer, int slot = 0)
        {
            GraphicsObject.SetBuffer(objectHandle, slot, buffer.handle);
        }
    }
}
