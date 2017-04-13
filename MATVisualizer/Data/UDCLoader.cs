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

                        case LoadingState.NodeAndCellCount:
                            result = LoadNodeAndCellCount(data, line);
                            if (result) data.InitializeArrayOfPointAndElement();
                            state++;
                            break;

                        case LoadingState.Nodes:
                            result = LoadNodes(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.Cells:
                            result = LoadCells(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.NodeAndCellDataCount:
                            result = LoadNodeAndCellComponentCount(data, line);
                            state++;
                            break;

                        case LoadingState.NodeDataCountAndComponentCount:
                            // 無視して読み飛ばす。
                            state++;
                            break;

                        case LoadingState.NodeComponentLabelAndUnit:
                            result = LoadNodeComponentLabelAndUnit(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.NodeData:
                            result = LoadNodeData(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.CellDataCountAndComponentCount:
                            // 無視して読み飛ばす。
                            state++;
                            break;

                        case LoadingState.CellComponentLabelAndUnit:
                            result = LoadCellComponentLabelAndUnit(data, line);
                            if (!result) state++;
                            break;

                        case LoadingState.CellData:
                            result = LoadCellData(data, line);
                            if (!result)
                            {
                                return data;
                            }
                            break;

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
            NodeAndCellCount,

            /// <summary>
            /// 節点番号、X/Y/Z座標値を 1⾏1節点で記述します。
            /// </summary>
            Nodes,

            /// <summary>
            /// 要素に関する定義を1⾏1要素で記述します。
            /// 材料番号は、要素の集合をでグループ分けするのに⽤います。
            /// </summary>
            Cells,

            /// <summary>
            /// 節点データ、要素データの各成分数を記述します。
            /// </summary>
            NodeAndCellDataCount,

            /// <summary>
            /// 節点データのデータ成分数と、それぞれの成分が「いくつの成分から構成されるか」を記述します。
            /// </summary>
            NodeDataCountAndComponentCount,

            /// <summary>
            /// 節点データ各成分のラベルと単位を指定します。
            /// この指定は省略することもできますが、指定しない場合でも区切りの ","（カンマ）が必ず必要になります。
            /// </summary>
            NodeComponentLabelAndUnit,

            /// <summary>
            /// 1⾏1節点で、節点番号と対応する節点データを並べて記述します。
            /// </summary>
            NodeData,

            /// <summary>
            /// 要素データのデータ成分数と、それぞれの成分が「いくつの成分から構成されるか」を記述します。
            /// </summary>
            CellDataCountAndComponentCount,

            /// <summary>
            /// 要素データ各成分のラベルと単位を指定します。
            /// この指定は省略することもできますが、指定しない場合でも区切りの ","（カンマ）が必ず必要になります。
            /// </summary>
            CellComponentLabelAndUnit,

            /// <summary>
            /// 1⾏1要素で、要素番号と対応する要素データを並べて記述します。
            /// </summary>
            CellData
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

                // #の後の文字が半角スペースの場合、読み込まないようにする。
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
            string[] splitText = line.Split(' ');

            if(splitText[0] == "step1")
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
        private static bool LoadNodeAndCellCount(UDC data, string line)
        {
            if (line.Length < 3)
            {
                return false;
            }

            string[] splitText = line.Split(' ');

            if (splitText.Length >= 2 && int.TryParse(splitText[0], out int pointCount) && int.TryParse(splitText[1], out int elementCount))
            {
                data.NodeCount = pointCount;
                data.CellCount = elementCount;
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
        private static bool LoadNodes(UDC data, string line)
        {
            if (data.Nodes == null)
                return false;

            string[] splitText = line.Split(' ');

            if (splitText.Length >= 4 && int.TryParse(splitText[0], out int pointNumber))
            {
                if(float.TryParse(splitText[1],out float x)
                    && float.TryParse(splitText[2], out float y)
                    && float.TryParse(splitText[3], out float z))
                {
                    data.Nodes[pointNumber - 1] = new UDCNode()
                    {
                        Coord = new System.Numerics.Vector3(x, y, z)
                    };

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
        private static bool LoadCells(UDC data, string line)
        {
            if (data.Cells == null)
                return false;

            string[] splitText = line.Split(' ');
            int elementNumber, material;
            UDCCellType type;

            // 要素番号、要素の材料番号、種類を読み込む。
            if (splitText.Length >= 3)
            {
                if (!int.TryParse(splitText[0], out elementNumber))
                    return false;

                if (!int.TryParse(splitText[1], out material))
                    return false;

                if (splitText[2].Length >= 2)
                    type = splitText[2].ParseToUDCCellType();
                else
                    return false;
            }
            else
            {
                return false;
            }

            int pointCount = type.GetNumberOfValues();

            // 要素を構成する接点を読み込む。
            if (splitText.Length >= 3 + pointCount)
            {
                int[] points = new int[pointCount];

                for (int index = 0; index < pointCount; index++)
                {
                    points[index] = int.Parse(splitText[3 + index]);
                }

                data.Cells[elementNumber - 1] = new UDCCell(material, type, points);

                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 節点のデータ成分数と要素のデータ成分数を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:データ成分数を読み込んだ</returns>
        private static bool LoadNodeAndCellComponentCount(UDC data, string line)
        {
            string[] splitText = line.Split(' ');

            if (int.TryParse(splitText[0], out int nodeDataCount))
            {
                data.NodeComponentCount = nodeDataCount;
            }
            else
            {
                return false;
            }

            if (int.TryParse(splitText[1], out int cellDataCount))
            {
                data.CellComponentCount = cellDataCount;
            }
            else
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// 節点のデータ各成分のラベルと単位を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:節点のデータ各成分のラベルと単位を読み込んだ</returns>
        private static bool LoadNodeComponentLabelAndUnit(UDC data, string line)
        {
            //line.Replace(" ", String.Empty);
            //string[] splitText = line.Split(',');

            if (line.Contains(','))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 節点のデータを読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:節点のデータを読み込んだ</returns>
        private static bool LoadNodeData(UDC data, string line)
        {
            if (data.NodeComponentCount == 0)
                return false;

            string[] splitText = line.Split(' ');

            if(splitText.Length >= 1 + data.NodeComponentCount)
            {
                if (!int.TryParse(splitText[0], out int number))
                    return false;

                float[] dataComponents = new float[data.NodeComponentCount];

                for(int index = 0; index < data.NodeComponentCount; index++)
                {
                    if (float.TryParse(splitText[1 + index], out float component))
                        dataComponents[index] = component;
                    else
                        return false;
                }

                data.Nodes[number - 1].Data = dataComponents;

                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 要素のデータ各成分のラベルと単位を読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:要素のデータ各成分のラベルと単位を読み込んだ</returns>
        private static bool LoadCellComponentLabelAndUnit(UDC data, string line)
        {
            //line.Replace(" ", String.Empty);
            //string[] splitText = line.Split(',');

            if (line.Contains(','))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 要素のデータを読み込みます。存在しない場合や失敗した場合、falseを返します。
        /// </summary>
        /// <param name="data">読み込み先のUDCデータ</param>
        /// <param name="line">読み込むテキスト</param>
        /// <returns>true:要素のデータを読み込んだ</returns>
        private static bool LoadCellData(UDC data, string line)
        {
            if (data.CellComponentCount == 0)
                return false;

            string[] splitText = line.Split(' ');

            if (splitText.Length >= 1 + data.CellComponentCount)
            {
                if (!int.TryParse(splitText[0], out int number))
                    return false;

                float[] dataComponents = new float[data.CellComponentCount];

                for (int index = 0; index < data.CellComponentCount; index++)
                {
                    if (float.TryParse(splitText[1 + index], out float component))
                        dataComponents[index] = component;
                    else
                        return false;
                }

                data.Cells[number - 1].Data = dataComponents;

                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
