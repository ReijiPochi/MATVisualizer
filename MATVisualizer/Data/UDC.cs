using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Vector3 = System.Numerics.Vector3;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDC ファイル（単一ステップ）のデータを格納します。
    /// </summary>
    public class UDC
    {
        public UDC()
        {

        }

        /// <summary>
        /// UDCデータのコメントです。
        /// </summary>
        public string Comment { get; set; }

        /// <summary>
        /// UDCデータのステップ数です。
        /// </summary>
        public int StepCount { get; set; }

        /// <summary>
        /// UDCデータの繰り返しタイプです。
        /// </summary>
        public UDCDataRepetitionType DataRepetitionType { get; set; }

        /// <summary>
        /// UDCデータの節点の数です。
        /// </summary>
        public int NodeCount { get; set; }

        /// <summary>
        /// UDCデータの要素の数です。
        /// </summary>
        public int CellCount { get; set; }

        /// <summary>
        /// UDCデータの節点が持つデータ成分の数です。
        /// </summary>
        public int NodeComponentCount { get; set; }

        /// <summary>
        /// UDCデータの要素が持つデータ成分の数です。
        /// </summary>
        public int CellComponentCount { get; set; }

        /// <summary>
        /// UDCデータの節点です。
        /// </summary>
        public UDCNode[] Nodes { get; set; }

        /// <summary>
        /// UDCデータの要素です。
        /// </summary>
        public UDCCell[] Cells { get; set; }

        /// <summary>
        /// Points と Elements のメモリを確保します。
        /// </summary>
        public void InitializeArrayOfPointAndElement()
        {
            if (NodeCount != 0)
                Nodes = new UDCNode[NodeCount];

            if (CellCount != 0)
                Cells = new UDCCell[CellCount];
        }
    }
}
