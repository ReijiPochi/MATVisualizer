using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    /// <summary>
    /// Describes a buffer resource.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct BufferDescription
    {
        /// <summary>
        /// Size of the buffer in bytes.
        /// </summary>
        uint ByteWidth;

        /// <summary>
        /// Identify how the buffer is expected to be read from and written to.
        /// Frequency of update is a key factor.
        /// </summary>
        Usage Usage;

        /// <summary>
        /// Identify how the buffer will be bound to the pipeline. 
        /// Flags can be combined with a logical OR.
        /// </summary>
        BindFlag BindFlags;

        /// <summary>
        /// CPU access flags or 0 if no CPU access is necessary.
        /// Flags can be combined with a logical OR.
        /// </summary>
        uint CPUAccessFlags;
        uint MiscFlags;
        uint StructureByteStride;
    }
}
