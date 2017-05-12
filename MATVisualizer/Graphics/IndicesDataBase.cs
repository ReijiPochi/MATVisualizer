using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public abstract class IndicesData : IDisposable
    {
        public IntPtr Pointer { get; protected set; }
        public uint NumIndices { get; set; }
        public abstract void Dispose();
    }
}
