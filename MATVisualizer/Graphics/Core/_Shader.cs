using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    public class _Shader
    {
        // DllImport属性のSetLastErrorをtrueにしないとうまくいかない
        [DllImport("GraphicsCore.dll", EntryPoint = "Shader_GenerateVertexShaderAndInputLayout", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern HRESULT GenerateVertexShaderAndInputLayout(string fileName, string entryPoint, ref GraphicsObjectDescription desc, out IntPtr vs, out IntPtr inputLayout);

        [DllImport("GraphicsCore.dll", EntryPoint = "Shader_GenerateGeometryShader", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern HRESULT GenerateGeometryShader(string fileName, string entryPoint, out IntPtr ppGS);

        [DllImport("GraphicsCore.dll", EntryPoint = "Shader_GeneratePixelShader", CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        public static extern HRESULT GeneratePixelShader(string fileName, string entryPoint, out IntPtr ppPS);
    }
}
