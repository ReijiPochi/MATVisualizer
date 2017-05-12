using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public abstract class VerticesData : IDisposable
    {
        public IntPtr Pointer { get; protected set; }
        public uint NumVertices { get; set; }
        public VertexType Type { get; set; }
        public abstract void Dispose();
    }
}
