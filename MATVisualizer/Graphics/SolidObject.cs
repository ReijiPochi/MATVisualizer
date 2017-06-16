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
        }

        public SolidObject(Graphics.Core.GraphicsObjectDescription desc)
        {
            Shapes = new ShapeCollection(this);
            Create(desc);
            Unlock();
        }

        /// <summary>
        /// このオブジェクトのShapeデータです。
        /// </summary>
        public ShapeCollection Shapes { get; protected set; }

        public Buffer Buffer { get; set; }
    }
}
