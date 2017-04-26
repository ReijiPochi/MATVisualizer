using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MATVisualizer.Graphics.Core;

namespace MATVisualizer.Graphics
{
    public class Object3D
    {
        public Object3D(GraphicsObjectDescription desc)
        {
            GraphicsCore.GetDevice();
            ptr = GraphicsObject.Create(ref desc);
        }

        private IntPtr ptr;

        /// <summary>
        /// 頂点データをグラフィックメモリに転送します。
        /// </summary>
        public void DownloadToGPU()
        {
            
        }
    }
}
