using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class Shape
    {
        public Shape(VerticesData vertices)
        {
            shapeHandle = _Shape.Create(vertices.Type);
            Set(vertices);
            Vertices = vertices;
        }

        public Shape(VerticesData vertices, IndicesData indices)
        {
            shapeHandle = _Shape.Create(vertices.Type);
            Set(vertices, indices);
            Vertices = vertices;
            Indices = indices;
        }

        public IntPtr shapeHandle;

        public VerticesData Vertices { get; set; }

        public IndicesData Indices { get; set; }

        public void Set(VerticesData vertices)
        {
            _Shape.Set(shapeHandle, vertices.Type, vertices.Pointer, vertices.NumVertices, IntPtr.Zero, 0);
        }

        public void Set(VerticesData vertices, IndicesData indices)
        {
            _Shape.Set(shapeHandle, vertices.Type, vertices.Pointer, vertices.NumVertices, indices.Pointer, indices.NumIndices);
        }

        public void UpdateAll()
        {
            _Shape.Update(shapeHandle, Vertices.Pointer, Vertices.NumVertices, Indices.Pointer, Indices.NumIndices);
        }

        public void UpdateIndices()
        {
            _Shape.Update(shapeHandle, IntPtr.Zero, 0, Indices.Pointer, Indices.NumIndices);
        }

        public void UpdateVertices()
        {
            _Shape.Update(shapeHandle, Vertices.Pointer, Vertices.NumVertices, Indices.Pointer, Indices.NumIndices);
        }
    }
}
