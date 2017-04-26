﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{

    public class GraphicsObject
    {
        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsCore_Initialize", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern IntPtr Create(ref GraphicsObjectDescription desc);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetVertices", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern HRESULT SetVertices(IntPtr obj, object[] data, int length);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetTexture", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void SetTexture(IntPtr obj, int slot, IntPtr texture);

        [DllImport("GraphicsCore.dll", EntryPoint = "GraphicsObject_SetBuffer", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern void GraphicsObject_SetBuffer(IntPtr obj, int slot, IntPtr buffer);
    }
}
