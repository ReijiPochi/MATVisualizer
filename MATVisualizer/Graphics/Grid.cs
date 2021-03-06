﻿using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class Grid : SolidObject
    {
        public Grid(int num, float pitch) : base()
        {
            GraphicsObjectDescription desc = new GraphicsObjectDescription()
            {
                vertexType = VertexType.ShapeAndValue,
                primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY.LINELIST,
            };

            _Shader.GenerateVertexShaderAndInputLayout("Resources/Effects/Sample.fx", "VSFunc", ref desc, out desc.vs, out desc.inputLayout);
            //Shader.GenerateGeometryShader("Resources/Effects/Sample.fx", "GSFunc", out desc.gs);
            _Shader.GeneratePixelShader("Resources/Effects/Sample.fx", "PSFunc", out desc.ps);

            Create(desc);

            Shapes[0] = new Shape(GenVertices(num, pitch));

            BufferDescription bDesc = new BufferDescription()
            {
                elementSize = 12,
                numElements = 4
            };

            Vector3[] bData =
            {
                new Vector3(0.35f, 0.35f, 0.35f),
                new Vector3(0.8f, 0.0f, 0.2f),
                new Vector3(0.0f, 0.2f, 0.8f),
                new Vector3(0.2f, 0.8f, 0.2f)
            };

            Buffers[0] = new Buffer(ref bDesc, bData);

            Unlock();
        }

        private static VerticesData<VertexData_ShapeAndIndex> GenVertices(int num, float pitch)
        {
            VertexData_ShapeAndIndex[] data = new VertexData_ShapeAndIndex[12 + num * 8];

            float length = num * pitch;

            int index = 0;

            for (int x = -num; x <= num; x++)
            {
                if (x != 0)
                {
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(length, 0, pitch * x), GC_DataIndex1 = 0 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(-length, 0, pitch * x), GC_DataIndex1 = 0 };
                    index++;
                }
                else
                {
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(length, 0, 0), GC_DataIndex1 = 1 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, 0), GC_DataIndex1 = 1 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(-length, 0, 0), GC_DataIndex1 = 0 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, 0), GC_DataIndex1 = 0 };
                    index++;
                }
            }

            for(int z = -num; z <= num; z++)
            {
                if(z != 0)
                {
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(pitch * z, 0, length), GC_DataIndex1 = 0 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(pitch * z, 0, -length), GC_DataIndex1 = 0 };
                    index++;

                }
                else
                {
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, length), GC_DataIndex1 = 2 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, 0), GC_DataIndex1 = 2 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, -length), GC_DataIndex1 = 0 };
                    index++;
                    data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, 0), GC_DataIndex1 = 0 };
                    index++;
                }
            }

            data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, length, 0), GC_DataIndex1 = 3 };
            index++;
            data[index] = new VertexData_ShapeAndIndex() { SV_Position = new Vector3(0, 0, 0), GC_DataIndex1 = 3 };

            return new VerticesData<VertexData_ShapeAndIndex>(data);
        }
    }
}
