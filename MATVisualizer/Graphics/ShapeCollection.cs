using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace MATVisualizer.Graphics
{
    /// <summary>
    /// GraphicsCore.dll の GraphicsObject::shapes[8] に対応するクラスです。
    /// </summary>
    public class ShapeCollection
    {
        /// <summary>
        /// ShapeCollection のコンストラクタです。
        /// </summary>
        /// <param name="_owner">このクラスのインスタンスを保持する Object3D を継承したクラス</param>
        public ShapeCollection(Object3D _owner)
        {
            owner = _owner ?? throw new ArgumentNullException("_owner");
        }

        /// <summary>
        /// このクラスのインスタンスを保持する Object3D を継承したクラス
        /// </summary>
        private Object3D owner;

        private Shape[] shapes = new Shape[8];

        public Shape this[int i]
        {
            get
            {
                if (i < 0 || i >= 8)
                    throw new Exception("ShapeCollection のスロット（インデックス）は 0 ~ 7 である必要があります。");

                if (shapes[i].shapeHandle != owner.GetShape(i))
                    throw new Exception("ShapeCollection と GraphicsObject::shapes[8] の対応がとれていないため、Shapeデータを取得できません。");

                return shapes[i];
            }
            set
            {
                shapes[i] = value;
                owner.SetShape(value, i);
            }
        }

        public int Count { get; protected set; }

        public void Set(Shape shape, int slot)
        {
            if (slot >= 8 || slot < 0)
                throw new Exception("ShapeCollection の slot は 0 ~ 7 である必要があります。");

            shapes[slot] = shape ?? throw new ArgumentNullException("shape");

            owner.SetShape(shape, slot);
        }
    }
}
