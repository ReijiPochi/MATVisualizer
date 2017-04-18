using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics.Core
{
    /// <summary>
    /// Identifies how to bind a resource to the pipeline.
    /// </summary>
    public enum BindFlag : uint
    {
        /// <summary>
        /// Bind a buffer as a vertex buffer to the input-assembler stage.
        /// </summary>
        VertexBuffer = 0x1,

        /// <summary>
        /// Bind a buffer as an index buffer to the input-assembler stage.
        /// </summary>
        IndexBuffer = 0x2,

        /// <summary>
        /// Bind a buffer as a constant buffer to a shader stage; this flag may NOT be combined with any other bind flag.
        /// </summary>
        ConstantBuffer = 0x4,

        /// <summary>
        /// Bind a buffer or texture to a shader stage; this flag cannot be used with the D3D11_MAP_WRITE_NO_OVERWRITE flag.
        /// </summary>
        ShaderResource = 0x8,

        /// <summary>
        /// Bind an output buffer for the stream-output stage.
        /// </summary>
        StreamOutput = 0x10,

        /// <summary>
        /// Bind a texture as a render target for the output-merger stage.
        /// </summary>
        RenderTarget = 0x20,

        /// <summary>
        /// Bind a texture as a depth-stencil target for the output-merger stage.
        /// </summary>
        DepthStencil = 0x40,

        /// <summary>
        /// Bind an unordered access resource.
        /// </summary>
        UnorderedAccess = 0x80,

        /// <summary>
        /// Set this flag to indicate that a 2D texture is used to receive output from the decoder API.
        /// The common way to create resources for a decoder output is by calling the ID3D11Device::CreateTexture2D method to create an array of 2D textures.
        /// However, you cannot use texture arrays that are created with this flag in calls to ID3D11Device::CreateShaderResourceView.
        /// </summary>
        Decoder = 0x200,

        /// <summary>
        /// Set this flag to indicate that a 2D texture is used to receive input from the video encoder API.
        /// The common way to create resources for a video encoder is by calling the ID3D11Device::CreateTexture2D method to create an array of 2D textures.
        /// However, you cannot use texture arrays that are created with this flag in calls to ID3D11Device::CreateShaderResourceView.
        /// </summary>
        VideoEncoder = 0x400
    }
}
