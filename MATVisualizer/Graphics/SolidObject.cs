using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
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

        public Shape[] Shapes { get; set; } = new Shape[8];

        public BufferResource Buffer { get; set; }

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
