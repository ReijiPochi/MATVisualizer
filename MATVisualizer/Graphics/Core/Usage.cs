using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    /// <summary>
    /// Identifies expected resource use during rendering. 
    /// The usage directly reflects whether a resource is accessible by the CPU and/or the graphics processing unit (GPU).
    /// </summary>
    public enum Usage
    {
        /// <summary>
        /// A resource that requires read and write access by the GPU.
        /// This is likely to be the most common usage choice.
        /// </summary>
        Default = 0,

        /// <summary>
        /// A resource that can only be read by the GPU.
        /// It cannot be written by the GPU, and cannot be accessed at all by the CPU.
        /// This type of resource must be initialized when it is created, since it cannot be changed after creation.
        /// </summary>
        Immutable = 1,

        /// <summary>
        /// A resource that is accessible by both the GPU (read only) and the CPU (write only).
        /// A dynamic resource is a good choice for a resource that will be updated by the CPU at least once per frame.
        /// To update a dynamic resource, use a Map method.
        /// </summary>
        Dynamic = 2,

        /// <summary>
        /// A resource that supports data transfer (copy) from the GPU to the CPU.
        /// </summary>
        Staging = 3
    }
}
