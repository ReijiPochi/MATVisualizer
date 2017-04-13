using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCElementTypeに関する拡張メソッドを定義しています。
    /// </summary>
    public static class UDCElementTypeExt
    {
        /// <summary>
        /// 各UDCElementTypeの成分数を取得します
        /// </summary>
        /// <param name="type">成分数を取得したいUDCElementType</param>
        /// <returns>成分数</returns>
        public static int GetNumberOfValues(this UDCElementType type)
        {
            switch (type)
            {
                case UDCElementType.Point:
                    return 1;

                case UDCElementType.Line:
                    return 2;

                case UDCElementType.Triangle:
                    return 3;

                case UDCElementType.Tetra:
                case UDCElementType.Quadrilateral:
                    return 4;

                case UDCElementType.QuadrangulrPyramid:
                    return 5;

                case UDCElementType.Hexahedron:
                    return 8;

                case UDCElementType.Prism:
                    return 6;

                default:
                    return 0;
            }
        }

        /// <summary>
        /// 文字列をUDCElementTypeに変換します。
        /// </summary>
        /// <param name="text">変換する文字列</param>
        /// <returns>変換結果</returns>
        public static UDCElementType ParseToUDCElementType(this string text)
        {
            switch (text)
            {
                case "pt":
                    return UDCElementType.Point;

                case "line":
                    return UDCElementType.Line;

                case "tri":
                    return UDCElementType.Triangle;

                case "quad":
                    return UDCElementType.Quadrilateral;

                case "tet":
                    return UDCElementType.Tetra;

                case "pyr":
                    return UDCElementType.QuadrangulrPyramid;

                case "hex":
                    return UDCElementType.Hexahedron;

                case "prism":
                    return UDCElementType.Prism;

                default:
                    throw new Exception("\"" + text + "\" はUDCElementTypeに変換できません。");
            }
        }
    }
}
