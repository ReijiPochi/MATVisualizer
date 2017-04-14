using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class GraphicsCore
    {
        [DllImport("GraphicsCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Initialize(IntPtr handle);
    }
}
