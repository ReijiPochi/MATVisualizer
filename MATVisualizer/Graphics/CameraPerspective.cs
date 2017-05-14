using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Graphics
{
    public class CameraPerspective : Camera
    {
        public CameraPerspective() : base()
        {

        }

        
        public Vector3 Eye { get; set; }
        public Vector3 Target { get; set; }
        public Vector3 Up { get; private set; } = Vector3.UnitY;
        public double FieldOfView { get; set; }

        public override void UpdateCamera()
        {
            Matrix4x4 matrix = Matrix4x4.CreateLookAt(Eye, Target, Up);
            matrix *= Matrix4x4.CreatePerspectiveFieldOfView((float)(Math.PI * (FieldOfView / 180.0)), (float)(Render.ViewPortWidth / Render.ViewPortHeight), 0.001f, 1000.0f);

            _Camera.Update(cameraHandle, ref matrix);
        }

        public override void Zoom(double zoom)
        {
            if (zoom > 0.5) zoom = 0.5;
            else if (zoom < -0.5) zoom = -0.5;

            Vector3 direction = (Eye - Target) * (float)zoom;
            Eye -= direction;
            //Target -= direction;
        }

        public override void Rotate(double x, double y)
        {
            Vector3 direction = Eye - Target;

            float r = Vector3.Distance(direction, Vector3.Zero);
            double theta = Math.Acos(direction.Y / r);
            double phi = Math.Sign(direction.X) * Math.Acos(direction.Z / Math.Sqrt(direction.Z * direction.Z + direction.X * direction.X));

            theta -= y;
            if (theta > Math.PI) theta = Math.PI - 0.001;
            else if (theta < 0)
                theta = 0.001;

            phi -= x;

            Vector3 delta = new Vector3((float)(Math.Sin(theta) * Math.Sin(phi)), (float)Math.Cos(theta), (float)(Math.Sin(theta) * Math.Cos(phi)));
            delta *= r;

            Eye = delta + Target;
        }

        public override void Move(double x, double y)
        {
            Vector3 direction = Eye - Target;
            double r = Vector3.Distance(direction, Vector3.Zero);

            Vector3 deltaEW = Vector3.Normalize(Vector3.Cross(direction, Up));
            Vector3 deltaSN = Vector3.Normalize(Vector3.Cross(deltaEW, direction));

            deltaEW *= (float)(x * r);
            deltaSN *= (float)(y * r);

            Vector3 delta = deltaEW + deltaSN;

            Eye += delta;
            Target += delta;
        }
    }
}
