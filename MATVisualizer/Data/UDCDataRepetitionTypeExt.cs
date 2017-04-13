using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCDataRepetitionTypeに関する拡張メソッドを定義しています。
    /// </summary>
    public static class UDCDataRepetitionTypeExt
    {
        /// <summary>
        /// 文字列をUDCDataRepetitionTypeに変換します。
        /// </summary>
        /// <param name="text">変換する文字列</param>
        /// <returns>変換結果</returns>
        public static UDCDataRepetitionType ParseToUDCDataRepetitionType(this string text)
        {
            switch (text)
            {
                case "data":
                    return UDCDataRepetitionType.Data;

                case "geom":
                    return UDCDataRepetitionType.Geometry;

                case "data_geom":
                    return UDCDataRepetitionType.DataAndGeometry;

                default:
                    throw new Exception("\"" + text + "\" はUDCDataRepetitionTypeに変換できません。");
            }
        }
    }
}
