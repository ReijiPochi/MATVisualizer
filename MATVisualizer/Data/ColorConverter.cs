using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace MATVisualizer.Data
{
    public class ColorConverter
    {
        public static Vector3 HSVtoRGB(Vector3 hsv)
        {
            Vector3 rgb = new Vector3(hsv.Z);

            if (hsv.X > 1.0f) hsv.X = 1.0f;
            else if (hsv.X < 0.0f) hsv.X = 0.0f;

            hsv.X *= 6.0f;
            int i = (int)hsv.X;

            float f = hsv.X - i;

            switch (i)
            {
                case 0:
                    rgb.Y *= 1 - hsv.Y * (1 - f);
                    rgb.Z *= 1 - hsv.Y;
                    break;

                case 1:
                    rgb.X *= 1 - hsv.Y * f;
                    rgb.Z *= 1 - hsv.Y;
                    break;

                case 2:
                    rgb.X *= 1 - hsv.Y;
                    rgb.Z *= 1 - hsv.Y * (1 - f);
                    break;

                case 3:
                    rgb.X *= 1 - hsv.Y;
                    rgb.Y *= 1 - hsv.Y * f;
                    break;

                case 4:
                    rgb.X *= 1 - hsv.Y * (1 - f);
                    rgb.Y *= 1 - hsv.Y;
                    break;

                case 5:
                case 6:
                    rgb.Y *= 1 - hsv.Y;
                    rgb.Z *= 1 - hsv.Y * f;
                    break;

                default:
                    break;
            }

            return rgb;
        }
    }
}
