using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{

    public class GraphicsObject
    {
        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_Create", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Create(GraphicsObjectDescription desc);

        //[DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetVertices", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        //public static extern HRESULT SetVertices(IntPtr obj, IntPtr data, uint length);

        //[DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetIndices", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        //public static extern HRESULT SetIndices(IntPtr obj, IntPtr data, uint length);

        //[DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetShape", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        //public static extern HRESULT SetShape(IntPtr obj, int slot, IntPtr vertex, uint numVertex, IntPtr index, uint numIndex);

        //[DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_UpdateShape", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        //public static extern void UpdateShape(IntPtr obj, int slot, IntPtr vertex, uint numVertex, IntPtr index, uint numIndex);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetTexture", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetTexture(IntPtr obj, int slot, IntPtr texture);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetBuffer", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetBuffer(IntPtr obj, int slot, IntPtr buffer);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetShape", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetShape(IntPtr obj, int slot, IntPtr shape);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetLock", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetLock(IntPtr obj, bool locking);
    }
}
