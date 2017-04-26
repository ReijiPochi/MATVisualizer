using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public enum D3D11_PRIMITIVE_TOPOLOGY
    {
        UNDEFINED = 0,
        POINTLIST = 1,
        LINELIST = 2,
        LINESTRIP = 3,
        TRIANGLELIST = 4,
        TRIANGLESTRIP = 5,
        LINELIST_ADJ = 10,
        LINESTRIP_ADJ = 11,
        TRIANGLELIST_ADJ = 12,
        TRIANGLESTRIP_ADJ = 13,
    }
}
