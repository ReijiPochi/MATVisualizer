﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public abstract class VerticesDataBase : IDisposable
    {
        public IntPtr Pointer { get; protected set; }
        public abstract void Dispose();
    }
}