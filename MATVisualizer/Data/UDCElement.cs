using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCデータの要素です。
    /// </summary>
    public class UDCElement
    {
        /// <summary>
        /// UDCElementを材料（番号）と要素の種類および節点（番号）から初期化します。
        /// </summary>
        /// <param name="type"></param>
        /// <param name="values"></param>
        public UDCElement(int material, UDCElementType type, params int[] values)
        {
            if (values.Length != type.GetNumberOfValues())
                throw new Exception("UDCElementTypeと与えられた節点の数が一致しません。");

            ElementType = type;
            Values = values;
        }

        /// <summary>
        /// UDCElementの材料（番号）です。
        /// </summary>
        public int MaterialNumber { get; protected set; }

        /// <summary>
        /// UDCElementの要素形状です。
        /// </summary>
        public UDCElementType ElementType { get; protected set; }

        /// <summary>
        /// UDCElementの節点（番号）です。
        /// </summary>
        public int[] Values { get; protected set; }
    }
}
