using MATVisualizer.Data;
using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class UDCObject
    {
        public UDCObject(UDC udc)
        {
            UdcData = udc;

            if(description.vs == IntPtr.Zero)
            {
                description = new GraphicsObjectDescription()
                {
                    vertexType = VertexType.ShapeAndValue,
                    primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY.TRIANGLELIST,
                };

                _Shader.GenerateVertexShaderAndInputLayout("Resources/Effects/Sample.fx", "VSFunc", ref description, out description.vs, out description.inputLayout);
                //Shader.GenerateGeometryShader("Resources/Effects/Sample.fx", "GSFunc", out desc.gs);
                _Shader.GeneratePixelShader("Resources/Effects/Sample.fx", "PSFunc", out description.ps);
            }

            Surface = new SolidObject(description);
            Surface.Shapes[1] = new Shape(new VerticesData<VertexData_ShapeAndIndex>(new VertexData_ShapeAndIndex[1]), new IndicesData<uint>(new uint[1]));

            SlicePlane = new SolidObject(description);
            SlicePlane.Shapes[0] = new Shape(VertexType.ShapeAndValue);

            //GetSurface(udc);
            GetSurface2(udc);
            

            Surface.Unlock();
            Render.AddObject(Surface);

            Render.AddObject(SlicePlane);
        }

        private static GraphicsObjectDescription description;

        private struct Polygon
        {
            public int p1;
            public int p2;
            public int p3;
            public UDCCell cell;
        }

        /// <summary>
        /// この UDCObject の UDCデータ です。
        /// </summary>
        public UDC UdcData { get; set; }

        /// <summary>
        /// 表面形状の3Dオブジェクトです。
        /// <para>Shape[0] : 元の3D形状</para>
        /// <para>Shape[1] : 断面抽出するときに、隙間を埋めるために表面に追加で生成される3D形状</para>
        /// </summary>
        public SolidObject Surface { get; set; }

        public SolidObject SlicePlane { get; set; }
        private uint[] originalIndices;

        /// <summary>
        /// このUDCObjectのUDCデータを指定された面で切断・断面抽出します。
        /// </summary>
        /// <param name="position">面の中心座標</param>
        /// <param name="normal">面の法線</param>
        public void Slice(Vector3 position, Vector3 normal)
        {
            IndicesData<uint> indices = (IndicesData<uint>)Surface.Shapes[0].Indices;
            int indicesIndex = 0;
            VerticesData<VertexData_ShapeAndIndex> surfaceVertices = (VerticesData<VertexData_ShapeAndIndex>)Surface.Shapes[0].Vertices;

            List<VertexData_ShapeAndIndex> sliceEdge = new List<VertexData_ShapeAndIndex>();
            List<uint> sliceIndex = new List<uint>();
            uint sliceIndexCount = 0;

            for (int i = 0; i < originalIndices.Length; i += 3)
            {
                float v1dot = Vector3.Dot(Vector3.Subtract(position, surfaceVertices.data[originalIndices[i]].SV_Position), normal);
                float v2dot = Vector3.Dot(Vector3.Subtract(position, surfaceVertices.data[originalIndices[i + 1]].SV_Position), normal);
                float v3dot = Vector3.Dot(Vector3.Subtract(position, surfaceVertices.data[originalIndices[i + 2]].SV_Position), normal);

                bool v1 = v1dot > 0;
                bool v2 = v2dot > 0;
                bool v3 = v3dot > 0;

                if (v1 && v2 && v3)
                {
                    indices.data[indicesIndex] = originalIndices[i];
                    indicesIndex++;
                    indices.data[indicesIndex] = originalIndices[i + 1];
                    indicesIndex++;
                    indices.data[indicesIndex] = originalIndices[i + 2];
                    indicesIndex++;
                }
                else if (v1 || v2 || v3)
                {
                    Vector3 newV12 = new Vector3(), newV13 = new Vector3(), newV23 = new Vector3();
                    bool newV12gen = false, newV13gen = false, newV23gen = false;

                    v1dot = v1 ? v1dot : -v1dot;
                    v2dot = v2 ? v2dot : -v2dot;
                    v3dot = v3 ? v3dot : -v3dot;

                    if ((v1 && !v2) || (!v1 && v2))
                    {
                        newV12gen = true;
                        newV12 = (surfaceVertices.data[originalIndices[i]].SV_Position * v2dot + surfaceVertices.data[originalIndices[i + 1]].SV_Position * v1dot) / (v1dot + v2dot);
                    }

                    if((v1 && !v3) || (!v1 && v3))
                    {
                        newV13gen = true;
                        newV13 = (surfaceVertices.data[originalIndices[i]].SV_Position * v3dot + surfaceVertices.data[originalIndices[i + 2]].SV_Position * v1dot) / (v1dot + v3dot);
                    }

                    if ((v2 && !v3) || (!v2 && v3))
                    {
                        newV23gen = true;
                        newV23 = (surfaceVertices.data[originalIndices[i + 1]].SV_Position * v3dot + surfaceVertices.data[originalIndices[i + 2]].SV_Position * v2dot) / (v2dot + v3dot);
                    }


                    int count = 0;

                    if (v1)
                    {
                        sliceEdge.Add(surfaceVertices.data[originalIndices[i]]);
                        count++;
                    }
                    if (newV12gen)
                    {
                        sliceEdge.Add(new VertexData_ShapeAndIndex() { SV_Position = newV12, GC_DataIndex1 = surfaceVertices.data[originalIndices[i]].GC_DataIndex1 });
                        count++;
                    }
                    if (v2)
                    {
                        sliceEdge.Add(surfaceVertices.data[originalIndices[i + 1]]);
                        count++;
                    }
                    if (newV23gen)
                    {
                        sliceEdge.Add(new VertexData_ShapeAndIndex() { SV_Position = newV23, GC_DataIndex1 = surfaceVertices.data[originalIndices[i]].GC_DataIndex1 });
                        count++;
                    }
                    if (v3)
                    {
                        sliceEdge.Add(surfaceVertices.data[originalIndices[i + 2]]);
                        count++;
                    }
                    if (newV13gen)
                    {
                        sliceEdge.Add(new VertexData_ShapeAndIndex() { SV_Position = newV13, GC_DataIndex1 = surfaceVertices.data[originalIndices[i]].GC_DataIndex1 });
                        count++;
                    }

                    if(count==4)
                    {
                        sliceIndex.Add(sliceIndexCount);
                        sliceIndex.Add(sliceIndexCount + 1);
                        sliceIndex.Add(sliceIndexCount + 2);
                        sliceIndex.Add(sliceIndexCount);
                        sliceIndex.Add(sliceIndexCount + 2);
                        sliceIndex.Add(sliceIndexCount + 3);

                        sliceIndexCount += 4;
                    }
                    else if(count == 3)
                    {
                        sliceIndex.Add(sliceIndexCount);
                        sliceIndex.Add(sliceIndexCount + 1);
                        sliceIndex.Add(sliceIndexCount + 2);

                        sliceIndexCount += 3;
                    }

                }
            }

            UDCNode[] nodes = UdcData.Nodes;
            List<VertexData_ShapeAndIndex> slicePlane = new List<VertexData_ShapeAndIndex>();
            List<uint> slicePlaneIndex = new List<uint>();
            uint slicePlaneIndexCount = 0;
            List<Vector3> buffer2 = new List<Vector3>();
            uint buffer2Count = 0;
            Vector3[] tempVertices = new Vector3[4];
            int tempVerticesIndex = 0;

            foreach (UDCCell cell in UdcData.Cells)
            {
                float v1dot = Vector3.Dot(Vector3.Subtract(position, nodes[cell.Nodes[0]].Coord), normal);
                float v2dot = Vector3.Dot(Vector3.Subtract(position, nodes[cell.Nodes[1]].Coord), normal);
                float v3dot = Vector3.Dot(Vector3.Subtract(position, nodes[cell.Nodes[2]].Coord), normal);
                float v4dot = Vector3.Dot(Vector3.Subtract(position, nodes[cell.Nodes[3]].Coord), normal);

                bool v1 = v1dot > 0;
                bool v2 = v2dot > 0;
                bool v3 = v3dot > 0;
                bool v4 = v4dot > 0;

                if(!(v1 && v2 && v3 && v4) && (v1 || v2 || v3 || v4))
                {
                    v1dot = v1 ? v1dot : -v1dot;
                    v2dot = v2 ? v2dot : -v2dot;
                    v3dot = v3 ? v3dot : -v3dot;
                    v4dot = v4 ? v4dot : -v4dot;

                    int vertexCount = 0;
                    tempVerticesIndex = 0;

                    if ((v1 && !v2) || (!v1 && v2))
                    {
                        Vector3 v = (nodes[cell.Nodes[0]].Coord * v2dot + nodes[cell.Nodes[1]].Coord * v1dot) / (v1dot + v2dot);
                        slicePlane.Add(new VertexData_ShapeAndIndex() { SV_Position = v, GC_DataIndex1 = buffer2Count });
                        vertexCount++;

                        tempVertices[tempVerticesIndex] = v;
                        tempVerticesIndex++;
                    }
                    if((v1 && !v3) || (!v1 && v3))
                    {
                        Vector3 v = (nodes[cell.Nodes[0]].Coord * v3dot + nodes[cell.Nodes[2]].Coord * v1dot) / (v1dot + v3dot);
                        slicePlane.Add(new VertexData_ShapeAndIndex() { SV_Position = v, GC_DataIndex1 = buffer2Count });
                        vertexCount++;

                        tempVertices[tempVerticesIndex] = v;
                        tempVerticesIndex++;
                    }
                    if((v1 && !v4) || (!v1 && v4))
                    {
                        Vector3 v = (nodes[cell.Nodes[0]].Coord * v4dot + nodes[cell.Nodes[3]].Coord * v1dot) / (v1dot + v4dot);
                        slicePlane.Add(new VertexData_ShapeAndIndex() { SV_Position = v, GC_DataIndex1 = buffer2Count });
                        vertexCount++;

                        tempVertices[tempVerticesIndex] = v;
                        tempVerticesIndex++;
                    }
                    if((v2 && !v3) || (!v2 && v3))
                    {
                        Vector3 v = (nodes[cell.Nodes[1]].Coord * v3dot + nodes[cell.Nodes[2]].Coord * v2dot) / (v2dot + v3dot);
                        slicePlane.Add(new VertexData_ShapeAndIndex() { SV_Position = v, GC_DataIndex1 = buffer2Count });
                        vertexCount++;

                        tempVertices[tempVerticesIndex] = v;
                        tempVerticesIndex++;
                    }
                    if((v2 && !v4) || (!v2 && v4))
                    {
                        Vector3 v = (nodes[cell.Nodes[1]].Coord * v4dot + nodes[cell.Nodes[3]].Coord * v2dot) / (v2dot + v4dot);
                        slicePlane.Add(new VertexData_ShapeAndIndex() { SV_Position = v, GC_DataIndex1 = buffer2Count });
                        vertexCount++;

                        tempVertices[tempVerticesIndex] = v;
                        tempVerticesIndex++;
                    }
                    if ((v3 && !v4) || (!v3 && v4))
                    {
                        Vector3 v = (nodes[cell.Nodes[2]].Coord * v4dot + nodes[cell.Nodes[3]].Coord * v3dot) / (v3dot + v4dot);
                        slicePlane.Add(new VertexData_ShapeAndIndex() { SV_Position = v, GC_DataIndex1 = buffer2Count });
                        vertexCount++;

                        tempVertices[tempVerticesIndex] = v;
                        tempVerticesIndex++;
                    }

                    if (vertexCount == 4)
                    {
                        if (Vector3.Dot(Vector3.Cross(tempVertices[1] - tempVertices[0], tempVertices[2] - tempVertices[0]), normal) < 0)
                        {
                            slicePlaneIndex.Add(slicePlaneIndexCount);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 3);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                        }
                        else
                        {
                            slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                            slicePlaneIndex.Add(slicePlaneIndexCount);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 3);
                        }

                        //if (Vector3.Dot(Vector3.Cross(tempVertices[2] - tempVertices[1], tempVertices[3] - tempVertices[1]), normal) < 0)
                        //{
                        //    slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                        //    slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                        //    slicePlaneIndex.Add(slicePlaneIndexCount + 3);
                        //}
                        //else
                        //{
                        //    slicePlaneIndex.Add(slicePlaneIndexCount + 3);
                        //    slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                        //    slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                        //}

                        slicePlaneIndexCount += 4;
                    }
                    else if (vertexCount == 3)
                    {
                        if (Vector3.Dot(Vector3.Cross(tempVertices[1] - tempVertices[0], tempVertices[2] - tempVertices[0]), normal) < 0)
                        {
                            slicePlaneIndex.Add(slicePlaneIndexCount);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                        }
                        else
                        {
                            slicePlaneIndex.Add(slicePlaneIndexCount + 2);
                            slicePlaneIndex.Add(slicePlaneIndexCount + 1);
                            slicePlaneIndex.Add(slicePlaneIndexCount);
                        }

                        slicePlaneIndexCount += 3;
                    }

                    buffer2.Add(ColorConverter.HSVtoRGB(0.7f - cell.Data[3] * 20, 1f, 1f));
                    buffer2Count++;
                }
            }

            Surface.Shapes[0].Indices.NumIndices = (uint)indicesIndex;
            Surface.Shapes[0].UpdateIndices();

            //if (Surface.Shapes[1].Vertices != null) Surface.Shapes[1].Vertices.Dispose();
            //if (Surface.Shapes[1].Indices != null) Surface.Shapes[1].Indices.Dispose();

            //Surface.Shapes[1].Vertices = new VerticesData<VertexData_ShapeAndIndex>(sliceEdge.ToArray());
            //Surface.Shapes[1].Indices = new IndicesData<uint>(sliceIndex.ToArray());

            //Surface.Shapes[1].Set(Surface.Shapes[1].Vertices, Surface.Shapes[1].Indices);

            if (Surface.Shapes[1].Vertices.NumVertices < sliceEdge.Count)
            {
                Surface.Shapes[1].Vertices.Dispose();
                Surface.Shapes[1].Set(new VerticesData<VertexData_ShapeAndIndex>(sliceEdge.ToArray()));
            }
            else
            {
                sliceEdge.CopyTo(((VerticesData<VertexData_ShapeAndIndex>)Surface.Shapes[1].Vertices).data);
                Surface.Shapes[1].Vertices.NumVertices = (uint)sliceEdge.Count;
            }

            if (Surface.Shapes[1].Indices.NumIndices < sliceIndex.Count)
            {
                Surface.Shapes[1].Indices.Dispose();
                Surface.Shapes[1].Set(new IndicesData<uint>(sliceIndex.ToArray()));
            }
            else
            {
                sliceIndex.CopyTo(((IndicesData<uint>)Surface.Shapes[1].Indices).data);
                Surface.Shapes[1].Indices.NumIndices = (uint)sliceIndex.Count;
            }

            Surface.Shapes[1].UpdateAll();

            if (SlicePlane.Shapes[0].Vertices != null) SlicePlane.Shapes[0].Vertices.Dispose();
            if (SlicePlane.Shapes[0].Indices != null) SlicePlane.Shapes[0].Indices.Dispose();

            SlicePlane.Shapes[0].Set(new VerticesData<VertexData_ShapeAndIndex>(slicePlane.ToArray()), new IndicesData<uint>(slicePlaneIndex.ToArray()));

            BufferDescription bufferDesc = new BufferDescription()
            {
                elementSize = 12,
                numElements = buffer2.Count
            };

            if (SlicePlane.Buffers[0] != null) SlicePlane.Buffers[0].Dispose();

            SlicePlane.Buffers[0] = new Buffer(ref bufferDesc, buffer2.ToArray());
        }

        //private void GetSurface(UDC udc)
        //{
        //    List<VertexData_ShapeAndIndex> vertex = new List<VertexData_ShapeAndIndex>();
        //    List<uint> index = new List<uint>();
        //    List<Vector3> buffer = new List<Vector3>();


        //    Polygon t1 = new Polygon();
        //    Polygon t2 = new Polygon();
        //    Polygon t3 = new Polygon();
        //    Polygon t4 = new Polygon();

        //    uint bufferIndex = 0, indexIndex = 0;

        //    foreach (UDCCell cell in udc.Cells)
        //    {
        //        int vertex1 = cell.Nodes[0];
        //        int vertex2 = cell.Nodes[1];
        //        int vertex3 = cell.Nodes[2];
        //        int vertex4 = cell.Nodes[3];

        //        t1.p3 = vertex3;
        //        t1.p2 = vertex2;
        //        t1.p1 = vertex1;

        //        t2.p3 = vertex4;
        //        t2.p2 = vertex3;
        //        t2.p1 = vertex1;

        //        t3.p3 = vertex2;
        //        t3.p2 = vertex4;
        //        t3.p1 = vertex1;

        //        t4.p3 = vertex2;
        //        t4.p2 = vertex3;
        //        t4.p1 = vertex4;

        //        bool t1Include = false, t2Include = false, t3Include = false, t4Include = false;

        //        foreach (UDCCell anothorCell in udc.Cells)
        //        {
        //            if (cell == anothorCell)
        //                continue;

        //            if (!t1Include) t1Include = Included(t1, anothorCell);
        //            if (!t2Include) t2Include = Included(t2, anothorCell);
        //            if (!t3Include) t3Include = Included(t3, anothorCell);
        //            if (!t4Include) t4Include = Included(t4, anothorCell);
        //        }

        //        if (!t1Include)
        //        {
        //            buffer.Add(ColorConverter.HSVtoRGB(0.7f - cell.Data[3] * 20, 1f, 1f));

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t1.p1 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t1.p2 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t1.p3 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            bufferIndex++;
        //        }

        //        if (!t2Include)
        //        {
        //            buffer.Add(ColorConverter.HSVtoRGB(0.7f - cell.Data[3] * 20, 1f, 1f));

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t2.p1 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t2.p2 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t2.p3 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            bufferIndex++;
        //        }

        //        if (!t3Include)
        //        {
        //            buffer.Add(ColorConverter.HSVtoRGB(0.7f - cell.Data[3] * 20, 1f, 1f));

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t3.p1 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t3.p2 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t3.p3 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            bufferIndex++;
        //        }

        //        if (!t4Include)
        //        {
        //            buffer.Add(ColorConverter.HSVtoRGB(0.7f - cell.Data[3] * 20, 1f, 1f));

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t4.p1 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t4.p2 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = udc.Nodes[t4.p3 - 1].Coord, GC_DataIndex1 = bufferIndex });
        //            index.Add(indexIndex);
        //            indexIndex++;

        //            bufferIndex++;
        //        }
        //    }

        //    Surface.Vertices = new VerticesData<VertexData_ShapeAndIndex>(vertex.ToArray());
        //    Surface.Indices = new IndicesData<uint>(index.ToArray());

        //    BufferDescription bufferDesc = new BufferDescription()
        //    {
        //        elementSize = 12,
        //        numElements = buffer.Count
        //    };

        //    Surface.Buffer = new BufferResource(ref bufferDesc, buffer.ToArray());
        //}

        /// <summary>
        /// UDCデータの表面を抽出し、このUDCObjectに設定します。
        /// </summary>
        /// <param name="udc"></param>
        private void GetSurface2(UDC udc)
        {
            Polygon[] polys = new Polygon[udc.CellCount * 4 + 1];
            int polyIndex = 0;

            foreach (UDCCell cell in udc.Cells)
            {
                int[] vertices = new int[4];
                Array.Copy(cell.Nodes, vertices, 4);
                Array.Sort(vertices);

                polys[polyIndex].p1 = vertices[0];
                polys[polyIndex].p2 = vertices[1];
                polys[polyIndex].p3 = vertices[2];
                polys[polyIndex].cell = cell;
                polyIndex++;

                polys[polyIndex].p1 = vertices[0];
                polys[polyIndex].p2 = vertices[1];
                polys[polyIndex].p3 = vertices[3];
                polys[polyIndex].cell = cell;
                polyIndex++;

                polys[polyIndex].p1 = vertices[0];
                polys[polyIndex].p2 = vertices[2];
                polys[polyIndex].p3 = vertices[3];
                polys[polyIndex].cell = cell;
                polyIndex++;

                polys[polyIndex].p1 = vertices[1];
                polys[polyIndex].p2 = vertices[2];
                polys[polyIndex].p3 = vertices[3];
                polys[polyIndex].cell = cell;
                polyIndex++;
            }

            polys[udc.CellCount * 4].p1 = int.MaxValue;

            Array.Sort(polys, (x, y) =>
            {
                if (x.p1 > y.p1) return 1;
                else if (x.p1 < y.p1) return -1;
                else
                {
                    if (x.p2 > y.p2) return 1;
                    else if (x.p2 < y.p2) return -1;
                    else
                    {
                        if (x.p3 > y.p3) return 1;
                        else if (x.p3 < y.p3) return -1;
                        else return 0;
                    }
                }
            });

            List<VertexData_ShapeAndIndex> vertex = new List<VertexData_ShapeAndIndex>();
            List<Vector3> buffer = new List<Vector3>();

            UDCNode[] nodeArray = udc.Nodes;
            uint bufferIndex = 0;

            for (int index = 0; index < polys.Length - 1; index++)
            {
                Polygon t1 = polys[index];
                Polygon t2 = polys[index + 1];

                if (t1.p2 != t2.p2 || t1.p1 != t2.p1 || t1.p3 != t2.p3)
                {
                    int[] nodes = t1.cell.Nodes;

                    bool node1 = Included(nodes[0], t1);
                    bool node2 = Included(nodes[1], t1);
                    bool node3 = Included(nodes[2], t1);
                    bool node4 = Included(nodes[3], t1);

                    if(node1)
                    {
                        if(node2)
                        {
                            if(node3)
                            {
                                vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[0]].Coord, GC_DataIndex1 = bufferIndex });
                                vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[1]].Coord, GC_DataIndex1 = bufferIndex });
                                vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[2]].Coord, GC_DataIndex1 = bufferIndex });
                            }
                            else
                            {
                                vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[0]].Coord, GC_DataIndex1 = bufferIndex });
                                vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[3]].Coord, GC_DataIndex1 = bufferIndex });
                                vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[1]].Coord, GC_DataIndex1 = bufferIndex });
                            }
                        }
                        else
                        {
                            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[0]].Coord, GC_DataIndex1 = bufferIndex });
                            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[2]].Coord, GC_DataIndex1 = bufferIndex });
                            vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[3]].Coord, GC_DataIndex1 = bufferIndex });
                        }
                    }
                    else
                    {
                        vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[3]].Coord, GC_DataIndex1 = bufferIndex });
                        vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[2]].Coord, GC_DataIndex1 = bufferIndex });
                        vertex.Add(new VertexData_ShapeAndIndex() { SV_Position = nodeArray[nodes[1]].Coord, GC_DataIndex1 = bufferIndex });
                    }

                    buffer.Add(ColorConverter.HSVtoRGB(0.7f - t1.cell.Data[3] * 20, 1f, 1f));
                    bufferIndex++;
                }
                else
                {
                    index++;
                }
            }

            

            uint[] indices = new uint[vertex.Count];
            for (uint i = 0; i < indices.Length; i++)
            {
                indices[i] = i;
            }

            originalIndices = new uint[indices.Length];
            Array.Copy(indices, originalIndices, indices.Length);

            Surface.Shapes[0] = new Shape(new VerticesData<VertexData_ShapeAndIndex>(vertex.ToArray()), new IndicesData<uint>(indices));

            BufferDescription bufferDesc = new BufferDescription()
            {
                elementSize = 12,
                numElements = buffer.Count
            };

            Surface.Buffers[0] = new Buffer(ref bufferDesc, buffer.ToArray());
        }

        /// <summary>
        /// インデックス番号pの点がポリゴンtに含まれているか判定します。
        /// </summary>
        /// <param name="p">判定する点</param>
        /// <param name="t">判定するポリゴン</param>
        /// <returns>true:pはtに含まれる</returns>
        private bool Included(int p, Polygon t)
        {
            if (t.p1 == p || t.p2 == p || t.p3 == p)
                return true;

            return false;
        }

        /// <summary>
        /// ポリゴンtがUDCセルcellに含まれているか判定します。
        /// </summary>
        /// <param name="t">判定するポリゴン</param>
        /// <param name="cell">判定するUDCセル</param>
        /// <returns></returns>
        private bool Included(Polygon t, UDCCell cell)
        {
            bool p1Inside = false, p2Inside = false, p3Inside = false;

            foreach(int index in cell.Nodes)
            {
                if (t.p1 == index) p1Inside = true;
                if (t.p2 == index) p2Inside = true;
                if (t.p3 == index) p3Inside = true;

                if (p1Inside && p2Inside && p3Inside)
                    return true;
            }

            return false;
        }

    }
}
