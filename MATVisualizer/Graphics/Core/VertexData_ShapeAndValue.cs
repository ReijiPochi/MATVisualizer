using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct VertexData_ShapeAndValue
    {
        public Vector3 SV_Position;
        public uint GC_DataIndex1;
    }
}
