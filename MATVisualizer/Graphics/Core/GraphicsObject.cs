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

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetTexture", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetTexture(IntPtr obj, int slot, IntPtr texture);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetBuffer", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetBuffer(IntPtr obj, int slot, IntPtr buffer);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetShape", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetShape(IntPtr obj, int slot, IntPtr shape);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetLock", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetLock(IntPtr obj, bool locking);

        [DllImport("GraphicsCore.dll", EntryPoint = "GrapgicsObject_GetShape", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr GetShape(IntPtr obj, int slot);

        [DllImport("GraphicsCore.dll", EntryPoint = "GrapgicsObject_GetBuffer", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr GetBuffer(IntPtr obj, int slot);
    }
}
