using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Data
{
    /// <summary>
    /// UDCデータを読み込みます。
    /// </summary>
    public class UDCLoader
    {
        /// <summary>
        /// ファイルからUDCデータを読み込みます。
        /// </summary>
        /// <param name="path">UDCファイルのパス</param>
        /// <returns>読み込まれたUDCデータ</returns>
        public static UDC Load(string path)
        {
            UDC data = new UDC();
            LoadingState state = LoadingState.Comment;

            using (StreamReader sr = new StreamReader(path))
            {
                bool result = true;
                string line = null;

                // ファイルを最後まで読み込む。
                while (!sr.EndOfStream)
                {
                    if (result)
                    {
                        // 1行読み込む。
                        line = sr.ReadLine();

                        // 行が読み込めていない場合、もしくは何も記述されていない場合読み飛ばす。
                        if (line == null || line.Length == 0)
                            continue;
                    }

                    switch (state)
                    {
                        case LoadingState.Comment:
                            result = LoadComment(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.StepCount:
                            result = LoadStepCount(data, line);
                            if (data.StepCount != 1)
                                throw new Exception("UDCLoaderクラスは複数ステップのUDCファイルをサポートしていません。");
                            state++;
                            break;

                        case LoadingState.DataRepetitionType:
                            result = LoadDataRepetitionType(data, line);
                            state++;
                            break;

                        case LoadingState.StepNumber:
                            result = LoadStepNumber(data, line);
                            state++;
                            break;

                        case LoadingState.PointAndElementCount:
                            result = LoadPointAndElementCount(data, line);
                            if (result) data.InitializeArrayOfPointAndElement();
                            state++;
                            break;

                        case LoadingState.Points:
                            result = LoadPoints(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.Elements:
                            result = LoadElements(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.PointAndElementDataCount:
                            return data;

                        default:
                            break;
                    }
                }
            }

            return data;
        }

        /// <summary>
        /// UDCファイルの読み込みステートを表します。
        /// </summary>
        private enum LoadingState
        {
            /// <summary>
            /// "#"で始まる⾏はコメントとして処理されます。
            /// コメント⾏は省略できます。
            /// また、ステップ数より後の⾏に記述する事はできません。
            /// </summary>
            Comment,

            /// <summary>
            /// 総ステップ数を記述します。
            /// 複数ステップでない場合も、必ず "1" を記述します。
            /// </summary>
            StepCount,

            /// <summary>
            /// 複数ステップの場合に、「データの変化の仕⽅」を定義語を使って指定します。
            /// 定義語は、データ成分値および形状 （座標情報） が変化するか否かにより、
            ///  data / geom / data_geom 
            /// の３つから選択します。
            /// 複数ステップでない場合は何れを選択してもかまいませんが、何れかを必ず記述しなければなりません。
            /// </summary>
            DataRepetitionType,

            /// <summary>
            /// 各ステップの先頭⾏（開始位置）を⽰します。
            /// 複数ステップでない場合も "step1" を記述します。
            /// </summary>
            StepNumber,

            /// <summary>
            /// 全節点数、全要素数を記述します。
            /// </summary>
            PointAndElementCount,

            /// <summary>
            /// 節点番号、X/Y/Z座標値を 1⾏1節点で記述します。
            /// </summary>
            Points,

            /// <summary>
            /// 要素に関する定義を1⾏1要素で記述します。
            /// 材料番号は、要素の集合をでグループ分けするのに⽤います。
            /// </summary>
            Elements,

            /// <summary>
            /// 節点データのデータ成分数と、それぞれの成分が「いくつの成分から構成されるか」 を記述します。
            /// </summary>
            PointAndElementDataCount,

            /// <summary>
            /// 節点データ各成分のラベルと単位を指定します。
            /// この指定は省略することもできますが、指定しない場合でも区切りの ","（カンマ）が必ず必要になります。
            /// </summary>
            PointDataLabelAndUnit,

            /// <summary>
            /// 1⾏1節点で、節点番号と対応する節点データを並べて記述します。
            /// </summary>
            PointData,

            /// <summary>
            /// 要素データのデータ成分数と、それぞれの成分が「いくつの成分から構成されるか」 を記述します。
            /// </summary>
            ElementAndElementDataCount,

            /// <summary>
            /// 要素データ各成分のラベルと単位を指定します。
            /// この指定は省略することもできますが、指定しない場合でも区切りの ","（カンマ）が必ず必要になります。
            /// </summary>
            ElementDataLabelAndUnit,

            /// <summary>
            /// 要素番号と対応する要素データを並べて記述します。
            /// </summary>
            ElementData
        }

        /// <summary>
        /// コメントを読み込みます。存在しない場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:コメントを読み込んだ</returns>
        private static bool LoadComment(UDC data, string line)
        {
            // #から始まる行はコメント
            if(line.Length > 1 && line[0] == '#')
            {
                int startIndex = 1;

                // #の後が半角スペースの場合、読み込まないようにする。
                for (int i = 1; i < line.Length; i++)
                {
                    if (line[i] == ' ')
                    {
                        startIndex++;
                    }
                    else
                    {
                        break;
                    }
                }

                // コメントを読み込む。
                for(int i = startIndex; i < line.Length; i++)
                {
                    data.Comment += line[i];
                }

                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 総ステップ数を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:ステップ数を読み込んだ</returns>
        private static bool LoadStepCount(UDC data, string line)
        {
            if(line.Length < 1)
            {
                return false;
            }
            
            // ステップ数を整数型に変換する。
            if(int.TryParse(line, out int stepCount))
            {
                data.StepCount = stepCount;
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// データの繰り返しタイプを読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:ステップ数を読み込んだ</returns>
        private static bool LoadDataRepetitionType(UDC data, string line)
        {
            if (line.Length < 4)
            {
                return false;
            }

            data.DataRepetitionType = line.ParseToUDCDataRepetitionType();

            return true;
        }

        /// <summary>
        /// ステップ番号を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:ステップ番号を読み込んだ</returns>
        private static bool LoadStepNumber(UDC data, string line)
        {
            string[] splitedText = line.Split(' ');

            if(splitedText[0] == "step1")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 全節点数と全要素数を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:全節点数と全要素数を読み込んだ</returns>
        private static bool LoadPointAndElementCount(UDC data, string line)
        {
            if (line.Length < 3)
            {
                return false;
            }

            string[] splitedText = line.Split(' ');

            if (splitedText.Length >= 2 && int.TryParse(splitedText[0], out int pointCount) && int.TryParse(splitedText[1], out int elementCount))
            {
                data.PointCount = pointCount;
                data.ElementCount = elementCount;
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 節点の座標を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:節点を読み込んだ</returns>
        private static bool LoadPoints(UDC data, string line)
        {
            if (data.Points == null)
                return false;

            string[] splitedText = line.Split(' ');

            if (splitedText.Length >= 4 && int.TryParse(splitedText[0], out int pointNumber))
            {
                if(float.TryParse(splitedText[1],out float x)
                    && float.TryParse(splitedText[2], out float y)
                    && float.TryParse(splitedText[3], out float z))
                {
                    data.Points[pointNumber - 1] = new System.Numerics.Vector3(x, y, z);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 要素の材料番号、種類、要素を構成する節点を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:要素を読み込んだ</returns>
        private static bool LoadElements(UDC data, string line)
        {
            if (data.Elements == null)
                return false;

            string[] splitedText = line.Split(' ');
            int elementNumber, material;
            UDCElementType type;

            // 要素番号、要素の材料番号、種類を読み込む。
            if (splitedText.Length >= 3)
            {
                if (!int.TryParse(splitedText[0], out elementNumber))
                    return false;

                if (!int.TryParse(splitedText[1], out material))
                    return false;

                if (splitedText[2].Length >= 2)
                    type = splitedText[2].ParseToUDCElementType();
                else
                    return false;
            }
            else
            {
                return false;
            }

            int pointCount = type.GetNumberOfValues();

            // 要素を構成する接点を読み込む。
            if (splitedText.Length >= 3 + pointCount)
            {
                int[] points = new int[pointCount];

                for (int index = 0; index < pointCount; index++)
                {
                    points[index] = int.Parse(splitedText[3 + index]);
                }

                data.Elements[elementNumber - 1] = new UDCElement(material, type, points);

                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
