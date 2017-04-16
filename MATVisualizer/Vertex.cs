using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Vector3 = System.Numerics.Vector3;
using System.Runtime.InteropServices;

namespace MATVisualizer
{
    /// <summary>
    /// 頂点データ
    /// </summary>
    public class Vertex
    {
        /// <summary>
        /// 頂点の座標
        /// </summary>
        public Vector3 position;

        /// <summary>
        /// 頂点データのバイト数
        /// </summary>
        public static int SizeInBytes
        {
            get { return Marshal.SizeOf(typeof(Vertex)); }
        }
    }
}
