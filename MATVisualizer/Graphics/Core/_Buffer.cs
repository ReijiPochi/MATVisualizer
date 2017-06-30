using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public class _Buffer
    {
        [DllImport("GraphicsCore.dll", EntryPoint = "Buffer_Create", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Create(ref BufferDescription desc, IntPtr data);

        [DllImport("GraphicsCore.dll", EntryPoint = "Buffer_Release", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void Release(IntPtr buffer);
    }
}
