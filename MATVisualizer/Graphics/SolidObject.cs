using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    /// <summary>
    /// レンダリング可能な、通常の3Dオブジェクトです。
    /// </summary>
    public class SolidObject : Object3D
    {
        public SolidObject()
        {
            Shapes = new ShapeCollection(this);
            Buffers = new BufferCollection(this);
        }

        public SolidObject(Graphics.Core.GraphicsObjectDescription desc)
        {
            Shapes = new ShapeCollection(this);
            Buffers = new BufferCollection(this);
            Create(desc);
            Unlock();
        }

        /// <summary>
        /// このオブジェクトのShapeデータです。
        /// </summary>
        public ShapeCollection Shapes { get; protected set; }

        /// <summary>
        /// このオブジェクトのBufferデータです。
        /// </summary>
        public BufferCollection Buffers { get; protected set; }
    }
}
