using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct GraphicsCoreDescription
    {
        public IntPtr handle;
        public int width;
        public int height;
    }
}
