using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public static class VertexTypeExt
    {
        public static VertexType FromVertexDataT(this VertexType me, Type type)
        {
            switch(type.Name)
            {
                case "VertexData_ShapeAndIndex":
                    return VertexType.ShapeAndValue;

                default:
                    return VertexType.Null;
            }
        }
    }
}
