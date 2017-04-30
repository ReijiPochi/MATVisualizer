using MATVisualizer.Graphics;
using MATVisualizer.Graphics.Core;
using System.Numerics;

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

        public Object3D ToObject3D()
        {
            VertexData_ShapeAndIndex[] vertices = new VertexData_ShapeAndIndex[CellCount * 12];
            Vector3[] buffer = new Vector3[CellCount];

            int vertexIndex = 0;
            uint dataIndex = 0;

            foreach (UDCCell cell in Cells)
            {
                uint vertex1 = (uint)cell.Nodes[0] - 1;
                uint vertex2 = (uint)cell.Nodes[1] - 1;
                uint vertex3 = (uint)cell.Nodes[2] - 1;
                uint vertex4 = (uint)cell.Nodes[3] - 1;

                vertices[vertexIndex].SV_Position = Nodes[vertex3].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex2].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex1].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;

                vertices[vertexIndex].SV_Position = Nodes[vertex4].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex3].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex1].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;

                vertices[vertexIndex].SV_Position = Nodes[vertex2].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex4].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex1].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;

                vertices[vertexIndex].SV_Position = Nodes[vertex2].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex3].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;
                vertices[vertexIndex].SV_Position = Nodes[vertex4].Coord;
                vertices[vertexIndex].GC_DataIndex1 = dataIndex;
                vertexIndex++;


                buffer[dataIndex] = new Vector3(cell.Data[0] * 50 + 0.5f, cell.Data[1] * 50 + 0.5f, cell.Data[2] * 50 + 0.5f);
                dataIndex++;

                //if (dataIndex > 0)
                //break;
            }

            GraphicsObjectDescription desc = new GraphicsObjectDescription()
            {
                vertexType = VertexType.ShapeAndValue,
                primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY.TRIANGLELIST,
            };

            Shader.GenerateVertexShaderAndInputLayout("Resources/Effects/Sample.fx", "VSFunc", ref desc, out desc.vs, out desc.inputLayout);
            //Shader.GenerateGeometryShader("Resources/Effects/Sample.fx", "GSFunc", out desc.gs);
            Shader.GeneratePixelShader("Resources/Effects/Sample.fx", "PSFunc", out desc.ps);

            Object3D obj1 = new Object3D(desc);

            obj1.Vertices = new VerticesData<VertexData_ShapeAndIndex>(vertices);
            obj1.DownloadVerticesToGPU();

            BufferDescription bufferDesc = new BufferDescription()
            {
                elementSize = 12,
                numElements = buffer.Length
            };

            BufferResource bufferResource = new BufferResource(ref bufferDesc, buffer);

            obj1.SetBuffer(bufferResource);

            obj1.Unlock();

            return obj1;
        }
    }
}
