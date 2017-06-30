using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    /// <summary>
    /// GraphicsCore.dll の GraphicsObject::buffers[8] に対応するクラスです。
    /// </summary>
    public class BufferCollection
    {
        /// <summary>
        /// BufferCollection のコンストラクタです。
        /// </summary>
        /// <param name="_owner">このクラスのインスタンスを保持する Object3D を継承したクラス</param>
        public BufferCollection(Object3D _owner)
        {
            owner = _owner ?? throw new ArgumentNullException("_owner");
        }

        /// <summary>
        /// このクラスのインスタンスを保持する Object3D を継承したクラス
        /// </summary>
        private Object3D owner;

        private Buffer[] buffers = new Buffer[8];

        public Buffer this[int i]
        {
            get
            {
                if (buffers[i] == null)
                    return null;

                if (i < 0 || i >= 8)
                    throw new Exception("BufferCollection のスロット（インデックス）は 0 ~ 7 である必要があります。");

                if (buffers[i].handle != owner.GetBuffer(i))
                    throw new Exception("BufferCollection と GraphicsObject::buffers[8] の対応がとれていないため、Bufferデータを取得できません。");

                return buffers[i];
            }
            set
            {
                buffers[i] = value;
                owner.SetBuffer(value, i);
            }
        }
    }
}
