using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer
{
    /// <summary>
    /// 
    /// </summary>
    public abstract class DisposableObject : IDisposable
    {
        protected bool disposed = false;

        public virtual void Dispose()
        {
            disposed = true;
            GC.SuppressFinalize(this);
        }

        ~DisposableObject()
        {
            Dispose();
        }
    }
}
