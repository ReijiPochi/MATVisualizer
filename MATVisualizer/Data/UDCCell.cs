using System;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCデータの要素です。
    /// </summary>
    public class UDCCell
    {
        /// <summary>
        /// UDCCellを材料（番号）と要素の種類および節点（番号）から初期化します。
        /// </summary>
        /// <param name="material">材料番号</param>
        /// <param name="type">種類</param>
        /// <param name="nodes">要素を構成する節点番号</param>
        public UDCCell(int material, UDCCellType type, params int[] nodes)
        {
            if (nodes.Length != type.GetNumberOfValues())
                throw new Exception("UDCElementTypeと与えられた節点の数が一致しません。");

            MaterialNumber = material;
            CellType = type;
            Nodes = nodes;
        }

        /// <summary>
        /// UDCElementの材料（番号）です。
        /// </summary>
        public int MaterialNumber { get; set; }

        /// <summary>
        /// UDCElementの要素形状です。
        /// </summary>
        public UDCCellType CellType { get; set; }

        /// <summary>
        /// UDCElementの節点（番号）です。
        /// </summary>
        public int[] Nodes { get; set; }

        /// <summary>
        /// 要素のデータです。
        /// </summary>
        public float[] Data { get; set; }
    }
}
