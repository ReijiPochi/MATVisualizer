using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public class _Shape
    {
        [DllImport("GraphicsCore.dll", EntryPoint = "Shape_Create", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Create(VertexType type);

        [DllImport("GraphicsCore.dll", EntryPoint = "Shape_Set", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern HRESULT Set(IntPtr shape, IntPtr vertex, uint numVertex, IntPtr index, uint numIndex);

        [DllImport("GraphicsCore.dll", EntryPoint = "Shape_Update", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void Update(IntPtr shape, IntPtr vertex, uint numVertex, IntPtr index, uint numIndex);
    }
}
