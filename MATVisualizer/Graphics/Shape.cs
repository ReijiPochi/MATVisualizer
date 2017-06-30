using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class Shape : DisposableObject
    {
        public Shape(VertexType type)
        {
            shapeHandle = _Shape.Create(type);
        }

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
            if (disposed) throw new ObjectDisposedException(GetType().Name);

            _Shape.Set(shapeHandle, vertices.Pointer, vertices.NumVertices, IntPtr.Zero, 0);
            Vertices = vertices;
        }

        public void Set(IndicesData indices)
        {
            if (disposed) throw new ObjectDisposedException(GetType().Name);

            _Shape.Set(shapeHandle, IntPtr.Zero, 0, indices.Pointer, indices.NumIndices);
            Indices = indices;
        }

        public void Set(VerticesData vertices, IndicesData indices)
        {
            if (disposed) throw new ObjectDisposedException(GetType().Name);

            _Shape.Set(shapeHandle, vertices.Pointer, vertices.NumVertices, indices.Pointer, indices.NumIndices);
            Vertices = vertices;
            Indices = indices;
        }

        public void UpdateAll()
        {
            if (disposed) throw new ObjectDisposedException(GetType().Name);

            _Shape.Update(shapeHandle, Vertices.Pointer, Vertices.NumVertices, Indices.Pointer, Indices.NumIndices);
        }

        public void UpdateIndices()
        {
            if (disposed) throw new ObjectDisposedException(GetType().Name);

            _Shape.Update(shapeHandle, IntPtr.Zero, 0, Indices.Pointer, Indices.NumIndices);
        }

        public void UpdateVertices()
        {
            if (disposed) throw new ObjectDisposedException(GetType().Name);

            _Shape.Update(shapeHandle, Vertices.Pointer, Vertices.NumVertices, IntPtr.Zero, 0);
        }

        public override void Dispose()
        {
            if (disposed) return;

            _Shape.Release(shapeHandle);

            base.Dispose();
        }
    }
}
