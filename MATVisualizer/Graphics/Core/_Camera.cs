using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public class _Camera
    {
        [DllImport("GraphicsCore.dll", EntryPoint = "Camera_Create", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Create();

        [DllImport("GraphicsCore.dll", EntryPoint = "Camera_Update", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Update(IntPtr camera, ref Matrix4x4 matrix);
    }
}
