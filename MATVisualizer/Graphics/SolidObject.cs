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

        }

        public SolidObject(Graphics.Core.GraphicsObjectDescription desc)
        {
            Create(desc);
            Unlock();
        }

        /// <summary>
        /// このオブジェクトのShapeデータです。
        /// </summary>
        public Shape[] Shapes { get; protected set; } = new Shape[8];

        public Buffer Buffer { get; set; }

        public void SetShapes()
        {
            for(int i = 0; i < 8; i++)
            {
                if(Shapes[i] != null)
                {
                    SetShape(Shapes[i], i);
                }
            }
        }
    }
}
