using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public class GraphicsCore
    {
        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_Initialize", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern int Initialize(GraphicsCoreDescription desc);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_GetDevice", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr GetDevice();

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_AddToRenderingList", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void AddToRenderingList(IntPtr obj);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_SetCamera", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetCamera(IntPtr camera);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_Resize", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern HRESULT Resize(int width, int height);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_Finalize", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void GraphicsCore_Finalize();

        [DllImport("GraphicsCore.dll", EntryPoint = "TEST", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Test(ref GraphicsObjectDescription desc);
    }
}
