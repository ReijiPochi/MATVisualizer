using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCCellTypeに関する拡張メソッドを定義しています。
    /// </summary>
    public static class UDCCellTypeExt
    {
        /// <summary>
        /// 各UDCCellTypeの成分数を取得します
        /// </summary>
        /// <param name="type">成分数を取得したいUDCElementType</param>
        /// <returns>成分数</returns>
        public static int GetNumberOfValues(this UDCCellType type)
        {
            switch (type)
            {
                case UDCCellType.Point:
                    return 1;

                case UDCCellType.Line:
                    return 2;

                case UDCCellType.Triangle:
                    return 3;

                case UDCCellType.Tetra:
                case UDCCellType.Quadrilateral:
                    return 4;

                case UDCCellType.QuadrangulrPyramid:
                    return 5;

                case UDCCellType.Hexahedron:
                    return 8;

                case UDCCellType.Prism:
                    return 6;

                default:
                    return 0;
            }
        }

        /// <summary>
        /// 文字列をUDCCellTypeに変換します。
        /// </summary>
        /// <param name="text">変換する文字列</param>
        /// <returns>変換結果</returns>
        public static UDCCellType ParseToUDCCellType(this string text)
        {
            switch (text)
            {
                case "pt":
                    return UDCCellType.Point;

                case "line":
                    return UDCCellType.Line;

                case "tri":
                    return UDCCellType.Triangle;

                case "quad":
                    return UDCCellType.Quadrilateral;

                case "tet":
                    return UDCCellType.Tetra;

                case "pyr":
                    return UDCCellType.QuadrangulrPyramid;

                case "hex":
                    return UDCCellType.Hexahedron;

                case "prism":
                    return UDCCellType.Prism;

                default:
                    throw new Exception("\"" + text + "\" はUDCCellTypeに変換できません。");
            }
        }
    }
}
