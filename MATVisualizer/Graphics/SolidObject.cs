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

        public VerticesData Vertices { get; set; }

        public IndicesData Indices { get; set; }

        public BufferResource Buffer { get; set; }

        public override void DownloadShape()
        {
            if(Vertices != null && Indices != null)
            {
                Core.GraphicsObject.SetShape(objectHandle, 0, Vertices.Pointer, Vertices.NumVertices, Indices.Pointer, Indices.NumIndices);
            }
            else if(Vertices != null)
            {
                Core.GraphicsObject.SetShape(objectHandle, 0, Vertices.Pointer, Vertices.NumVertices, IntPtr.Zero, 0);
            }
        }

        public void UpdateIndex()
        {
            Core.GraphicsObject.UpdateShape(objectHandle, 0, IntPtr.Zero, 0, Indices.Pointer, Indices.NumIndices);
        }
    }
}
