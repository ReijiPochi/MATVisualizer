using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer
{
    /// <summary>
    /// The return codes from API functions. 
    /// </summary>
    enum HRESULT : uint
    {
        /// <summary>
        /// The specified file was not found.
        /// </summary>
        D3D11_ERROR_FILE_NOT_FOUND = 0x887C0002,

        /// <summary>
        /// The application has exceeded the maximum number of unique state objects per Direct3D device.
        /// The limit is 2^20 for feature levels up to 11.1.
        /// </summary>
        D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS = 0x887C0001,

        /// <summary>
        /// The application has exceeded the maximum number of unique view objects per Direct3D device.
        /// The limit is 2^20 for feature levels up to 11.1.
        /// </summary>
        D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS = 0x887C0003,

        /// <summary>
        /// The application's first call per command list to Map on a deferred context did not use D3D11_MAP_WRITE_DISCARD.
        /// </summary>
        D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD = 0x887C0004,

        /// <summary>
        /// The application made a call that is invalid. Either the parameters of the call or the state of some object was incorrect.
        /// Enable the D3D debug layer in order to see details via debug messages.
        /// </summary>
        DXGI_ERROR_INVALID_CALL = 0x887A0001,

        /// <summary>
        /// The GPU was busy at the moment when the call was made, and the call was neither executed nor scheduled.
        /// </summary>
        DXGI_ERROR_WAS_STILL_DRAWING = 0x887A000A,

        /// <summary>
        /// Attempted to create a device with the debug layer enabled and the layer is not installed.
        /// </summary>
        E_FAIL = 0x80004005,

        /// <summary>
        /// An invalid parameter was passed to the returning function.
        /// </summary>
        E_INVALIDARG = 0x80070057,

        /// <summary>
        /// Direct3D could not allocate sufficient memory to complete the call.
        /// </summary>
        E_OUTOFMEMORY = 0x8007000E,

        /// <summary>
        /// The method call isn't implemented with the passed parameter combination.
        /// </summary>
        E_NOTIMPL = 0x80004001,

        /// <summary>
        /// Alternate success value, indicating a successful but nonstandard completion
        /// (the precise meaning depends on context).
        /// </summary>
        S_FALSE = 1,

        /// <summary>
        /// No error occurred.
        /// </summary>
        S_OK = 0
    }
}
