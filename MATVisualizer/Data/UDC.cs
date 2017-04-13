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
        public int PointCount { get; set; }

        /// <summary>
        /// UDCデータの要素の数です。
        /// </summary>
        public int ElementCount { get; set; }

        /// <summary>
        /// UDCデータの節点です。
        /// </summary>
        public Vector3[] Points { get; set; }

        /// <summary>
        /// UDCデータの要素です。
        /// </summary>
        public UDCElement[] Elements { get; set; }

        /// <summary>
        /// Points と Elements のメモリを確保します。
        /// </summary>
        public void InitializeArrayOfPointAndElement()
        {
            if (PointCount != 0)
                Points = new Vector3[PointCount];

            if (ElementCount != 0)
                Elements = new UDCElement[ElementCount];
        }
    }
}
