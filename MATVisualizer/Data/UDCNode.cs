namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCデータの節点です。
    /// </summary>
    public class UDCNode
    {
        public UDCNode()
        {

        }

        /// <summary>
        /// 節点の座標です。
        /// </summary>
        public System.Numerics.Vector3 Coord { get; set; }

        /// <summary>
        /// 節点のデータです。
        /// </summary>
        public float[] Data { get; set; }
    }
}
