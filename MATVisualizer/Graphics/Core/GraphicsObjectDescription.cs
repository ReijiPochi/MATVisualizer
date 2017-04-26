using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct GraphicsObjectDescription
    {
        public VertexType vertexType;
        public D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
    }
}
