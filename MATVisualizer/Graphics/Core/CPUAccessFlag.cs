using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    /// <summary>
    /// CPUAccessFlag enumeration Specifies the types of CPU access allowed for a resource.
    /// </summary>
    public enum CPUAccessFlag
    {
        /// <summary>
        /// The resource is to be mappable so that the CPU can change its contents.
        /// Resources created with this flag cannot be set as outputs of the pipeline and must be created with either dynamic or staging usage.
        /// </summary>
        Write = 0x10000,

        /// <summary>
        /// The resource is to be mappable so that the CPU can read its contents.
        /// Resources created with this flag cannot be set as either inputs or outputs to the pipeline and must be created with staging usage.
        /// </summary>
        Read = 0x20000
    }
}
