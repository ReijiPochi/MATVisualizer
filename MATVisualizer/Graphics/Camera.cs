using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public abstract class Camera
    {
        public Camera()
        {
            cameraHandle = _Camera.Create();
        }

        public IntPtr cameraHandle;

        public virtual void UpdateCamera()
        {

        }

        public virtual void Zoom(double zoom)
        {

        }

        public virtual void Rotate(double x, double y)
        {

        }

        public virtual void Move(double x, double y)
        {

        }
    }
}
