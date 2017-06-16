using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MATVisualizer.Graphics.Core;
using System.Runtime.InteropServices;

namespace MATVisualizer.Graphics
{
    /// <summary>
    /// Direct3Dで描画されるオブジェクトの抽象クラスです。
    /// </summary>
    public abstract class Object3D
    {
        /// <summary>
        /// GraphicsCore によって生成されたハンドルです。
        /// </summary>
        public IntPtr objectHandle;

        /// <summary>
        /// ハンドルを取得します。
        /// </summary>
        /// <param name="desc"></param>
        protected void Create(GraphicsObjectDescription desc)
        {
            objectHandle = GraphicsObject.Create(desc);
        }

        /// <summary>
        /// オブジェクトをレンダリングされないようにします。
        /// </summary>
        public void Lock()
        {
            if (objectHandle != IntPtr.Zero)
                GraphicsObject.SetLock(objectHandle, true);
        }

        /// <summary>
        /// オブジェクトをレンダリングされるようにします。
        /// </summary>
        public void Unlock()
        {
            if (objectHandle != IntPtr.Zero)
                GraphicsObject.SetLock(objectHandle, false);
        }

        /// <summary>
        /// Shapeデータをオブジェクトに登録します。
        /// </summary>
        /// <param name="shape">登録する Shapeデータ</param>
        /// <param name="slot">登録先のスロット (0 ~ 7)</param>
        protected internal void SetShape(Shape shape, int slot)
        {
            GraphicsObject.SetShape(objectHandle, slot, shape.shapeHandle);
        }

        /// <summary>
        /// Bufferデータをオブジェクトに登録します。
        /// </summary>
        /// <param name="buffer">登録する Bufferデータ</param>
        /// <param name="slot">登録先のスロット (0 = 7)</param>
        protected internal void SetBuffer(Buffer buffer, int slot = 0)
        {
            GraphicsObject.SetBuffer(objectHandle, slot, buffer.handle);
        }

        /// <summary>
        /// オブジェクトに登録されているShapeデータを取得します。
        /// </summary>
        /// <param name="slot">取得元のスロット (0 ~ 7)</param>
        /// <returns>取得したShapeデータのハンドル</returns>
        protected internal IntPtr GetShape(int slot)
        {
            return GraphicsObject.GetShape(objectHandle, slot);
        }
    }
}
